/**
 **************************************************************************************************
 * @file        uart_task.c
 * @author
 * @version   v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */

#include "limits.h"
#include "clog.h"
#include "freertos/FreeRTOS.h"

#include "freertos/timers.h"
#include "uart_task.h"
#include "rtos_tools.h"
/**
 * @addtogroup    XXX 
 * @{  
 */
#include "app_uart.h"

/**
 * @addtogroup    uart_task_Modules 
 * @{  
 */

/**
 * @defgroup      uart_task_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      uart_task_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      uart_task_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      uart_task_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      uart_task_Private_Variables 
 * @brief         
 * @{  
 */
TimerHandle_t uart_task_tim;
TaskHandle_t  Uart_Task_Handle = NULL;

/**
 * @}
 */
 
/**
 * @defgroup      uart_task_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      uart_task_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

static void uart_task_tim_callback(TimerHandle_t xTimer);

static void rx_task();
/**
 * @}
 */

/**
 * @defgroup      uart_task_Functions 
 * @brief         
 * @{  
 */

uint32_t Uart_Task_Init(void)
{
	BaseType_t basetype = { 0 };

    basetype |= xTaskCreatePinnedToCore(Uart_Task,\
							"Uart Task",\
							4096,
							NULL,
							 2 ,
							&Uart_Task_Handle,
                            tskNO_AFFINITY);

    basetype |= xTaskCreatePinnedToCore(rx_task,\
                        "rx_task",\
                        8192,
                        NULL,
                        configMAX_PRIORITIES ,
                        NULL,
                        tskNO_AFFINITY);                        
    /*
	basetype = xTaskCreate(Uart_Task,\
							"Uart Task",\
							128,
							NULL,
							3,
							&Uart_Task_Handle);
    */          
    DEBUG("Uart Task Init\r\n");             
	return basetype;
}


static void rx_task()
{
    APP_Uart_RevProcess();
}

void Uart_Task(void * pvParameter)
{
	uint32_t event_flag = 0;
	
	DEBUG("Uart Task Enter\r\n");
	UBaseType_t uarttask_ramainheap = 0;

    Uart_Task_Event_Start(UART_TASK_INIT_EVENT , EVENT_FROM_TASK);

	while(1)
	{
		xTaskNotifyWait(0x00,ULONG_MAX,&event_flag , portMAX_DELAY);
		
		if((event_flag & UART_TASK_TEST_EVENT) != 0x00)
		{
			DEBUG("Uart Task Looping\r\n");
			uarttask_ramainheap = uxTaskGetStackHighWaterMark(NULL);
			DEBUG("Uart Task ramain heap:%d %%\r\n",uarttask_ramainheap);
	
		}
		if((event_flag & UART_TASK_TEST2_EVENT) != 0x00)
		{
			DEBUG("Uart Task UART_TASK_TEST2_EVENT\r\n");
            APP_Uart_TestCode();
        }		
        
		if((event_flag & UART_TASK_INIT_EVENT) != 0x00)
		{
			DEBUG("Uart Task INIT EVENT\r\n");            
            APP_Uart_Init();
            Uart_Task_Event_Start(UART_TASK_TEST2_EVENT , EVENT_FROM_TASK);
		}				
		
	}
	
}


void Uart_Task_Event_Start(uint32_t events, uint8_t event_from)
{
	switch(event_from)
	{
		case EVENT_FROM_TASK:
		{
			xTaskNotify(Uart_Task_Handle , events , eSetBits);
		}
		break;
		case EVENT_FROM_ISR:
		{
			xTaskNotifyFromISR(Uart_Task_Handle, events, eSetBits , NULL);
		}
		break;
		default:break;
	}
}




void Uart_Task_Tim_Init(void)
{
	uart_task_tim = xTimerCreate(	"UartTimOUT",			/*lint !e971 Unqualified char types are allowed for strings and single characters only. */
									pdMS_TO_TICKS(1000),
									pdTRUE,
									NULL,
									uart_task_tim_callback );
}

void Uart_Task_StartTim(uint16_t time_count)
{
	xTimerChangePeriod( uart_task_tim,  pdMS_TO_TICKS(time_count) , 0 );
	xTimerStart( uart_task_tim,0);
}
static void uart_task_tim_callback(TimerHandle_t xTimer)
{
	//Uart_Task_Event_Start(Uart_TASK_SEND_AT_EVENT, EVENT_FROM_TASK);
}



/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

