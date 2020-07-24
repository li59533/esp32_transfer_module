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
        case CMD_GET_CONF_REQ : break;
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


/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

