#include "robot_control.h"
#include "wheel.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_task_wdt.h"
static const char *TAG = "MONITOR";
#define GPIO_WCET_MONITOR GPIO_NUM_48

portTASK_FUNCTION(Monitor_Task, arg) 
{
    uint32_t last_wheel_hb = 0;
    uint32_t last_threeeyes_hb = 0;
    uint32_t last_ir_hb = 0; // Armazena o último estado da tarefa de linha
    int fault_counter = 0;

    // --- CONFIGURAÇÃO DO WATCHDOG DE HARDWARE (TWDT) ---
    // Insere a própria tarefa do Monitor no Watchdog nativo do silício do ESP32-S3.
    // Se esta tarefa travar, o chip reinicia por hardware.
    esp_task_wdt_add(NULL); 

    while (1) 
    {
        //  Início do processamento da tarefa: Liga o pino do osciloscópio
       // gpio_set_level(GPIO_WCET_MONITOR, 1);

        // Alimenta o Watchdog de Hardware do ESP-IDF
        esp_task_wdt_reset();

        if (!fault_mode) 
        {
            // Se QUALQUER uma das três travar (parar de incrementar o batimento), detecta a falha.
            if (wheel_heartbeat == last_wheel_hb || 
                threeeyes_heartbeat == last_threeeyes_hb ||
                ir_heartbeat == last_ir_hb) 
            {
                fault_counter++;
                if (fault_counter >= 3) 
                {
                    ESP_LOGE(TAG, "SISTEMA CRÍTICO TRAVOU! Iniciando rotação de falha.");
                    fault_mode = 1;
                }
            } 
            else 
            {
                fault_counter = 0;
                last_wheel_hb = wheel_heartbeat;
                last_threeeyes_hb = threeeyes_heartbeat;
                last_ir_hb = ir_heartbeat; // Atualiza o histórico do batimento de linha
            }
        }

        // Fim do processamento da tarefa: Desliga o pino do osciloscópio
        //gpio_set_level(GPIO_WCET_MONITOR, 0);

        vTaskDelay(pdMS_TO_TICKS(200)); 
    }
}