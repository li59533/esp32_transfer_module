/**
 **************************************************************************************************
 * @file        app_web.c
 * @author
 * @app_web
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


#include "app_web.h"

#include <sys/param.h>

#include "esp_log.h"
#include "esp_http_server.h"
#include "mdns.h"
#include <lwip/netdb.h>
#include "system_param.h"

/**
 * @addtogroup    app_web_Modules 
 * @{  
 */

/**
 * @defgroup      app_web_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       app_web_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_web_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       app_web_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_web_Private_Variables 
 * @brief         
 * @{  
 */

static const char *TAG = "web_server";

/**
 * @}
 */

/**
 * @defgroup      app_web_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_web_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */
static esp_err_t http_resp_index_html(httpd_req_t *req);
static esp_err_t up_conf_func(httpd_req_t *req);
static esp_err_t index_handler(httpd_req_t *req);

/**
 * @}
 */

/**
 * @defgroup      app_web_Functions 
 * @brief         
 * @{  
 */
void APP_Web_Init_mdns(void)
{
    mdns_init();
    mdns_hostname_set("device-conf");
    mdns_instance_name_set("esp home web server");

    mdns_txt_item_t serviceTxtData[] = {
        {"board", "esp32"},
        {"path", "/"}
    };

    ESP_ERROR_CHECK(mdns_service_add("ESP32-WebServer", "_http", "_tcp", 80, serviceTxtData,
                                     sizeof(serviceTxtData) / sizeof(serviceTxtData[0])));
}

