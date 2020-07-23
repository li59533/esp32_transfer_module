/**
 **************************************************************************************************
 * @file        app_net.c
 * @author
 * @version   v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#include "app_net.h"

/**
 * @addtogroup    XXX 
 * @{  
 */
#include "string.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "system_param.h"
#include "net_task.h"
#include "clog.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#include "net_task.h"
#include "rtos_tools.h"

/**
 * @addtogroup    app_net_Modules 
 * @{  
 */

/**
 * @defgroup      app_net_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_net_Macros_Defines 
 * @brief         
 * @{  
 */
#define SOCKET_SEND      0
#define SOCKET_CONF      1
#define SOCKET_NUM_COUNT 2


#define SOCKET_CONF_PORT  26601
/**
 * @}
 */
 
/**
 * @defgroup      app_net_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_net_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      app_net_Private_Variables 
 * @brief         
 * @{  
 */
static const char *TAG = "wifi station";

int sock[SOCKET_NUM_COUNT] = { 0 };

/**
 * @}
 */
 
/**
 * @defgroup      app_net_Public_Variables 
 * @brief         
 * @{  
 */
#define APP_NET_UDPSEND_QUEUE_NUM       1
#define APP_NET_UDPSEND_SPACE_LEN     4096
static uint8_t app_net_udpsend_space[APP_NET_UDPSEND_SPACE_LEN] = { 0 };

typedef struct 
{
    uint8_t *buf;
    uint16_t size ;
    uint16_t in;
    uint16_t out;
    uint16_t count;
}APP_Net_UDPSendQueue_t;


APP_Net_UDPSendQueue_t APP_Net_UDPSendQueue[APP_NET_UDPSEND_QUEUE_NUM] = 
{
    {
        .in = 0,
        .out = 0,
        .count = 0,        
    }

};
/**
 * @}
 */

/**
 * @defgroup      app_net_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */
static esp_err_t event_handler(void *ctx, system_event_t *event);
/**
 * @}
 */

/**
 * @defgroup      app_net_Functions 
 * @brief         
 * @{  
 */

typedef struct 
{
    uint8_t Cur_Mode;

}APP_Net_Param_t;

typedef enum
{
    Net_Mode_UDP,
}APP_Net_Mode_e;

APP_Net_Param_t APP_Net_Param = 
{
    .Cur_Mode = Net_Mode_UDP
};

void APP_Net_Init(void)
{
    APP_Net_Param.Cur_Mode = g_SystemParam_Config.workingmode;

    APP_Net_UDP_SendQueue_Init();
    Net_Task_Event_Start(NET_TASK_STA_EVENT,EVENT_FROM_TASK);
}

void APP_NET_STA(void)
{

    tcpip_adapter_init(); // TCP/IP Init
    //ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t wifi_config = {
        .sta= {
            .ssid = "Bigym",
            .password = "1234567890",
        },
    };  

// ------------------Config IP conf------------------------

    ESP_ERROR_CHECK(tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_STA));
    tcpip_adapter_dhcp_status_t dhcp_status;
    ESP_ERROR_CHECK( tcpip_adapter_dhcpc_get_status(TCPIP_ADAPTER_IF_STA, &dhcp_status));
    ESP_LOGI(TAG, "ip_info Dhcp c:      %d" , (uint8_t) dhcp_status); 

    tcpip_adapter_ip_info_t ip_info;
    ESP_ERROR_CHECK(tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA , &ip_info));
    ESP_LOGI(TAG, "ip_info IP:%s" , inet_ntoa(ip_info.ip));
    ESP_LOGI(TAG, "ip_info Netmask:%s" , inet_ntoa(ip_info.netmask));
    ESP_LOGI(TAG, "ip_info GW:%s" , inet_ntoa(ip_info.gw));

    uint32_t ipinfo_temp = 0;
    ipinfo_temp = inet_addr("192.168.2.200");
    ESP_LOGI(TAG, "ip_info ip:%x" , ipinfo_temp );
    memcpy((uint8_t *)&ip_info.ip , (uint8_t *)&ipinfo_temp , 4);
    ipinfo_temp = inet_addr("255.255.255.0");
    ESP_LOGI(TAG, "ip_info netmask:%x" , ipinfo_temp );
    memcpy((uint8_t *)&ip_info.netmask , (uint8_t *)&ipinfo_temp , 4);
    ipinfo_temp = inet_addr("192.168.2.1");
    ESP_LOGI(TAG, "ip_info gw:%x" , ipinfo_temp );
    memcpy((uint8_t *)&ip_info.gw , (uint8_t *)&ipinfo_temp , 4);    

    ESP_ERROR_CHECK(tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_STA , &ip_info));
    ESP_LOGI(TAG, "ip_info SET" );
    ESP_ERROR_CHECK(tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA , &ip_info));
    ESP_LOGI(TAG, "ip_info IP:%s" , inet_ntoa(ip_info.ip));
    ESP_LOGI(TAG, "ip_info Netmask:%s" , inet_ntoa(ip_info.netmask));
    ESP_LOGI(TAG, "ip_info GW:%s" , inet_ntoa(ip_info.gw));

