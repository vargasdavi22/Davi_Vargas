#include "threeeyes_task.h"
#include "robot_control.h"
#include "driver/gpio.h"

static const char *TAG = "three_eyes";

#define STOP_DISTANCE_CM    10.0f
#define RELEASE_DISTANCE_CM 15.0f
#define CLEAR_COUNT_LIMIT   5
#define GPIO_WCET_THREEEYES GPIO_NUM_47

portTASK_FUNCTION(Threeeyes, args)
{
    ThreeEyes_Init();

    ultrasonic_value_t sensor[3];
    static int clear_count = 0;

    while (1)
    {
        // Início do processamento da tarefa: Liga o pino do osciloscópio
        //gpio_set_level(GPIO_WCET_THREEEYES, 1);

        ThreeEyes_TrigAndWait(pdMS_TO_TICKS(50));
        ThreeEyes_Read(&sensor[0], &sensor[1], &sensor[2]);

        uint32_t min_ticks = 0xFFFFFFFF;
        int near_sensor = -1;

        for (int i = 0; i < 3; i++)
        {
            if (sensor[i].isUpdated == pdTRUE && sensor[i].tof_ticks < min_ticks)
            {
                min_ticks = sensor[i].tof_ticks;
                near_sensor = i;
            }
        }

        if (near_sensor >= 0)
        {
            float distance = (min_ticks * (1000000.0 / esp_clk_apb_freq())) / 58.0f;

            if (distance <= STOP_DISTANCE_CM)
            {
                obstacle_detected = 1;
                clear_count = 0;
            }
            else if (distance >= RELEASE_DISTANCE_CM)
            {
                clear_count++;
                if (clear_count >= CLEAR_COUNT_LIMIT)
                {
                    obstacle_detected = 0;
                    clear_count = 0;
                }
            }
        }

        // Incrementa o batimento cardíaco da tarefa
        threeeyes_heartbeat++;

        // Fim do processamento da tarefa: Desliga o pino do osciloscópio
        //pio_set_level(GPIO_WCET_THREEEYES, 0);

        vTaskDelay(pdMS_TO_TICKS(60)); 
    }
}