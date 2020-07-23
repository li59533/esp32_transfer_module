/**
 **************************************************************************************************
 * @file        system_param.h
 * @author
 * @version
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _SYSTEM_PARAM_H_
#define _SYSTEM_PARAM_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
/**
 * @addtogroup    system_param_Modules 
 * @{  
 */

/**
 * @defgroup      system_param_Exported_Macros 
 * @{  
 */
#define SYS_UDP_CONF_PORT       62536		
#define SYS_SSID_MAX            20
#define SYS_PASSWORD_MAX        20
#define SYS_DOMAIN_NAME_MAX     30						
/**
 * @}
 */

/**
 * @defgroup      system_param_Exported_Constants
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      system_param_Exported_Types 
 * @{  
 */



typedef struct
{

    uint8_t workingmode ;
    char ssid[SYS_SSID_MAX];
    char password[SYS_PASSWORD_MAX];
    uint8_t dhcp_flag;
    uint32_t DNS;
    uint32_t gateway;
    uint8_t domainname_flag;
    char domain_name[SYS_DOMAIN_NAME_MAX];
    uint32_t target_ip;
    uint16_t target_port;
    uint32_t local_ip;
    uint16_t local_port;
    uint16_t local_conf_port;

}SystemParam_Config_t;

/**
 * @}
 */

/**
 * @defgroup      system_param_Exported_Variables 
 * @{  
 */
extern SystemParam_Config_t g_SystemParam_Config;
/**
 * @}
 */

/**
 * @defgroup      system_param_Exported_Functions 
 * @{  
 */
void SystemParam_Init(void);

int16_t SystemParam_Read(void);

void SystemParam_Save(void);

void SystemParam_Rest(void);

void SystemParam_Apply(uint8_t handle);
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
