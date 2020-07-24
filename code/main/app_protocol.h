/**
 **************************************************************************************************
 * @file        app_protocol.h
 * @author
 * @app_protocol
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _APP_PROTOCOL_H_
#define _APP_PROTOCOL_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
/**
 * @addtogroup    app_protocol_Modules 
 * @{  
 */

/**
 * @defgroup      app_protocol_Exported_Macros 
 * @{  
 */
#define APP_PROTOCOL_HEAD       0x4c // -> 'L'
#define APP_PROTOCOL_FOOT       0x4e // -> 'N'
/**
 * @}
 */

/**
 * @defgroup      app_protocol_Exported_Constants
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_protocol_Exported_Types 
 * @{  
 */

#pragma pack (1)
typedef struct
{
    uint8_t head;
    uint16_t len;
    uint16_t inf;
    uint8_t cmd;
    uint8_t * payload;
    uint8_t foot;
    uint8_t sum;
}APP_Protocol_P2P_t;

#pragma pack ()

typedef enum
{
    CMD_VERSION_REQ = 0,
    CMD_VERSION_RESP = 1,
    CMD_GET_CONF_REQ = 2,
    CMD_GET_CONF_RESP = 3,
    CMD_SET_CONF_REQ = 4,
    CMD_SET_CONF_RESP = 5,

    CMD_INITIATIVE_REPORT = 30 , 
    CMD_GET_DATA_REQ = 31 ,
    CMD_GET_DATA_RESP = 32 ,

}APP_Protocol_CMD_e;

/**
 * @}
 */

/**
 * @defgroup      app_protocol_Exported_Variables  
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_protocol_Exported_Functions 
 * @{  
 */
void APP_Protocol_P2P_Process(uint8_t *buf , uint16_t len);
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
