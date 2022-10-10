//Semaphore is used to send a message

#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

TaskHandle_t myTask1Handle = NULL;
TaskHandle_t myTask2Handle = NULL;

SemaphoreHandle_t xSemaphore = NULL;  // First we created a handle for semaphore

TaskHandle_t ISR = NULL;

#define CONFIG_LED_PIN 2
#define ESP_INTR_FLAG_DEFAULT 0
#define CONFIG_BUTTON_PIN 0



void task1(void *arg)
{
    while (1)
    {
        printf("sent message! [%d]", xTaskGetTickCount());
        xSemaphoreGive(xSemaphore);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void task2(void *arg)
{
    while (1)
    {
    if(xSemaphoreTake(xSemaphore, portMAX_DELAY)){
        printf("got message! [%d]", xTaskGetTickCount());
    }      
    }
}

void app_main()
{
    xSemaphore = xSemaphoreCreateBinary();
    xTaskCreate(task1, "task1", 4096, NULL, 10, &myTask1Handle);
    xTaskCreatePinnedToCore(task2, "task2", 4096, NULL, 10, &myTask2Handle, 1);
}