// --------------------------------------------------------

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());  
    ESP_ERROR_CHECK(esp_wifi_connect());

    ESP_LOGI(TAG, "wifi_init_sta finished.");
    ESP_LOGI(TAG, "connect to ap SSID:%s password:%s \n",\
             wifi_config.ap.ssid, wifi_config.ap.password);   
}


static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    DEBUG("event->event_id:%d\n" , event->event_id);
    
    switch (event->event_id) {
        case SYSTEM_EVENT_STA_START:
            ESP_LOGE(TAG, "SYSTEM_EVENT_STA_START");

            break;
        case SYSTEM_EVENT_STA_GOT_IP:

            Net_Task_Event_Start(NET_TASK_UDP_EVENT,EVENT_FROM_TASK);
            ESP_LOGE(TAG, "SYSTEM_EVENT_STA_GOT_IP");
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            ESP_LOGE(TAG, "SYSTEM_EVENT_STA_DISCONNECTED");
            
            break;
        case SYSTEM_EVENT_STA_CONNECTED:
            ESP_LOGE(TAG, "SYSTEM_EVENT_STA_CONNECTED");
            break;    
        default:
            ESP_LOGE(TAG, "SYSTEM_EVENT_STA_Default");
            break;
    }
    return ESP_OK;
}




void APP_Net_UDPProcess(void)
{
    /*
    SOCK_DGRAM - > UDP
    SOCK_STREAM - > TCP
    */

    //char addr_str[128];

    // struct sockaddr_in dest_addr;
    // dest_addr.sin_addr.s_addr = inet_addr("192.168.2.100");
    // dest_addr.sin_family = AF_INET;
    // dest_addr.sin_port = htons(20000);


    //inet_ntoa_r(dest_addr.sin_addr, addr_str, sizeof(addr_str) - 1);
//-----------------------ip Info Log----------------------------
    tcpip_adapter_ip_info_t ip_info;
    ESP_ERROR_CHECK(tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA , &ip_info));

    ESP_LOGI(TAG, "ip_info IPx:      %X" , *(uint32_t *)(&ip_info.ip));
    ESP_LOGI(TAG, "ip_info Netmaskx: %X" , *(uint32_t *)(&ip_info.netmask));
    ESP_LOGI(TAG, "ip_info GWx:      %X" , *(uint32_t *)(&ip_info.gw));

    ESP_LOGI(TAG, "ip_info IP:      %s" , inet_ntoa(ip_info.ip));
    ESP_LOGI(TAG, "ip_info Netmask: %s" , inet_ntoa(ip_info.netmask));
    ESP_LOGI(TAG, "ip_info GW:      %s" , inet_ntoa(ip_info.gw));

    tcpip_adapter_dns_info_t dns;

    ESP_ERROR_CHECK( tcpip_adapter_get_dns_info(TCPIP_ADAPTER_IF_STA, TCPIP_ADAPTER_DNS_MAIN, &dns));
    ESP_LOGI(TAG, "ip_info DNSx:      %X" , *(uint32_t *)(&dns.ip));
    ESP_LOGI(TAG, "ip_info DNS:      %s" , inet_ntoa(dns.ip));
    uint32_t dns_test = 0x08080808;
    memcpy((uint8_t *)&dns.ip ,(uint8_t *)&dns_test , 4  );
    ESP_ERROR_CHECK( tcpip_adapter_set_dns_info(TCPIP_ADAPTER_IF_STA, TCPIP_ADAPTER_DNS_MAIN, &dns));

    ESP_ERROR_CHECK( tcpip_adapter_get_dns_info(TCPIP_ADAPTER_IF_STA, TCPIP_ADAPTER_DNS_MAIN, &dns));
    ESP_LOGI(TAG, "ip_info DNSx:      %X" , *(uint32_t *)(&dns.ip));
    ESP_LOGI(TAG, "ip_info DNS:      %s" , inet_ntoa(dns.ip));   

    tcpip_adapter_dhcp_status_t dhcp_status;
    ESP_ERROR_CHECK( tcpip_adapter_dhcpc_get_status(TCPIP_ADAPTER_IF_STA, &dhcp_status));
    ESP_LOGI(TAG, "ip_info Dhcp c:      %d" , (uint8_t) dhcp_status); 

