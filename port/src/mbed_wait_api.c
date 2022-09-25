#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mbed_wait_api.h"


void wait(float s)
{
    vTaskDelay(1000 * s / portTICK_PERIOD_MS);
}

/*  The actual time delay may be up to one timer tick less - 1 msec */
void wait_ms(int ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);   
}

/*  The actual time delay may be 1 less usec */
void wait_us(int us)
{
    vTaskDelay(1/ portTICK_PERIOD_MS);
}