/**2.Create 3 Realtime tasks each with periodicity T1=1000MS,T2=2000ms,T3=5000ms.Also create use oneshot software timer to trigger a callback function from T3 after 10000ms**/

#include<stdio.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include<freertos/task.h>
#include<freertos/timer.h>
#include<freertos/semphr.h>

TaskHandle_t xHandle_1;
TaskHandle_t xHandle_2;
TaskHandle_t xHandle_3;
SemaphoreHandle_t smf;
TimerHandle_t tmr;
int count=0;

void task1(void *pv)
{
	printf("Task 1 with periodicity 1000ms\n");
	vTaskDelay(1000/portTICK_PERIOD_MS);
}

void task2(void *pv)
{
	printf("Task 1 with periodicity 2000ms\n");
	vTaskDelay(2000/portTICK_PERIOD_MS);
}

void task3(void *pv)
{
	printf("Task 1 with periodicity 5000ms\n");
	vTaskDelay(5000/portTICK_PERIOD_MS);
}
void ISR(void *pv)
{
	int aux=0;
	xSemaphoreGiveFromISR(smf,&aux);
	if(aux)
	{
		portYIELD_FROM_ISR();
	}
}
void vBacklightTimerCallback(TimerHandle_t pxtimer)
{
	vSetBackLightState(BACKLIGHT_OFF);
}	
void vKeyPressEventInterruptHandler( void *pv )
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
 	vSetBacklightState( BACKLIGHT_ON );
 	if( xTimerStartFromISR( xBacklightTimer,&xHigherPriorityTaskWoken ) != pdPASS )
    	{
    		printf("one-shot timer\n");
   	}
   	if( xHigherPriorityTaskWoken != pdFALSE )
    	{
    		portYIELD_FROM_ISR();
    	}
}
void app_main()
{
	smf=xSemaphoreCreateBinary();
	tmr=xTimerCreate("tmr_smf",pdMS_TO_TICK(100),true,0,ISR);
	xTimerStart(tmr,pdMS_TO_TICKS(100));
	while(1)
	{
		if(xSemaphoreTake(smf,portMAX_DELAY))
		{
			printf("Software timers\n");
			count =! count;
		}
		vTaskDelay(pdMS_TO_TICKS(10));
	}
    	BaseType_t result; 
    	result = xTaskCreate(task1,"task1",1024,NULL,5,&xHandle_1); 
    	if(result == pdPASS)
    	{
        	printf("Task1 created\n");
    	}
     	result = xTaskCreate(task2,"task2",1024,NULL,6,&xHandle_2); 
    	if(result == pdPASS)
    	{
        	printf("Task2 created\n");
    	}
     	result = xTaskCreate(task3,"task3",1024,NULL,7,&xHandle_3); 
    	if(result == pdPASS)
    	{
       	printf("Task3 created\n");
    	}
}
