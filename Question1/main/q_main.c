/**1.Create 3 realtime tasks each with the periodicity T1=1000ms ,T2=2000ms and T3=5000ms.Also create two additional tasks T4 and T5 where T4 send integer data to T5 using Message Queues**/

#include<stdio.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include<freertos/task.h>
#include<freertos/queue.h>

xQueueHandle xQueue;
TaskHandle_t xHandle_1;
TaskHandle_t xHandle_2;
TaskHandle_t xHandle_3;

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

static void vsenderTask(void *pv)
{
	long lvaluetosend;
	portBASE_TYPE xstatus;
	lvaluetosend=(long)pv;
	for(int i=0;i<5;i++)
	{
		xstatus=xQueueSendToBack(xQueue,&lvaluetosend,0);
		if(xstatus!=pdPASS)
		{
			vPrintString("Could not send data\n");
		}
	taskYIELD();
	}
}
static void vreceiveTask(void *pv)
{
	long lrecievevalue;
	portBASE_TYPE xstatus;
	const portTickTYpe xTickToWait = 100/portTICK_RATE_MS;
	for(int i=0;i<5;i++)
	{
		if(uxQueueMessageWaiting(xQueue)!=0)
		{
			vPrintString("Queue should have been empty\n");	
		}
		xstatus=xQueueReceive(xQueue,&lrecievevalue,xTickToWait);
		if(xstatus == pdPASS)
		{
			vPrintString("Received= \n",lrecievevalue);
		}
		else
		{
			vPrintString("Queue not recieve data from queue\n");
		}
	}
}
void app_main()
{
	
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
    	xQueue=xQueueCreate(5,sizeof(long));
	if(xQueue!=NULL)
	{
		xTaskCreate(vsenderTask,"Sender1",1024,(void *)100,8,NULL); 
		xTaskCreate(vsenderTask,"Sender2",1024,(void *)200,9,NULL); 
		
		xTaskCreate(vreceiveTask,"Receiver",1024,NULL,12,NULL);
		vTaskStartScheduler();
	}
	else
	{
		printf("Queue could not be created\n");
	} 
	for(;;)	
}
