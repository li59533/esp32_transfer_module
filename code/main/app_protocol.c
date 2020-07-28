/**
 **************************************************************************************************
 * @file        app_protocol.c
 * @author
 * @app_protocol
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */

/**
 * @addtogroup    XXX 
 * @{  
 */
#include "app_protocol.h"
#include "version.h"
#include "app_uart.h"
#include "system_param.h"
/**
 * @addtogroup    app_protocol_Modules 
 * @{  
 */

/**
 * @defgroup      app_protocol_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       app_protocol_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_protocol_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       app_protocol_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_protocol_Private_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_protocol_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_protocol_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */
static int8_t app_protocol_check(uint8_t * buf , uint8_t len ); // len: buf len
static uint8_t app_protocol_checksum(uint8_t * buf , uint16_t len); // len: buf len
static void app_protocol_process_cmd(uint8_t *cmd , uint16_t len); // len: cmd ~ payload
static void app_protocol_cmd_version_req(uint8_t *buf , uint16_t len);
static void app_protocol_cmd_getconf_req(uint8_t *buf , uint16_t len);
static void app_protocol_cmd_status_resp(uint8_t cmd , uint8_t status );

/**
 * @}
 */

/**
 * @defgroup      app_protocol_Functions 
 * @brief         
 * @{  
 */

void APP_Protocol_P2P_Process(uint8_t *buf , uint16_t len)
{
    uint8_t * data_buf;
    data_buf = malloc(len);

    app_protocol_check( data_buf , len );

    free(data_buf);
}

uint8_t APP_Protocol_AddTlv(uint8_t * disaddr,APP_Protocol_Tlv_t * tlv_value)
{
	disaddr[0] = tlv_value->Tag;
	disaddr[1] = tlv_value->Len;
	memcpy(&disaddr[2],tlv_value->Value.Array,tlv_value->Len);
	return tlv_value->Len + 2;
}

static uint8_t app_protocol_checksum(uint8_t * buf , uint16_t len)
{
    uint8_t sum  = 0;
    for(uint16_t i = 0 ; i < len; i ++)
    {
        sum += buf[i];
    }
    return sum;
}

static int8_t app_protocol_check(uint8_t * buf , uint8_t len )
{
    APP_Protocol_P2P_t * APP_Protocol_P2P_buf = (APP_Protocol_P2P_t * )buf;
    if(APP_Protocol_P2P_buf->head == APP_PROTOCOL_HEAD)
    {
        printf("protocol pass head\n");
        if(APP_Protocol_P2P_buf->len == (len + 1))
        {
            printf("protocol pass len\n");
            if(buf[len - 2] == APP_PROTOCOL_FOOT)
            {
                printf("protocol pass foot\n");
                if(app_protocol_checksum( buf, len - 1) == buf[len - 1])
                {
                    printf("protocol pass check sum\n");
                    app_protocol_process_cmd( &APP_Protocol_P2P_buf->cmd , len - 7);
                }
                else
                {
                    return -4;
                }
            }
            else
            {
                return -3;
            }
        }
        else
        {
            return -2;
        }
    }
    else
    {
        return -1;
    }
    return 1;
}

void app_protocol_process_cmd(uint8_t *cmd , uint16_t len)
{
    switch( * cmd)
    {
        case CMD_VERSION_REQ :  app_protocol_cmd_version_req( cmd  , len - 1);break;
        case CMD_VERSION_RESP : break;
        case CMD_GET_CONF_REQ : app_protocol_cmd_getconf_req( cmd  , len - 1);break;
        case CMD_GET_CONF_RESP : break;
        case CMD_SET_CONF_REQ : break;
        case CMD_SET_CONF_RESP : break;
        case CMD_INITIATIVE_REPORT :break; 
        case CMD_GET_DATA_REQ :break;
        case CMD_GET_DATA_RESP :break;
        default:break;
    }
}

static void app_protocol_cmd_version_req(uint8_t *buf , uint16_t len)
{
    uint8_t * payload_ptr ; 
    buf = malloc(200);
    APP_Protocol_P2P_t *APP_Protocol_P2P_data = (APP_Protocol_P2P_t *)buf;

    APP_Protocol_P2P_data->head = APP_PROTOCOL_HEAD;
    APP_Protocol_P2P_data->inf = 0x0000;
    APP_Protocol_P2P_data->cmd = CMD_VERSION_RESP;

    payload_ptr = &APP_Protocol_P2P_data->cmd + 1;

    memcpy(payload_ptr , Version_Get_Bin() , 4);

    payload_ptr += 4;

    *payload_ptr = APP_PROTOCOL_FOOT;

    APP_Protocol_P2P_data->len = (payload_ptr - &APP_Protocol_P2P_data->head) + 1;

    payload_ptr += 1;

    *payload_ptr = app_protocol_checksum(&APP_Protocol_P2P_data->head, APP_Protocol_P2P_data->len - 1);

    APP_Uart_EnterSendQueue(&APP_Protocol_P2P_data->head, APP_Protocol_P2P_data->len );

}

