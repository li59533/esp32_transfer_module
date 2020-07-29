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


static esp_err_t http_resp_index_html(httpd_req_t *req)
{
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
                                    "ssid:<input type=\"text\" name=\"text1\">"
                                    "<br>"
                                    "<input type=\"submit\" value=\"Submit\">"
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
    char content[100];
    memset(content , 0 , sizeof(content));
    size_t rev_size = MIN(req->content_len , sizeof(content));
    int ret = httpd_req_recv(req , content , rev_size);
    printf("content:%s\n" ,content );
    char param[50];
    if (httpd_query_key_value(content, "text1", param, sizeof(param)) == ESP_OK) {
        printf("Found URL query parameter => text1=%s\n", param);
    }
    if (httpd_query_key_value(content, "text2", param, sizeof(param)) == ESP_OK) {
        printf("Found URL query parameter => text2=%s\n", param);
    }

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

