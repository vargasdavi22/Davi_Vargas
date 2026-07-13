#include "ir_line_task.h"
#include "robot_control.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define IR_ACTIVE_LEVEL 0
#define GPIO_WCET_IR   GPIO_NUM_3

#define INFRA_RED_VERY_LEFT_GPIO   GPIO_NUM_15
#define INFRA_RED_LEFT_GPIO        GPIO_NUM_16
#define INFRA_RED_MIDDLE_GPIO      GPIO_NUM_17
#define INFRA_RED_RIGHT_GPIO       GPIO_NUM_18
#define INFRA_RED_VERY_RIGHT_GPIO  GPIO_NUM_8

#define IR_MASK ((1ULL << INFRA_RED_VERY_LEFT_GPIO) | \
                 (1ULL << INFRA_RED_LEFT_GPIO) | \
                 (1ULL << INFRA_RED_MIDDLE_GPIO) | \
                 (1ULL << INFRA_RED_RIGHT_GPIO) | \
                 (1ULL << INFRA_RED_VERY_RIGHT_GPIO))

static const char *TAG = "IR_TASK";

portTASK_FUNCTION(IR_Task, arg)
{
    gpio_config_t ir_config = {
        .pin_bit_mask = IR_MASK,
        .mode = GPIO_MODE_INPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&ir_config);

    while (1)
    {
        // Início do processamento da tarefa: Liga o pino do osciloscópio
        //gpio_set_level(GPIO_WCET_IR, 1);

        int s[5];
        s[0] = (gpio_get_level(INFRA_RED_VERY_LEFT_GPIO) == IR_ACTIVE_LEVEL);
        s[1] = (gpio_get_level(INFRA_RED_LEFT_GPIO) == IR_ACTIVE_LEVEL);
        s[2] = (gpio_get_level(INFRA_RED_MIDDLE_GPIO) == IR_ACTIVE_LEVEL);
        s[3] = (gpio_get_level(INFRA_RED_RIGHT_GPIO) == IR_ACTIVE_LEVEL);
        s[4] = (gpio_get_level(INFRA_RED_VERY_RIGHT_GPIO) == IR_ACTIVE_LEVEL);

        if (s[0] && s[1] && !s[2] && !s[3] && !s[4])       line_error = -2;
        else if (!s[0] && s[1] && s[2] && !s[3] && !s[4])  line_error = -1;
        else if (!s[0] && !s[1] && s[2] && !s[3] && !s[4]) line_error = 0;
        else if (!s[0] && !s[1] && s[2] && s[3] && !s[4])  line_error = 1;
        else if (!s[0] && !s[1] && !s[2] && s[3] && s[4])  line_error = 2;
        else if (s[0]) line_error = -2;
        else if (s[1]) line_error = -1;
        else if (s[2]) line_error = 0;
        else if (s[3]) line_error = 1;
        else if (s[4]) line_error = 2;
        
        
// Incrementa o batimento cardíaco da tarefa de linha para o Watchdog fiscalizar
        ir_heartbeat++;

        // Fim do processamento da tarefa: Desliga o pino do osciloscópio (GPIO 4)
        //gpio_set_level(GPIO_NUM_4, 0);
      

        vTaskDelay(pdMS_TO_TICKS(20)); // Período de 20ms (RM)
    }
}