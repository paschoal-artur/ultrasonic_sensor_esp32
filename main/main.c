#include <stdio.h>
#include <stdbool.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ultrasonic.h>
#include <esp_err.h>

#define DISTANCIA_MAXIMA_CM 500 //! 5m máximo
#define PINO_TRIGGER GPIO_NUM_5
#define PINO_ECHO GPIO_NUM_18

void teste_ultrasom (void *pvParameters)
{
    ultrasonic_sensor_t sensor  = {
        .trigger_pin = PINO_TRIGGER,
        .echo_pin = PINO_ECHO,
    };

    ultrasonic_init(&sensor);

//Loop infinito para executar a medição da distância
    while(true)
    {
        float distancia;
        esp_err_t resultado = ultrasonic_measure(&sensor, DISTANCIA_MAXIMA_CM,  &distancia);
        if (resultado != ESP_OK) 
        {
            printf("Error %d: ", resultado);
            switch(resultado)
            {
                case ESP_ERR_ULTRASONIC_PING:
                    printf("Cannot ping (device is in invalid state)\n");
                    break;
                case ESP_ERR_ULTRASONIC_PING_TIMEOUT:
                    printf("Ping timeout (no device found)\n");
                    break;
                case ESP_ERR_ULTRASONIC_ECHO_TIMEOUT:
                    printf("Echo timeout (i.e. distance too big)\n");
                    break;
                default:
                    printf("%s\n", esp_err_to_name(resultado));
            }
        }
        else
        {
            printf("Distance: %0.04f cm\n", distancia*100);
        }
        
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void app_main()
{
    xTaskCreate(teste_ultrasom, "teste_ultrasom", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
}