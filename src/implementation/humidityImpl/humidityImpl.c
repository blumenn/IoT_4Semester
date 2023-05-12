#include <stdint.h>
#include <hih8120.h>
#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>
#include "humidityImpl.h"


uint16_t temp;
extern SemaphoreHandle_t xTestSemaphore;


void humimpl_measure(){
    if(xSemaphoreTake(xTestSemaphore,pdMS_TO_TICKS(200))==pdTRUE){
    if ( HIH8120_OK != hih8120_wakeup() )
{
       // Something went wrong
       // Investigate the return code further
}
_delay_ms(1000);
if ( HIH8120_OK !=  hih8120_measure() )
{
       // Something went wrong
       // Investigate the return code further
}
temp = hih8120_getHumidity();
xSemaphoreGive(xTestSemaphore);
    }
}
uint16_t humimpl_getMeasurement(){
    
return temp;
}