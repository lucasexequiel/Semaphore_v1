#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

//Tags to use on "logging functions"
#define TASK1_TAG "TASK_1" 
#define TASK2_TAG "TASK_2"

//task handle
TaskHandle_t myTask1Handle = NULL;
TaskHandle_t myTask2Handle = NULL;

//ISR handle
TaskHandle_t ISR = NULL;

void IRAM_ATTR button_isr_handler (void* arg){
    xTaskResumeFromISR(ISR);
}

//Task that will react to button clicks
void button_task(void *arg){
   
    while ((1))
    {
        vTaskSuspend(NULL);
        ESP_LOGE("interrupt","Button pressed! \n");
    }
}

void task1(void *arg)
{
    char buffer[20] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x11, 0x22, 0x33, 0x44, 0x55, 0x11, 0x22, 0x33, 0x44, 0x55, 0x11, 0x22, 0x33, 0x44, 0x55};
    while (1)
    {
        //The principal diference between printf and ESP_LOGI is that Log function needs a tag.
        ESP_LOGI(TASK1_TAG,"Hello from task1 \n");        //Logging function used to write a string to console with green color (I means info)
        vTaskDelay(1000 / portTICK_RATE_MS);
        ESP_LOG_BUFFER_HEX("buff", buffer, 20); // Tag, array and how much caracter we will write
        printf("Timestamp = %d Tick count = %d \n", esp_log_timestamp(), xTaskGetTickCount());
    }
}

void task2(void *arg)
{
   while(1)
    {
        ESP_LOGW("TASK_2","Hello from task2 \n"); //ESP_LOGW - W come from warning
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}
void app_main()
{
    xTaskCreate(task1, "task1", 4096, NULL, 10, &myTask1Handle);
    xTaskCreatePinnedToCore(task2, "task2", 4096, NULL, 10, &myTask2Handle, 1);
}