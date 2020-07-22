/**
 **************************************************************************************************
 * @file        app_uart.c
 * @author
 * @version   v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#include "app_uart.h"
/**
 * @addtogroup    XXX 
 * @{  
 */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"
#include "clog.h"

/**
 * @addtogroup    app_uart_Modules 
 * @{  
 */

/**
 * @defgroup      app_uart_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_uart_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      app_uart_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_uart_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      app_uart_Private_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      app_uart_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_uart_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_uart_Functions 
 * @brief         
 * @{  
 */
#define TXD_PIN (GPIO_NUM_4)
#define RXD_PIN (GPIO_NUM_5)

static const int RX_BUF_SIZE = 2048;

void APP_Uart_Init(void)
{
    const uart_config_t uart_config = {
    .baud_rate = 115200,//115200,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_1, &uart_config));
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    // We won't use a buffer for sending data.
    uart_driver_install(UART_NUM_1, 2048, 0, 0, NULL, 0);    

    // -----------interrupt-----------------
    //esp_err_t uart_enable_intr_mask(uart_port_t uart_num, uint32_t enable_mask)
    // -------------------------------------

}

void APP_Uart_SendBytes(uint8_t * buf , uint16_t len)
{
    const int txBytes = uart_write_bytes(UART_NUM_1, (const char *)buf, len);
    ESP_LOGI("logName", "Wrote %d bytes", txBytes);
}


void APP_Uart_RevProcess(void)
{
    static const char *RX_TASK_TAG = "RX_TASK";
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE+1);
    while (1) {
        const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 1000 / portTICK_RATE_MS);
        if (rxBytes > 0) {
            data[rxBytes] = 0;
            ESP_LOGI(RX_TASK_TAG, "Read %d ", rxBytes);
            DEBUG("-------------------\n");
            ESP_LOG_BUFFER_HEXDUMP(RX_TASK_TAG, data, rxBytes, ESP_LOG_INFO);
        }
    }
    free(data);
}


// -------------------Test Code --------------------------

void APP_Uart_TestCode(void)
{
    uint8_t buf[1024] = {0};

    for(uint16_t i = 0 ; i < 1024 ; i ++)
    {
        buf[i] = i;
    }

    APP_Uart_SendBytes(buf , sizeof(buf));
}


void APP_Uart_TestCode2(void)
{
    static const char *RX_TASK_TAG = "RX_TASK";
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE+1);
    while (1) {
        const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 1000 / portTICK_RATE_MS);
        if (rxBytes > 0) {
            data[rxBytes] = 0;
            ESP_LOGI(RX_TASK_TAG, "Read %d ", rxBytes);
            DEBUG("-------------------\n");
            ESP_LOG_BUFFER_HEXDUMP(RX_TASK_TAG, data, rxBytes, ESP_LOG_INFO);
        }
    }
    free(data);
}



// -------------------------------------------------------

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

