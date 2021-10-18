/*
    Operative System for FollowING device-GIBIO
    Version 1
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

// Globals

static SemaphoreHandle_t xMutexADC;
static SemaphoreHandle_t xMutexTrama;

void vTask1(void *pvParameters)
{
    while(1)
    {
        if(xSemaphoreTake(xMutexADC, 0) == pdTRUE)
	{
	    printf("Tarea1: logre tomar mutex ADC\n");
	    xSemaphoreGive(xMutexADC);
	    vTaskDelay(1000 / portTICK_PERIOD_MS);

	}
    }

}
void vTask2(void *pvParameters)
{
    while(1)
    {
	if(xSemaphoreTake(xMutexADC, 0) == pdTRUE)
	{
	    printf("Tarea2: logre tomar mutex ADC\n");
	    xSemaphoreGive(xMutexADC);
	    vTaskDelay(3000 / portTICK_PERIOD_MS);
	}
    }
}

void app_main()
{
    printf("Hello world!\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is ESP8266 chip with %d CPU cores, WiFi, ",
            chip_info.cores);

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
    xMutexADC = xSemaphoreCreateMutex();
    xMutexTrama = xSemaphoreCreateMutex();

    if(xMutexADC != NULL && xMutexTrama != NULL){
        xTaskCreate(vTask1, "Task1", 1000, NULL, 2, NULL);
        xTaskCreate(vTask2, "Task2", 1000, NULL, 1, NULL);
    }

}
