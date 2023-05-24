#include "servoHandler.h"
#include "../InterfaceWrapper/Wrapper.h"
#include "implementation/servo/servoImpl.h"
#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>
#include <queue.h>
static config configuration;
static SemaphoreHandle_t servoTestSemaphore;
extern QueueHandle_t xQueue;

void run_servo_handler_task(void *pvParameters)
{
	for (;;){
		servo_measuring();
		vTaskDelay(pdMS_TO_TICKS(10000));
	}
}

void create_servo_handler_task()
{
	BaseType_t taskCreated;
	taskCreated = xTaskCreate(
	run_servo_handler_task,
	"servo_handler_task",
	configMINIMAL_STACK_SIZE+100,
	NULL,
	5,
	NULL
	);
}

void servo_handler_init(void){
servo_init();
create_servo_handler_task();
	if ( servoTestSemaphore == NULL )  // Check to confirm that the Semaphore has not already been created.
	{
		servoTestSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore.
		if ( ( servoTestSemaphore ) != NULL )
		{
			xSemaphoreGive( ( servoTestSemaphore ) );  // Make the mutex available for use, by initially "Giving" the Semaphore.
		}
	}

return;
}
void servo_set_config(uint16_t maxHumSetting,
    uint16_t minHumSetting,
    uint16_t maxTempSetting,
    uint16_t minTempsetting,
    uint16_t maxCo2Setting,
    uint16_t minCo2Setting){
         if(xSemaphoreTake(servoTestSemaphore,pdMS_TO_TICKS(200))==pdTRUE){
        
            configuration.maxHumSetting = maxHumSetting;
configuration.minHumSetting = minHumSetting;
configuration.maxTempSetting = maxTempSetting;
configuration.minTempsetting = minTempsetting;
configuration.maxCo2Setting = maxCo2Setting;
configuration.minCo2Setting = minCo2Setting;
xSemaphoreGive(servoTestSemaphore);
        }
}






void servo_measuring(){
    if(xSemaphoreTake(servoTestSemaphore,pdMS_TO_TICKS(200))==pdTRUE){
        {
    latestData data = get_latestData();
    uint16_t temp = data.temp.data;
    uint16_t hum = data.hum.data;
   uint16_t co2 = data.co2.data;
    if (configuration.maxHumSetting<hum)
    {
        servoOpenWindow();
        xSemaphoreGive(servoTestSemaphore);
        return ;
    }
    if (configuration.maxCo2Setting < co2)
    {
        servoOpenWindow();
        xSemaphoreGive(servoTestSemaphore);
        return ;
    }
    if (configuration.maxTempSetting<temp)
    {
        servoOpenWindow();
        xSemaphoreGive(servoTestSemaphore);
        return ;
    }
    if (configuration.minCo2Setting<co2)
    {
        
        xSemaphoreGive(servoTestSemaphore);
		return;
    }
    if(configuration.minHumSetting< hum){
        xSemaphoreGive(servoTestSemaphore);
		return;
    }
    if (configuration.minTempsetting< temp)
    {
        xSemaphoreGive(servoTestSemaphore);
        return;
    }
    servoCloseWindow();
    xSemaphoreGive(servoTestSemaphore);
        }
    return;
    
    
    
}
}