/**
 **************************************************************************************************
 * @file        net_task.c
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
#include "net_task.h"
#include "rtos_tools.h"
/**
 * @addtogroup    XXX 
 * @{  
 */
#include "app_net.h"
#include "app_web.h"
/**
 * @addtogroup    net_task_Modules 
 * @{  
 */

/**
 * @defgroup      net_task_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      net_task_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      net_task_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      net_task_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      net_task_Private_Variables 
 * @brief         
 * @{  
 */
TimerHandle_t net_task_tim;
TaskHandle_t  Net_Task_Handle = NULL;

/**
 * @}
 */
 
/**
 * @defgroup      net_task_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      net_task_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

static void net_task_tim_callback(TimerHandle_t xTimer);
static void UDPRev_task();
/**
 * @}
 */

/**
 * @defgroup      net_task_Functions 
 * @brief         
 * @{  
 */

uint32_t Net_Task_Init(void)
{
	BaseType_t basetype = { 0 };

        basetype = xTaskCreatePinnedToCore(Net_Task,\
							"Net Task",\
							8192,
							NULL,
							 configMAX_PRIORITIES -1 ,
							&Net_Task_Handle,
                            tskNO_AFFINITY);

        basetype |= xTaskCreatePinnedToCore(UDPRev_task,\
                    "UDPRev_task",\
                    8192,
                    NULL,
                    configMAX_PRIORITIES ,
                    NULL,
                    tskNO_AFFINITY);      
    /*
	basetype = xTaskCreate(Net_Task,\
							"Net Task",\
							128,
							NULL,
							3,
							&Net_Task_Handle);
    */          
    DEBUG("Net Task Init\r\n");             
	return basetype;
}

static void UDPRev_task()
{
    APP_Net_UDP_RevProcess();
}

void Net_Task(void * pvParameter)
{
	uint32_t event_flag = 0;
	
	DEBUG("Net Task Enter\r\n");
	UBaseType_t nettask_ramainheap = 0;

    APP_Net_Init();


    //Net_Task_Event_Start(NET_TASK_TEST2_EVENT,EVENT_FROM_TASK);
    Net_Task_Event_Start(NET_TASK_CHANGE_EVENT,EVENT_FROM_TASK);
	while(1)
	{
		xTaskNotifyWait(0x00,ULONG_MAX,&event_flag , portMAX_DELAY);
		
		if((event_flag & NET_TASK_TEST_EVENT) != 0x00)
		{
			DEBUG("Net Task Looping\r\n");
			nettask_ramainheap = uxTaskGetStackHighWaterMark(NULL);
			DEBUG("Net Task ramain heap:%d %%\r\n",nettask_ramainheap);
	
		}

		if((event_flag & NET_TASK_CHANGE_EVENT) != 0x00)
		{
            
            APP_Net_ChangeMode();
			DEBUG("Net Task INIT EVENT\r\n");

		}				
        
        if((event_flag & NET_TASK_TCP_EVENT) != 0x00)
		{
			DEBUG("Net Task TCP EVENT\r\n");
            APP_Net_TCPProcess();
		}			
        if((event_flag & NET_TASK_UDP_EVENT) != 0x00)
		{
			DEBUG("Net Task UDP EVENT\r\n");
            APP_Net_UDPProcess();
		}			
        if((event_flag & NET_TASK_UDP_SEND_EVENT) != 0x00)
		{
            DEBUG("Net Task UDP SEND EVENT\r\n");
            APP_Net_UDPSend_Process();
            
		}		     
        if((event_flag & NET_TASK_WEB_EVENT) != 0x00)
		{
            DEBUG("Net Task WEB EVENT\r\n");
            APP_Web_StartServer();

		}		 
           
	}
}


void Net_Task_Event_Start(uint32_t events, uint8_t event_from)
{
	switch(event_from)
	{
		case EVENT_FROM_TASK:
		{
			xTaskNotify(Net_Task_Handle , events , eSetBits);
		}
		break;
		case EVENT_FROM_ISR:
		{
			xTaskNotifyFromISR(Net_Task_Handle, events, eSetBits , NULL);
		}
		break;
		default:break;
	}
}





void Net_Task_Tim_Init(void)
{
	net_task_tim = xTimerCreate(	"NetTimOUT",			/*lint !e971 Unqualified char types are allowed for strings and single characters only. */
									pdMS_TO_TICKS(1000),
									pdTRUE,
									NULL,
									net_task_tim_callback );
}

void Net_Task_StartTim(uint16_t time_count)
{
	xTimerChangePeriod( net_task_tim,  pdMS_TO_TICKS(time_count) , 0 );
	xTimerStart( net_task_tim,0);
}
static void net_task_tim_callback(TimerHandle_t xTimer)
{
	//Net_Task_Event_Start(Net_TASK_SEND_AT_EVENT, EVENT_FROM_TASK);
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

