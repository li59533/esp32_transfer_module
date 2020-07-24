/**
 **************************************************************************************************
 * @file        app_uart.h
 * @author
 * @version    v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _APP_UART_H_
#define _APP_UART_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
/**
 * @addtogroup    app_uart_Modules 
 * @{  
 */

/**
 * @defgroup      app_uart_Exported_Macros 
 * @{  
*/

/**
 * @}
 */

/**
 * @defgroup      app_uart_Exported_Constants
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      app_uart_Exported_Types 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_uart_Exported_Variables 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_uart_Exported_Functions 
 * @{  
 */
void APP_Uart_Init(void);
void APP_Uart_Send_Task(void);
void APP_Uart_EnterSendQueue(uint8_t *buf , uint16_t len );
void APP_Uart_RevProcess(void);
// -------------------Test Code --------------------------

void APP_Uart_TestCode(void);
void APP_Uart_TestCode2(void);

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
