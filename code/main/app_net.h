/**
 **************************************************************************************************
 * @file        app_net.h
 * @author
 * @version    v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _APP_NET_H_
#define _APP_NET_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
#include "app_protocol.h"
#include "system_param.h"
/**
 * @addtogroup    app_net_Modules 
 * @{  
 */

/**
 * @defgroup      app_net_Exported_Macros 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_net_Exported_Constants
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      app_net_Exported_Types 
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
    uint32_t netmask;
}APP_Net_Param_t;
/**
 * @}
 */

/**
 * @defgroup      app_net_Exported_Variables 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_net_Exported_Functions 
 * @{  
 */
void APP_Net_Init(void);
void APP_NET_STA(void);
void APP_Net_TCPProcess(void);
void APP_Net_UDPProcess(void);
void APP_Net_UDPSend_Process(void);

int APP_Net_UDPSendBytes(int socketNUM , uint8_t * buf , uint16_t len ,uint32_t dest_ip , uint32_t port);
int APP_Net_UDPRevBytes(int socketNUM , uint8_t * buf , uint16_t len ,uint32_t *dest_ip , uint32_t *port);
void APP_Net_UDP_RevProcess(void);
// ----------Queue--------------
void APP_Net_UDP_SendQueue_Init(void);
void APP_Net_UDP_SendEnqueue(uint8_t udp_queue_num , uint8_t * buf , uint16_t len);
uint16_t APP_Net_UDP_SendQueue_Count(uint8_t udp_queue_num);
int8_t  APP_Net_UDP_SendDequeue(uint8_t udp_queue_num , uint8_t * buf ,uint16_t * len);
// -----------------------------


// ----------Test Code ---------
void APP_Net_UDP_TestCode(void);
// -----------------------------
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
