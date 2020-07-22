/**
 **************************************************************************************************
 * @file        uart_task.h
 * @author
 * @version    v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _UART_TASK_H_
#define _UART_TASK_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
#include "freertos/task.h"
/**
 * @addtogroup    uart_task_Modules 
 * @{  
 */

/**
 * @defgroup      uart_task_Exported_Macros 
 * @{  
 */
#define UART_TASK_TEST_EVENT 			(0X01 << 0)
#define UART_TASK_TEST2_EVENT			(0X01 << 1)
#define UART_TASK_INIT_EVENT            (0X01 << 2)


/**
 * @}
 */

/**
 * @defgroup      uart_task_Exported_Constants
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      uart_task_Exported_Types 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      uart_task_Exported_Variables 
 * @{  
 */
extern TaskHandle_t  Uart_Task_Handle ;
/**
 * @}
 */

/**
 * @defgroup      uart_task_Exported_Functions 
 * @{  
 */
uint32_t Uart_Task_Init(void); 
void Uart_Task(void * pvParameter);
void Uart_Task_Event_Start(uint32_t events, uint8_t event_from);
void Uart_Task_Tim_Init(void);
void Uart_Task_StartTim(uint16_t time_count);

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
#endif