static esp_err_t http_resp_index_html(httpd_req_t *req)
{

    char str_buf[50];
    printf("index html\n");
    /* Send file-list table definition and column labels */
    httpd_resp_sendstr_chunk(req, 
                                "<!DOCTYPE html>"
                                "<html lang=\"en\">"
                                "<head>"
                                    "<meta charset=\"UTF-8\">"
                                    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                                    "<title>Configuration</title>"
                                    "<h1 align = \"center\">Configuration</h1>"
                                    "<hr/>"
                                "</head>"
                                "<body align = \"center\">"
                                "<form method=\"POST\" action=\"/up_conf/\">"
                                );
    // ------------- workingmode ----------
    snprintf(str_buf , 50 , "%d",g_SystemParam_Config.workingmode);
    httpd_resp_sendstr_chunk(req,"workingmode:<input type=\"text\" name=\"workingmode\" value=\"");                            
    httpd_resp_sendstr_chunk(req,str_buf);
    httpd_resp_sendstr_chunk(req,"\"><br>");
    // ------------- ssid -----------------
    httpd_resp_sendstr_chunk(req,"ssid:<input type=\"text\" name=\"ssid\" value=\"");                            
    httpd_resp_sendstr_chunk(req,g_SystemParam_Config.ssid);
    httpd_resp_sendstr_chunk(req,"\"><br>");
    // ------------- password -------------
    httpd_resp_sendstr_chunk(req,"password:<input type=\"text\" name=\"password\" value=\"");                            
    httpd_resp_sendstr_chunk(req,g_SystemParam_Config.password);
    httpd_resp_sendstr_chunk(req,"\"><br>");
    // ------------- dhcp flag ------------
    snprintf(str_buf , 50 , "%d",g_SystemParam_Config.dhcp_flag);
    httpd_resp_sendstr_chunk(req,"dhcp_flag:<input type=\"text\" name=\"dhcp_flag\" value=\"");                            
    httpd_resp_sendstr_chunk(req,str_buf);
    httpd_resp_sendstr_chunk(req,"\"><br>");  
    // ------------- DNS ------------------
    snprintf(str_buf , 50 , "%s",inet_ntoa(g_SystemParam_Config.DNS));
    httpd_resp_sendstr_chunk(req,"DNS:<input type=\"text\" name=\"DNS\" value=\"");                            
    httpd_resp_sendstr_chunk(req,str_buf);
    httpd_resp_sendstr_chunk(req,"\"><br>");   
    // ------------- gateway --------------
    snprintf(str_buf , 50 , "%s",inet_ntoa(g_SystemParam_Config.gateway));
    httpd_resp_sendstr_chunk(req,"gateway:<input type=\"text\" name=\"gateway\" value=\"");                            
    httpd_resp_sendstr_chunk(req,inet_ntoa(g_SystemParam_Config.gateway));
    httpd_resp_sendstr_chunk(req,"\"><br>");  
    // ------------- domainname flag ------
    snprintf(str_buf , 50 , "%d",g_SystemParam_Config.domainname_flag);
    httpd_resp_sendstr_chunk(req,"domainname_flag:<input type=\"text\" name=\"domainname_flag\" value=\"");                            
    httpd_resp_sendstr_chunk(req,str_buf);
    httpd_resp_sendstr_chunk(req,"\"><br>"); 
    // ------------- domain name ----------
    httpd_resp_sendstr_chunk(req,"domain_name:<input type=\"text\" name=\"domain_name\" value=\"");                            
    httpd_resp_sendstr_chunk(req,g_SystemParam_Config.domain_name);
    httpd_resp_sendstr_chunk(req,"\"><br>");
    // ------------- target ip ------------
    snprintf(str_buf , 50 , "%s",inet_ntoa(g_SystemParam_Config.target_ip));
    httpd_resp_sendstr_chunk(req,"target_ip:<input type=\"text\" name=\"target_ip\" value=\"");                            
    httpd_resp_sendstr_chunk(req,str_buf);
    httpd_resp_sendstr_chunk(req,"\"><br>");  
    // ------------- target port ----------
    snprintf(str_buf , 50 , "%d",g_SystemParam_Config.target_port);
    httpd_resp_sendstr_chunk(req,"target_port:<input type=\"text\" name=\"target_port\" value=\"");                            
    httpd_resp_sendstr_chunk(req,str_buf);
    httpd_resp_sendstr_chunk(req,"\"><br>"); 
    // ------------- local ip -------------
    snprintf(str_buf , 50 , "%s",inet_ntoa(g_SystemParam_Config.local_ip));
    httpd_resp_sendstr_chunk(req,"local_ip:<input type=\"text\" name=\"local_ip\" value=\"");                            
    httpd_resp_sendstr_chunk(req,str_buf);
    httpd_resp_sendstr_chunk(req,"\"><br>");  
    // ------------- local port -----------
    snprintf(str_buf , 50 , "%d",g_SystemParam_Config.local_port);
    httpd_resp_sendstr_chunk(req,"local_port:<input type=\"text\" name=\"local_port\" value=\"");                            
    httpd_resp_sendstr_chunk(req,str_buf);
    httpd_resp_sendstr_chunk(req,"\"><br>");     
    // ------------------------------------

    // ------------------------------------
    httpd_resp_sendstr_chunk(req,  "<input type=\"submit\" value=\"Submit\">"
                                "</form>"
                                "</body>"
                                "</html>"
    );

    /* Send empty chunk to signal HTTP response completion */
    httpd_resp_sendstr_chunk(req, NULL);
    return ESP_OK;
}