static void app_protocol_cmd_getconf_req(uint8_t *buf , uint16_t len)
{
    uint8_t * payload_ptr ; 
    uint8_t * data_space;
    data_space = malloc(200);
    APP_Protocol_P2P_t *APP_Protocol_P2P_data = (APP_Protocol_P2P_t *)data_space;

    APP_Protocol_P2P_data->head = APP_PROTOCOL_HEAD;
    APP_Protocol_P2P_data->inf = 0x0000;
    APP_Protocol_P2P_data->cmd = CMD_GET_CONF_RESP;

    payload_ptr = &APP_Protocol_P2P_data->cmd + 1;

    APP_Protocol_Tlv_t APP_Protocol_Tlv ; 

    // ------- workingmode ------------
    APP_Protocol_Tlv.Tag = TAG_CONF_WORKINGMODE ;
    APP_Protocol_Tlv.Len = 1;
    APP_Protocol_Tlv.Value.BIT_8 = g_SystemParam_Config.workingmode;
    payload_ptr += APP_Protocol_AddTlv(payload_ptr , &APP_Protocol_Tlv);
    // --------------------------------
    // ------- ssid -------------------
    APP_Protocol_Tlv.Tag = TAG_CONF_SSID ;
    APP_Protocol_Tlv.Len = strlen(g_SystemParam_Config.ssid);
    memcpy(APP_Protocol_Tlv.Value.Array ,g_SystemParam_Config.ssid , APP_Protocol_Tlv.Len );
    payload_ptr += APP_Protocol_AddTlv(payload_ptr , &APP_Protocol_Tlv);
    // ------- password ---------------
    APP_Protocol_Tlv.Tag = TAG_CONF_PASSWORD ;
    APP_Protocol_Tlv.Len = strlen(g_SystemParam_Config.password);
    memcpy(APP_Protocol_Tlv.Value.Array ,g_SystemParam_Config.password , APP_Protocol_Tlv.Len);
    payload_ptr += APP_Protocol_AddTlv(payload_ptr , &APP_Protocol_Tlv);
    // --------------------------------
    // ------- dhcp flag --------------
    APP_Protocol_Tlv.Tag = TAG_CONF_DHCP_FLAG ;
    APP_Protocol_Tlv.Len = 1;
    APP_Protocol_Tlv.Value.BIT_8 = g_SystemParam_Config.dhcp_flag;
    payload_ptr += APP_Protocol_AddTlv(payload_ptr , &APP_Protocol_Tlv);
    // --------------------------------
    // ------- DNS --------------------
    APP_Protocol_Tlv.Tag = TAG_CONF_DNS;
    APP_Protocol_Tlv.Len = 4;
    APP_Protocol_Tlv.Value.BIT_32 = g_SystemParam_Config.DNS;
    payload_ptr += APP_Protocol_AddTlv(payload_ptr , &APP_Protocol_Tlv);
    // --------------------------------
    // ------- gateway ----------------
    APP_Protocol_Tlv.Tag = TAG_CONF_GATEWAY ; 
    APP_Protocol_Tlv.Len = 4;
    APP_Protocol_Tlv.Value.BIT_32 = g_SystemParam_Config.gateway;
    payload_ptr += APP_Protocol_AddTlv(payload_ptr , &APP_Protocol_Tlv);   
    // --------------------------------
    // ------- domainname flag --------
    APP_Protocol_Tlv.Tag = TAG_CONF_DOMAINNAME_FLAG ;
    APP_Protocol_Tlv.Len = 1;
    APP_Protocol_Tlv.Value.BIT_8 = g_SystemParam_Config.domainname_flag;
    payload_ptr += APP_Protocol_AddTlv(payload_ptr , &APP_Protocol_Tlv);    
    // --------------------------------
    // ------- domainname -------------
    APP_Protocol_Tlv.Tag = TAG_CONF_DOMAINNAME ;
    APP_Protocol_Tlv.Len = strlen(g_SystemParam_Config.domain_name);
    memcpy(APP_Protocol_Tlv.Value.Array ,g_SystemParam_Config.domain_name , APP_Protocol_Tlv.Len);
    payload_ptr += APP_Protocol_AddTlv(payload_ptr , &APP_Protocol_Tlv);
    // --------------------------------
    // ------- target ip --------------
    APP_Protocol_Tlv.Tag = TAG_CONF_TARGET_IP ; 
    APP_Protocol_Tlv.Len = 4;
    APP_Protocol_Tlv.Value.BIT_32 = g_SystemParam_Config.target_ip;
    payload_ptr += APP_Protocol_AddTlv(payload_ptr , &APP_Protocol_Tlv);   
    // --------------------------------
    // ------- target port ------------
    APP_Protocol_Tlv.Tag = TAG_CONF_TARGET_PORT ; 
    APP_Protocol_Tlv.Len = 2;
    APP_Protocol_Tlv.Value.BIT_16 = g_SystemParam_Config.target_port;
    payload_ptr += APP_Protocol_AddTlv(payload_ptr , &APP_Protocol_Tlv);  
    // --------------------------------
    // ------- local ip ---------------
    APP_Protocol_Tlv.Tag = TAG_CONF_LOCAL_IP ; 
    APP_Protocol_Tlv.Len = 4;
    APP_Protocol_Tlv.Value.BIT_32 = g_SystemParam_Config.local_ip;
    payload_ptr += APP_Protocol_AddTlv(payload_ptr , &APP_Protocol_Tlv);  
    // --------------------------------
    // ------- local port -------------
    APP_Protocol_Tlv.Tag = TAG_CONF_LOCAL_PORT ; 
    APP_Protocol_Tlv.Len = 2;
    APP_Protocol_Tlv.Value.BIT_16 = g_SystemParam_Config.local_port;
    payload_ptr += APP_Protocol_AddTlv(payload_ptr , &APP_Protocol_Tlv);  
    // --------------------------------
    // ------- local conf port --------
    APP_Protocol_Tlv.Tag = TAG_CONF_LOCAL_CONF_PORT ; 
    APP_Protocol_Tlv.Len = 2;
    APP_Protocol_Tlv.Value.BIT_16 = g_SystemParam_Config.local_port;
    payload_ptr += APP_Protocol_AddTlv(payload_ptr , &APP_Protocol_Tlv);  
    // --------------------------------
    *payload_ptr = APP_PROTOCOL_FOOT;

    APP_Protocol_P2P_data->len = (payload_ptr - &APP_Protocol_P2P_data->head) + 1;

    payload_ptr += 1;

    *payload_ptr = app_protocol_checksum(&APP_Protocol_P2P_data->head, APP_Protocol_P2P_data->len - 1);

    APP_Uart_EnterSendQueue(&APP_Protocol_P2P_data->head, APP_Protocol_P2P_data->len );
}

