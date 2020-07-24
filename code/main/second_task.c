/**
 **************************************************************************************************
 * @file        second_task.c
 * @author
 * @version   v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#include "second_task.h"
#include "clog.h"



/**
 * @addtogroup    XXX 
 * @{  
 */

#include "app_net.h"
#include "app_uart.h"

/**
 * @addtogroup    second_task_Modules 
 * @{  
 */

/**
 * @defgroup      second_task_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      second_task_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      second_task_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      second_task_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      second_task_Private_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      second_task_Public_Variables 
 * @brief         
 * @{  
 */
TaskHandle_t  Second_Task_Handle = NULL;
/**
 * @}
 */

/**
 * @defgroup      second_task_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      second_task_Functions 
 * @brief         
 * @{  
 */


uint32_t Second_Task_Init(void)
{
	BaseType_t basetype = { 0 };

    basetype = xTaskCreatePinnedToCore(Second_Task,\
							"Second_Task",\
							2048,
							NULL,
							 3 ,
							&Second_Task_Handle,
                            tskNO_AFFINITY);
/*
	basetype = xTaskCreate(Second_Task,\
							"Second_Task",\
							256,
							NULL,
							1,
							&tskNO_AFFINITY);
*/
	return basetype;
}

void Second_Task(void * pvParameter)
{
	//UBaseType_t ramainheap = 0;
	DEBUG("Second Task Enter\r\n");
	while(1)
	{
		DEBUG("Second Task Looping\r\n");
		vTaskDelay(pdMS_TO_TICKS(1000));
        //APP_Net_UDP_TestCode();
        //APP_Uart_TestCode();
	}
	
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

