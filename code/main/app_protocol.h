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
	uint8_t Tag;
	uint8_t Len;
	union 
	{
		uint8_t BIT_8;
		uint16_t BIT_16;
		uint32_t BIT_32;
		uint8_t Array[20];
	}Value;
}APP_Protocol_Tlv_t;

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


typedef enum
{
    TAG_CONF_WORKINGMODE    = 0X00,
    TAG_CONF_SSID           = 0X01,
    TAG_CONF_PASSWORD       = 0X02,
    TAG_CONF_DHCP_FLAG      = 0X03,
    TAG_CONF_DNS            = 0X04,
    TAG_CONF_GATEWAY        = 0X05,
    TAG_CONF_DOMAINNAME_FLAG= 0X06,
    TAG_CONF_DOMAINNAME     = 0X07,
    TAG_CONF_TARGET_IP      = 0X08,
    TAG_CONF_TARGET_PORT    = 0X09,
    TAG_CONF_LOCAL_IP       = 0X0A,
    TAG_CONF_LOCAL_PORT     = 0X0B,
    TAG_CONF_LOCAL_CONF_PORT= 0X0C,
}APP_Protocol_ConfTag_e;


typedef enum
{
    CONF_SUCCEED = 0x00,
    CONF_FAIL = 0X01,
}APP_Protocol_Confstatus_e;


typedef enum
{
    WorkingMode_TCP = 1,
    WorkingMode_UDP ,
    WorkingMode_AP ,
}APP_Protocol_WorkingMode_e;

typedef enum
{
    DHCP_F_OPEN = 1,
    DHCP_F_CLOSE ,
}APP_Protocol_DHCP_Flag_e;

typedef enum
{
    DOMAIN_F_OPEN = 1,
    DOMAIN_F_CLOSE ,
}APP_Protocol_Domain_Flag_e;
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
uint8_t APP_Protocol_AddTlv(uint8_t * disaddr,APP_Protocol_Tlv_t * tlv_value);
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