static void app_protocol_cmd_setconf_req(uint8_t *buf , uint16_t len)
{
	uint8_t * payload_ptr = 0;
	APP_Protocol_Tlv_t * tlv_buf = 0;
	uint8_t result = CONF_SUCCEED;
	
	
	payload_ptr = buf;
	printf("APP_Rev Payload LEN is %d\r\n",len);
	while(1)
	{
		tlv_buf = (APP_Protocol_Tlv_t *) payload_ptr;
		printf("APP_Rev Tag is %X\r\n",tlv_buf->Tag);
		switch(tlv_buf->Tag)
		{
            case TAG_CONF_WORKINGMODE:
                {
                    if(tlv_buf->Len == 1)
                    {
                        g_SystemParam_Config.workingmode = tlv_buf->Value.BIT_8;
                    }
                    else
                    {
                        result = CONF_FAIL;
                    }
                }
                break;
            case TAG_CONF_SSID:
                {
                    if(tlv_buf->Len < SYS_SSID_MAX )
                    {
                        memset(g_SystemParam_Config.ssid , 0 , sizeof(g_SystemParam_Config.ssid));
                        memcpy(g_SystemParam_Config.ssid , tlv_buf->Value.Array , tlv_buf->Len);
                    }
                    else
                    {
                        result = CONF_FAIL;
                    }
                }
                break;
            case TAG_CONF_PASSWORD:
                {
                    if(tlv_buf->Len < SYS_PASSWORD_MAX )
                    {
                        memset(g_SystemParam_Config.password , 0 , sizeof(g_SystemParam_Config.password));
                        memcpy(g_SystemParam_Config.password , tlv_buf->Value.Array , tlv_buf->Len);                        
                    }
                    else
                    {
                        result = CONF_FAIL;
                    }                    
                }
                break;
            case TAG_CONF_DHCP_FLAG:
                {
                    if(tlv_buf->Len == 1 )
                    {
                        g_SystemParam_Config.dhcp_flag = tlv_buf->Value.BIT_8;                     
                    }
                    else
                    {
                        result = CONF_FAIL;
                    }                         
                }
                break;
            case TAG_CONF_DNS:
                {
                    if(tlv_buf->Len == 4 )
                    {
                        g_SystemParam_Config.DNS = tlv_buf->Value.BIT_32;                     
                    }
                    else
                    {
                        result = CONF_FAIL;
                    }  
                }
                break;
            case TAG_CONF_GATEWAY:
                {
                    if(tlv_buf->Len == 4 )
                    {
                        g_SystemParam_Config.gateway = tlv_buf->Value.BIT_32;                     
                    }
                    else
                    {
                        result = CONF_FAIL;
                    }                      
                }
                break;
            case TAG_CONF_DOMAINNAME_FLAG:
                {
                    if(tlv_buf->Len == 1 )
                    {
                        g_SystemParam_Config.domainname_flag = tlv_buf->Value.BIT_8;                     
                    }
                    else
                    {
                        result = CONF_FAIL;
                    }                       
                }
                break;
            case TAG_CONF_DOMAINNAME:
                {
                    if(tlv_buf->Len < SYS_DOMAIN_NAME_MAX)
                    {
                        memset(g_SystemParam_Config.domain_name , 0 , sizeof(g_SystemParam_Config.domain_name));
                        memcpy(g_SystemParam_Config.domain_name , tlv_buf->Value.Array , tlv_buf->Len);                      
                    }
                    else
                    {
                        result = CONF_FAIL;
                    }                          
                }
                break;
            case TAG_CONF_TARGET_IP:
                {
                    if(tlv_buf->Len == 4 )
                    {
                        g_SystemParam_Config.target_ip = tlv_buf->Value.BIT_32;                     
                    }
                    else
                    {
                        result = CONF_FAIL;
                    }                         
                }
                break;
            case TAG_CONF_TARGET_PORT:
                {
                    if(tlv_buf->Len == 2 )
                    {
                        g_SystemParam_Config.target_port = tlv_buf->Value.BIT_16;                     
                    }
                    else
                    {
                        result = CONF_FAIL;
                    }                       
                }
                break;
            case TAG_CONF_LOCAL_IP:
                {
                    if(tlv_buf->Len == 4 )
                    {
                        g_SystemParam_Config.local_ip = tlv_buf->Value.BIT_32;                     
                    }
                    else
                    {
                        result = CONF_FAIL;
                    }                     
                }
                break;
            case TAG_CONF_LOCAL_PORT:
                {
                    if(tlv_buf->Len == 2 )
                    {
                        g_SystemParam_Config.local_port = tlv_buf->Value.BIT_16;                     
                    }
                    else
                    {
                        result = CONF_FAIL;
                    }                       
                }
                break;
            case TAG_CONF_LOCAL_CONF_PORT:
                {
                    if(tlv_buf->Len == 2 )
                    {
                        g_SystemParam_Config.local_conf_port = tlv_buf->Value.BIT_16;                     
                    }
                    else
                    {
                        result = CONF_FAIL;
                    }                      
                }
                break;
            default:break;
        }

		payload_ptr = payload_ptr + tlv_buf->Len + 2 ;
		
		if((payload_ptr - buf)>= (len - 2))
		{
			break;
		}        
    }

    if(result == CONF_SUCCEED)
    {
        SystemParam_Save();
    }
    else
    {
        SystemParam_Read();

    }

    app_protocol_cmd_status_resp(CMD_SET_CONF_RESP ,result );
}

static void app_protocol_cmd_status_resp(uint8_t cmd , uint8_t status )
{
    uint8_t * payload_ptr ; 
    uint8_t * data_space;
    data_space = malloc(200);
    APP_Protocol_P2P_t *APP_Protocol_P2P_data = (APP_Protocol_P2P_t *)data_space;

    APP_Protocol_P2P_data->head = APP_PROTOCOL_HEAD;
    APP_Protocol_P2P_data->inf = 0x0000;
    APP_Protocol_P2P_data->cmd = cmd;

    payload_ptr = &APP_Protocol_P2P_data->cmd + 1;

    // ------- status ------------
    * payload_ptr = status;
    payload_ptr += 1;
    // --------------------------------

    // --------------------------------
    *payload_ptr = APP_PROTOCOL_FOOT;

    APP_Protocol_P2P_data->len = (payload_ptr - &APP_Protocol_P2P_data->head) + 1;

    payload_ptr += 1;

    *payload_ptr = app_protocol_checksum(&APP_Protocol_P2P_data->head, APP_Protocol_P2P_data->len - 1);

    APP_Uart_EnterSendQueue(&APP_Protocol_P2P_data->head, APP_Protocol_P2P_data->len );    
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

