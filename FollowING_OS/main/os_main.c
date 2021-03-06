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
static SemaphoreHandle_t xMutexTx;

void AdquisicionADC(void *pvParameters)
{
    uint16_t i;
    while(1)
    {
        if(xSemaphoreTake(xMutexADC, portMAX_DELAY) == pdTRUE)
	{
		// ACA iria codigo de adquisicion del ADC
		// pasa datos q va recibiendo al bufferADC
	    // codigo para probar funcionamiento
	    for(i = 1; i < 11; i ++)
	    {
	        printf("Adquisicion del ADC %d\n", i);
		//vTaskDelay(500 / portTICK_PERIOD_MS);
	    }
	    xSemaphoreGive(xMutexADC);
	    vTaskDelay(1000 / portTICK_PERIOD_MS);

	}
    }

}
void ValidacionTrama(void *pvParameters)
{
    uint16_t cont;
    while(1)
    {
	if(xSemaphoreTake(xMutexADC, portMAX_DELAY) == pdTRUE)
	{
	    if(xSemaphoreTake(xMutexTrama, portMAX_DELAY) == pdTRUE)
	    {
		    // ACA IRIA CODIGO DE VALIDACION DE TRAMA
		    // Valida datos en bufferADC con lo q deberia
		    // haber en la trama
		// codigo para probar funcionamiento
	        for(cont = 1; cont < 4; cont ++)
	        {
	            printf("Validacion de trama....%d\n", cont);
		    //vTaskDelay(1000 / portTICK_PERIOD_MS);
	        }
		xSemaphoreGive(xMutexTrama);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	    }
	    xSemaphoreGive(xMutexADC);
	}
    }
}

void ArmadoTrama(void *pvParameters)
{
    uint16_t cont;
    while(1)
    {
	if(xSemaphoreTake(xMutexTrama, portMAX_DELAY) == pdTRUE)
	{
	    if(xSemaphoreTake(xMutexTx, portMAX_DELAY) == pdTRUE)
	    {
			// ACA IRIA CODIGO DE ARMADO DE LA TRAMA
			// pasar de datos de bufferADC a bufferTrama
		// codigo para probar funcionamiento
	        for(cont = 1; cont < 4; cont ++)
	        {
                    printf("Armado de trama... %d\n", cont);
		    //vTaskDelay(1000 / portTICK_PERIOD_MS);
		}
		xSemaphoreGive(xMutexTx);
	    }
	    xSemaphoreGive(xMutexTrama);
	    vTaskDelay(5000 / portTICK_PERIOD_MS);
	}
    }
}

void Transmision(void *pvParameters)
{
    uint16_t cont;
    while(1)
    {
	if(xSemaphoreTake(xMutexTx, portMAX_DELAY) == pdTRUE)
	{
		// ACA IRIA CODIGO DE TRANSMISION
		// enviar datos de bufferTrama a traves de wifi
		// al servidor, con protocolo MQTT
	    // codigo de prueba para probar funcionamiento
	    for(cont = 1; cont < 4; cont ++)
	    {
		printf("Transmision...%d\n", cont);
		//vTaskDelay(1000 / portTICK_PERIOD_MS);
	    }
	    xSemaphoreGive(xMutexTx);
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
    xMutexTx = xSemaphoreCreateMutex();
    
    if(xMutexADC != NULL && xMutexTrama != NULL && xMutexTx != NULL)
    {
        xTaskCreate(AdquisicionADC, "Adquisicion", 1000, NULL, 4, NULL);
        xTaskCreate(ValidacionTrama, "Validacion", 1000, NULL, 3, NULL);
	xTaskCreate(ArmadoTrama, "ArmadoTrama", 1000, NULL, 2, NULL);
	xTaskCreate(Transmision, "Transmision", 1000, NULL, 1, NULL);
    }

}
