#include "wheel_task.h"
#include "robot_control.h"
#include "wheel.h"
#include "pid_ctrl.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include <stdint.h>
#include <math.h>

static const char *TAG = "WHEEL_TASK";

// PARÂMETROS TEMPORAIS E CONFIGURAÇÃO DA TAREFA (REQUISITOS DE TEMPO REAL)
#define CONTROL_PERIOD_MS     50
#define CONTROL_PERIOD_S      ((float)CONTROL_PERIOD_MS / 1000.0f)
// Pino digital mapeado para o canal do osciloscópio para medição real do WCET
#define GPIO_WCET_WHEEL       GPIO_NUM_46

// CONFIGURAÇÕES DE VELOCIDADE REAL (FÍSICA EM m/s) E DIRETRIZES DO PID
#define V_REF_BASE            0.50f 
#define MAX_W_REF             3.50f 
#define MAX_WHEEL_REF         1.20f  
#define MAX_PHYSICAL_SPEED_MS 1.0f 

// Ganhos do PID Angular
#define ANGULAR_KP 1.40f 
#define ANGULAR_KI 0.0f
#define ANGULAR_KD 0.75f 

// Ganhos do PID de Velocidade das Rodas (Malha Interna em Malha Fechada via Encoder)
#define WHEEL_KP 0.8f
#define WHEEL_KI 0.1f
#define WHEEL_KD 0.02f

// Saturações de Hardware (PWM) para proteção elétrica e estabilidade de torque
#define MIN_PWM_FACTOR 0.55f 
#define MAX_PWM_FACTOR 0.95f 
#define DECEL_STEP_PWM  25   