// -----------------------------------------------------------
    // -------SOCKET CONF-------
    sock[SOCKET_CONF] = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);

    struct sockaddr_in local_addr;
    local_addr.sin_addr.s_addr = inet_addr("192.168.2.200");
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(SOCKET_CONF_PORT);
    int err = bind(sock[SOCKET_CONF], (struct sockaddr *)&local_addr, sizeof(local_addr));
    // -------SOCKET SEND-------
    sock[SOCKET_SEND] = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    // -------------------------
    if (sock[SOCKET_SEND] == -1) 
    {
        ESP_LOGE(TAG, "Shutting down socket and restarting...");
        shutdown(sock[SOCKET_SEND], 0);
        close(sock[SOCKET_SEND]);
    }
    else
    {
        DEBUG("Socket SNED:%d OK!\n" , sock[SOCKET_SEND]);
    }

    if (sock[SOCKET_CONF] == -1) 
    {
        ESP_LOGE(TAG, "Shutting down socket and restarting...");
        shutdown(sock[SOCKET_CONF], 0);
        close(sock[SOCKET_CONF]);
    }
    else
    {
        DEBUG("Socket CONF:%d OK!\n" , sock[SOCKET_CONF]);
    }

}

int APP_Net_UDPSendBytes(int socketNUM , uint8_t * buf , uint16_t len ,uint32_t dest_ip , uint32_t port)
{
    struct sockaddr_in dest_addr;
    char addr_str[128];
    dest_addr.sin_addr.s_addr = dest_ip;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    inet_ntoa_r(dest_addr.sin_addr, addr_str, sizeof(addr_str) - 1);

    int err = sendto(socketNUM, buf, len, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    return err;
}
int APP_Net_UDPRevBytes(int socketNUM , uint8_t * buf , uint16_t len ,uint32_t * dest_ip , uint32_t * port)
{
    struct sockaddr_in dest_addr;
    int16_t iplen;
    int revlen = recvfrom(socketNUM, buf, len, 0, (struct sockaddr *)&dest_addr, &iplen);
    *dest_ip = dest_addr.sin_addr.s_addr;
    *port = htons(dest_addr.sin_port);

    return revlen;
}



void APP_Net_UDP_RevProcess(void)
{
    static const char *UDP_Rev_TAG = "UDP_Rev_TASK";
    esp_log_level_set(UDP_Rev_TAG, ESP_LOG_INFO);
    uint8_t* data = (uint8_t*) malloc(1024+1);
    while (1) 
    {
        if(sock[SOCKET_CONF] != -1)
        {
            int16_t rev_len = 0 ;
            uint32_t dest_ip;// = inet_addr("192.168.2.101");
            uint32_t port;// = 20000;

            rev_len = APP_Net_UDPRevBytes( sock[SOCKET_CONF] , data , 1024 , &dest_ip ,  &port);

            if (rev_len > 0) 
            {
                data[rev_len] = 0;

                ESP_LOGI(UDP_Rev_TAG, "IP %s ", inet_ntoa(dest_ip));
                ESP_LOGI(UDP_Rev_TAG, "port %d ", port);
                ESP_LOGI(UDP_Rev_TAG, "Read %d ", rev_len);
                DEBUG("-------------------\n");
                ESP_LOG_BUFFER_HEXDUMP(UDP_Rev_TAG, data, rev_len, ESP_LOG_INFO);
            }            
        }

        RTOS_Delay_ms(500);
    }
    free(data);
}




void APP_Net_UDP_SendQueue_Init(void)
{
    APP_Net_UDPSendQueue[0].buf = app_net_udpsend_space;
    APP_Net_UDPSendQueue[0].count = 0;
    APP_Net_UDPSendQueue[0].in = 0;
    APP_Net_UDPSendQueue[0].out = 0;
    APP_Net_UDPSendQueue[0].size = APP_NET_UDPSEND_SPACE_LEN;
}

void APP_Net_UDP_SendEnqueue(uint8_t udp_queue_num , uint8_t * buf , uint16_t len)
{
    if(udp_queue_num >= APP_NET_UDPSEND_QUEUE_NUM)
    {
        return ;
    }

    if(len <= APP_Net_UDPSendQueue[udp_queue_num].size)
    {
        for(uint16_t i = 0 ; i < len ; i ++)
        {
            APP_Net_UDPSendQueue[udp_queue_num].buf[APP_Net_UDPSendQueue[udp_queue_num].in] = buf[i];
            APP_Net_UDPSendQueue[udp_queue_num].in ++;
            APP_Net_UDPSendQueue[udp_queue_num].in %= APP_Net_UDPSendQueue[udp_queue_num].size;
            APP_Net_UDPSendQueue[udp_queue_num].count ++;
        }

        Net_Task_Event_Start(NET_TASK_UDP_SEND_EVENT, EVENT_FROM_TASK);

    }
    else
    {
        DEBUG("UDP Enqueue Err\n");
    }
}

uint16_t APP_Net_UDP_SendQueue_Count(uint8_t udp_queue_num)
{
    return  APP_Net_UDPSendQueue[udp_queue_num].count ;
}

int8_t  APP_Net_UDP_SendDequeue(uint8_t udp_queue_num , uint8_t * buf ,uint16_t * len)
{
    if(udp_queue_num >= APP_NET_UDPSEND_QUEUE_NUM)
    {
        return -1;
    }

    *len = APP_Net_UDPSendQueue[udp_queue_num].count;
    for(uint16_t i = 0 ; i < *len ; i ++)
    {
        buf[i] = APP_Net_UDPSendQueue[udp_queue_num].buf[APP_Net_UDPSendQueue[udp_queue_num].out];
        APP_Net_UDPSendQueue[udp_queue_num].out ++;
        APP_Net_UDPSendQueue[udp_queue_num].out %= APP_Net_UDPSendQueue[udp_queue_num].size;
        APP_Net_UDPSendQueue[udp_queue_num].count --;
    }

    return 1;
}


void APP_Net_UDPSend_Process(void)
{
    if(sock[SOCKET_SEND] != -1)
    {
        if(APP_Net_UDP_SendQueue_Count(0) > 0)
        {
            uint8_t * send_buf = 0;
            uint16_t len = 0;
            send_buf = malloc(4096 * sizeof(uint8_t));
            if(  APP_Net_UDP_SendDequeue(0 , send_buf ,&len) == 1)
            {
                uint32_t dest_ip = inet_addr("192.168.2.102");
                uint32_t port = 20000;
                int err = APP_Net_UDPSendBytes(sock[SOCKET_SEND] , send_buf , len ,dest_ip ,  port);
                DEBUG("UDP Send Func return:%d\n" , err); 
            }
            else
            {
                /* code */
            }

            free(send_buf);
        }


        if(APP_Net_UDP_SendQueue_Count(0) > 0)
        {
            Net_Task_Event_Start(NET_TASK_UDP_SEND_EVENT, EVENT_FROM_TASK);
        }
        
    }
    
}

// ------------Test Code ---------------

void APP_Net_UDP_TestCode(void)
{
    uint8_t send_test[1024] = { 0 };

    for(uint16_t i = 0 ; i < 1024 ; i ++)
    {
        send_test[i] = (uint8_t )i;
    }

    for(uint8_t i = 0 ; i < 3; i ++)
    {
        APP_Net_UDP_SendEnqueue( 0 , send_test , 1024);     
    }
    
}



// -------------------------------------


/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