static esp_err_t up_conf_func(httpd_req_t *req)
{

    
    printf("up_conf_func\n");
    char content[300];
    memset(content , 0 , sizeof(content));
    size_t rev_size = MIN(req->content_len , sizeof(content));
    int ret = httpd_req_recv(req , content , rev_size);
    printf("content:%s\n" ,content );
    char param[50];
    // -------------- workingmode ------------
    if (httpd_query_key_value(content, "workingmode", param, sizeof(param)) == ESP_OK) {
        printf("Found URL workingmode=%s\n", param);
    }
    // ---------------------------------------
    // -------------- ssid ------------
    if (httpd_query_key_value(content, "ssid", param, sizeof(param)) == ESP_OK) {
        printf("Found URL ssid=%s\n", param);
    }
    // ---------------------------------------
    // -------------- password ------------
    if (httpd_query_key_value(content, "password", param, sizeof(param)) == ESP_OK) {
        printf("Found URL password=%s\n", param);
    }
    // ---------------------------------------
    // -------------- dhcp_flag ------------
    if (httpd_query_key_value(content, "dhcp_flag", param, sizeof(param)) == ESP_OK) {
        printf("Found URL dhcp_flag=%s\n", param);
    }
    // ---------------------------------------
        // -------------- DNS ------------
    if (httpd_query_key_value(content, "DNS", param, sizeof(param)) == ESP_OK) {
        printf("Found URL DNS=%s\n", param);
    }
    // ---------------------------------------
        // -------------- gateway ------------
    if (httpd_query_key_value(content, "gateway", param, sizeof(param)) == ESP_OK) {
        printf("Found URL gateway=%s\n", param);
    }
    // ---------------------------------------
        // -------------- domainname_flag ------------
    if (httpd_query_key_value(content, "domainname_flag", param, sizeof(param)) == ESP_OK) {
        printf("Found URL domainname_flag=%s\n", param);
    }
    // ---------------------------------------
        // -------------- domain_name ------------
    if (httpd_query_key_value(content, "domain_name", param, sizeof(param)) == ESP_OK) {
        printf("Found URL domain_name=%s\n", param);
    }
    // ---------------------------------------
        // -------------- target_ip ------------
    if (httpd_query_key_value(content, "target_ip", param, sizeof(param)) == ESP_OK) {
        printf("Found URL target_ip=%s\n", param);
    }
    // ---------------------------------------
        // -------------- target_port ------------
    if (httpd_query_key_value(content, "target_port", param, sizeof(param)) == ESP_OK) {
        printf("Found URL target_port=%s\n", param);
    }
    // ---------------------------------------
        // -------------- local_ip ------------
    if (httpd_query_key_value(content, "local_ip", param, sizeof(param)) == ESP_OK) {
        printf("Found URL local_ip=%s\n", param);
    }
    // ---------------------------------------
        // -------------- local_port ------------
    if (httpd_query_key_value(content, "local_port", param, sizeof(param)) == ESP_OK) {
        printf("Found URL local_port=%s\n", param);
    }
    // ---------------------------------------

    /* Respond with an empty chunk to signal HTTP response completion */
    //httpd_resp_send_chunk(req, NULL, 0);    

    httpd_resp_sendstr_chunk(req, 
                                "<!DOCTYPE html>"
                                "<html lang=\"en\">"
                                "<head>"
                                    "<meta charset=\"UTF-8\">"
                                    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                                    "<title>Configuration</title>"
                                    "<h1 align = \"center\">Configuration</h1>"
                                    "<hr/>"
                                "</head>"
                                "<body align = \"center\">"
                                    "<p>OK</p>"
                                "</body>"
                                "</html>"
    );
    httpd_resp_send_chunk(req, NULL, 0);  
    return ESP_OK ;
}

static esp_err_t index_handler(httpd_req_t *req)
{
    http_resp_index_html( req ); 
    return ESP_OK ;
}


esp_err_t APP_Web_StartServer(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    /* Use the URI wildcard matching function in order to
     * allow the same handler to respond to multiple different
     * target URIs which match the wildcard scheme */
    config.uri_match_fn = httpd_uri_match_wildcard;

    ESP_LOGI(TAG, "Starting HTTP Server");
    if (httpd_start(&server, &config) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start file server!");
        return ESP_FAIL;
    }

    /* URI handler for index */
    httpd_uri_t index_func = {
        .uri       = "/",  // 
        .method    = HTTP_GET,
        .handler   = index_handler,
    //    .user_ctx  = server_data    // Pass server data as context
    };
    httpd_register_uri_handler(server, &index_func);

    
    httpd_uri_t up_conf = {
        .uri       = "/up_conf/*",   // 
        .method    = HTTP_POST,
        .handler   = up_conf_func,
    //    .user_ctx  = server_data    // Pass server data as context
    };

    httpd_register_uri_handler(server, &up_conf);
    


    return ESP_OK;    
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

