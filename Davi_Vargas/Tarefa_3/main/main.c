#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

// Headers das tarefas existentes
#include "ir_line_task.h"
#include "wheel_task.h"
#include "threeeyes_task.h"

// Header de controle global
#include "robot_control.h"

// Protótipo exato da Monitor_Task para o xTaskCreate reconhecer sem precisar de .h
void Monitor_Task(void *arg); 

// Definição física dos pinos seguros de GPIO para medição de WCET no osciloscópio
#define GPIO_WCET_IR         GPIO_NUM_3
#define GPIO_WCET_WHEEL      GPIO_NUM_46
#define GPIO_WCET_THREEEYES  GPIO_NUM_47
#define GPIO_WCET_MONITOR    GPIO_NUM_48

// Alocação real das variáveis globais do robô para o Linker
volatile int line_error = 0;
volatile float left_speed_ref = 0.0f;
volatile float right_speed_ref = 0.0f;
volatile float encoder_left_speed = 0.0f;
volatile float encoder_right_speed = 0.0f;
volatile int obstacle_detected = 0;

volatile int fault_mode = 0;
volatile uint32_t wheel_heartbeat = 0;
volatile uint32_t threeeyes_heartbeat = 0;
volatile uint32_t ir_heartbeat = 0;

void app_main(void)
{
    ESP_LOGI("MAIN", "Inicializando pinos de GPIO para medição do WCET...");

    // Configura os 4 pinos seguros como saída digital simples
    gpio_config_t wcet_gpio_cfg = {
        .pin_bit_mask = (1ULL << GPIO_WCET_IR) | 
                        (1ULL << GPIO_WCET_WHEEL) | 
                        (1ULL << GPIO_WCET_THREEEYES) | 
                        (1ULL << GPIO_WCET_MONITOR),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&wcet_gpio_cfg);

    // Garante pinos em nível baixo no início
    gpio_set_level(GPIO_WCET_IR, 0);
    gpio_set_level(GPIO_WCET_WHEEL, 0);
    gpio_set_level(GPIO_WCET_THREEEYES, 0);
    gpio_set_level(GPIO_WCET_MONITOR, 0);

    ESP_LOGI("MAIN", "Criando tarefas com escalonamento Rate Monotonic (RM)...");

    // Atribuição de Prioridades por Rate Monotonic (RM)
    xTaskCreate(IR_Task, "ir_task", configMINIMAL_STACK_SIZE * 3, NULL, 8, NULL);
    xTaskCreate(wheel_ctrl, "wheel_ctrl", configMINIMAL_STACK_SIZE * 5, NULL, 7, NULL);
    xTaskCreate(Threeeyes, "threeeyes", configMINIMAL_STACK_SIZE * 4, NULL, 6, NULL);
    xTaskCreate(Monitor_Task, "monitor_task", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
}
    // ATRIBUIÇÃO DE PRIORIDADES POR RATE MONOTONIC (RM)
    // Regra: Menor Período (maior frequência) -> Prioridade Mais Alta.
    //
    // 1. IR_Task (Leitura da Linha):   T = 20ms  -> Freq Máxima -> Prioridade 8
    // 2. wheel_ctrl (Controle PID):    T = 50ms  -> Freq Alta   -> Prioridade 7
    // 3. Threeeyes (Ultrassónicos):    T = 60ms  -> Freq Média  -> Prioridade 6
    // 4. Monitor_Task (Watchdog):      T = 200ms -> Freq Baixa  -> Prioridade 5
    // =================================================================