// Função auxiliar de saturação matemática
static float clamp_float(float value, float min, float max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// TAREFA DE TEMPO REAL: CONTROLE DOS MOTORES (PRIORIDADE RM = 7)
portTASK_FUNCTION(wheel_ctrl, arg)
{
    // Contador interno para disparar o travamento de teste (100 ciclos x 50ms = 5 segundos)
    int contador_ciclos_teste = 0;

    // Inicialização dos periféricos de hardware dos motores
    wheel_Init();
    wheel_GoForward();

    // Leitura inicial dos Encoders para garantir o Delta zero no primeiro ciclo
    int last_left = 0, last_right = 0;
    wheel_GetEndoderPulses(&last_left, &last_right);

    float current_pwm_left = 0.0f, current_pwm_right = 0.0f;

    // Instanciação dos blocos de controle PID fornecidos pela biblioteca pid_ctrl
    pid_ctrl_block_handle_t angular_pid = NULL, left_pid = NULL, right_pid = NULL;

    pid_ctrl_config_t angular_pid_config = {
        .init_param = {.kp = ANGULAR_KP, .ki = ANGULAR_KI, .kd = ANGULAR_KD,
                       .max_output = MAX_W_REF, .min_output = -MAX_W_REF,
                       .max_integral = 1.0f, .min_integral = -1.0f, .cal_type = PID_CAL_TYPE_POSITIONAL}
    };

    pid_ctrl_config_t wheel_pid_config = {
        .init_param = {.kp = WHEEL_KP, .ki = WHEEL_KI, .kd = WHEEL_KD,
                       .max_output = 0.5f, .min_output = -0.5f,
                       .max_integral = 0.2f, .min_integral = -0.2f, .cal_type = PID_CAL_TYPE_POSITIONAL}
    };

    pid_new_control_block(&angular_pid_config, &angular_pid);
    pid_new_control_block(&wheel_pid_config, &left_pid);
    pid_new_control_block(&wheel_pid_config, &right_pid);

    while (1)
    {
        // INSTRUMENTAÇÃO DO WCET: Sinal em nível ALTO inicia a medição no osciloscópio
        //gpio_set_level(GPIO_WCET_WHEEL, 1);

       //SIMULAÇÃO DE ERRO DO WATCHDOG //
      contador_ciclos_teste++;
      if (contador_ciclos_teste >= 100) 
      {
		    ESP_LOGW(TAG, "Simulando falha de software: travando em loop infinito.");
		  while(1)
		   {
		       vTaskDelay(pdMS_TO_TICKS(100)); 
			   }
			}

        // SE O SISTEMA ENTRAR EM FALHA/TRAVAMENTO DETECTADO PELO MONITOR
        if (fault_mode)
        {
            // Força o robô a girar indefinidamente no próprio eixo aplicando rotação diferencial
            wheel_SetVel((uint32_t)(BDC_MCPWM_DUTY_TICK_MAX * 0.65f), 0); 
            
            // Sinaliza atividade para o Watchdog e encerra a medição de tempo de processamento
            wheel_heartbeat++; 
            gpio_set_level(GPIO_WCET_WHEEL, 0);
            vTaskDelay(pdMS_TO_TICKS(CONTROL_PERIOD_MS));
            continue; // Desvia do fluxo normal do PID
        }

        //SE ENCONTRAR OBSTÁCULO A 10CM (SINALIZADO PELO ULTRASSÔNICO)
        if (obstacle_detected)
        {
            // Aplica rampa de desaceleração controlada (frenagem) até zerar o PWM
            if (current_pwm_left > 0.0f || current_pwm_right > 0.0f) 
            {
                current_pwm_left -= DECEL_STEP_PWM;
                current_pwm_right -= DECEL_STEP_PWM;
                if (current_pwm_left < 0.0f) current_pwm_left = 0.0f;
                if (current_pwm_right < 0.0f) current_pwm_right = 0.0f;
                wheel_SetVel((uint32_t)current_pwm_left, (uint32_t)current_pwm_right);
            }
            else
            {
                wheel_SetVel(0, 0); // Totalmente parado diante do obstáculo
            }

            // Atualiza histórico dos encoders para evitar picos de leitura no retorno
            wheel_GetEndoderPulses(&last_left, &last_right);
            wheel_heartbeat++;
            gpio_set_level(GPIO_WCET_WHEEL, 0);
            vTaskDelay(pdMS_TO_TICKS(CONTROL_PERIOD_MS));
            continue; // Desvia do fluxo normal do PID
        }

        // TRATAMENTO DA MALHA FECHADA: LEITURA E CÁLCULO DE VELOCIDADE REAL
        int current_left = 0, current_right = 0;
        wheel_GetEndoderPulses(&current_left, &current_right);

        int delta_left = abs(current_left - last_left);
        int delta_right = abs(current_right - last_right);
        last_left = current_left;
        last_right = current_right;

        // Converte pulsos do encoder direto para velocidade escalar real (m/s)
        float metros_por_pulso = (2.0f * M_PI * WHELL_RADIUS) / (float)WHEELS_ENCODER_PPR;
        encoder_left_speed = ((float)delta_left * metros_por_pulso) / CONTROL_PERIOD_S;
        encoder_right_speed = ((float)delta_right * metros_por_pulso) / CONTROL_PERIOD_S;

        // CONTROLE EM CASCATA: CINEMÁTICA INVERSA + PID
        float v_ref = V_REF_BASE; 
        float w_ref = 0.0f;       

        // PID Angular calcula a velocidade angular (w_ref) baseada no erro de linha
        pid_compute(angular_pid, (float)line_error, &w_ref);
        w_ref = clamp_float(w_ref, -MAX_W_REF, MAX_W_REF);

        // Cinemática Inversa distribui as referências de m/s individuais para cada roda
        left_speed_ref = clamp_float(v_ref - w_ref, 0.0f, MAX_WHEEL_REF);
        right_speed_ref = clamp_float(v_ref + w_ref, 0.0f, MAX_WHEEL_REF);

        // PIDs de Roda calculam o ajuste necessário baseado no erro da velocidade real medido pelo encoder
        float left_error = left_speed_ref - encoder_left_speed;
        float right_error = right_speed_ref - encoder_right_speed;
        float left_speed_correction = 0.0f, right_speed_correction = 0.0f;

        pid_compute(left_pid, left_error, &left_speed_correction);
        pid_compute(right_pid, right_error, &right_speed_correction);

        // Composição final das velocidades corrigidas
        float final_left_speed = clamp_float(left_speed_ref + left_speed_correction, 0.0f, MAX_PHYSICAL_SPEED_MS);
        float final_right_speed = clamp_float(right_speed_ref + right_speed_correction, 0.0f, MAX_PHYSICAL_SPEED_MS);

        // Mapeamento linear da velocidade em m/s convertendo para duty-cycle bruto do PWM
        float pwm_left = (final_left_speed / MAX_PHYSICAL_SPEED_MS) * BDC_MCPWM_DUTY_TICK_MAX;
        float pwm_right = (final_right_speed / MAX_PHYSICAL_SPEED_MS) * BDC_MCPWM_DUTY_TICK_MAX;
        float pwm_min = BDC_MCPWM_DUTY_TICK_MAX * MIN_PWM_FACTOR;
        float pwm_max = BDC_MCPWM_DUTY_TICK_MAX * MAX_PWM_FACTOR;

        // Clamping final e tratamento de banda morta para evitar zumbido parado
        pwm_left = (left_speed_ref <= 0.02f) ? 0.0f : clamp_float(pwm_left, pwm_min, pwm_max);
        pwm_right = (right_speed_ref <= 0.02f) ? 0.0f : clamp_float(pwm_right, pwm_min, pwm_max);

        current_pwm_left = pwm_left;
        current_pwm_right = pwm_right;

        // Atuação física nos drivers de potência
        wheel_SetVel((uint32_t)pwm_left, (uint32_t)pwm_right);

        //Incremento do Watchdog de software (Heartbeat)
        wheel_heartbeat++;

        // INSTRUMENTAÇÃO DO WCET: Final do processamento real derruba o sinal para nível BAIXO
        //gpio_set_level(GPIO_WCET_WHEEL, 0);

        // Liberação de CPU: Garante a periodicidade estrita de 50ms para o escalonador RM
        vTaskDelay(pdMS_TO_TICKS(CONTROL_PERIOD_MS)); 
    }
}