/**
 **************************************************************************************************
 * @file        net_task.h
 * @author
 * @version    v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _NET_TASK_H_
#define _NET_TASK_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
#include "freertos/task.h"
/**
 * @addtogroup    net_task_Modules 
 * @{  
 */

/**
 * @defgroup      net_task_Exported_Macros 
 * @{  
 */
#define NET_TASK_TEST_EVENT 			(0X01 << 0)
#define NET_TASK_TEST2_EVENT			(0X01 << 1)

#define NET_TASK_INIT_EVENT             (0X01 << 2)
#define NET_TASK_TCP_EVENT              (0X01 << 3)
#define NET_TASK_UDP_EVENT              (0x01 << 4)
#define NET_TASK_UDP_SEND_EVENT         (0X01 << 5)

/**
 * @}
 */

/**
 * @defgroup      net_task_Exported_Constants
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      net_task_Exported_Types 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      net_task_Exported_Variables 
 * @{  
 */
extern TaskHandle_t  Net_Task_Handle ;
/**
 * @}
 */

/**
 * @defgroup      net_task_Exported_Functions 
 * @{  
 */
uint32_t Net_Task_Init(void); 
void Net_Task(void * pvParameter);
void Net_Task_Event_Start(uint32_t events, uint8_t event_from);
void Net_Task_Tim_Init(void);
void Net_Task_StartTim(uint16_t time_count);

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


