/**
 **************************************************************************************************
 * @file        system_param.c
 * @author
 * @version
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *              
 **************************************************************************************************
 */
#include "self_def.h"
#include "system_param.h"

/**
 * @addtogroup    XXX 
 * @{  
 */
#include "clog.h"

#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"


/**
 * @addtogroup    system_param_Modules 
 * @{  
 */

/**
 * @defgroup      system_param_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      system_param_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      system_param_Constants_Defines 
 * @brief         
 * @{  
 */

const char storage_name_NetMode[] = "Net_Mode";
const char storage_name_Local_IP[] = "Local_IP";
const char storage_name_Local_port[] = "Local_port";
const char storage_name_Target_IP[] = "Target_IP";
const char storage_name_Target_port[] = "Target_port";
const char storage_name_DHCP_Flag[] = "DHCP_Flag";

/**
 * @}
 */

/**
 * @defgroup      system_param_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      system_param_Private_Variables 
 * @brief         
 * @{  
 */
const SystemParam_Config_t SystemParam_Config_Default = 
{
    .Net_Mode = 0x00 ,
    .Local_IP = 0xc0a802fa ,
    .Local_port = 20000 , 
    .Target_IP = 0xc0a80264 , 
    .Target_port = 20000 ,
    .DHCP_Flag = 0x00,
};
/**
 * @}
 */

/**
 * @defgroup      system_param_Public_Variables 
 * @brief         
 * @{  
 */
SystemParam_Config_t g_SystemParam_Config;
/**
 * @}
 */

/**
 * @defgroup      system_param_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      system_param_Functions 
 * @brief         
 * @{  
 */
 
 
 
void SystemParam_Init(void)
{		
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );


}

nvs_handle_t my_handle;
int16_t SystemParam_Read(void)
{
    esp_err_t err ; 
    // Open
    printf("\n");
    printf("Opening Non-Volatile Storage (NVS) handle... \n");
    err = nvs_open("storage", NVS_READONLY, &my_handle);
    if (err != ESP_OK) 
    {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } 
    else 
    {
        // Read
        printf("Reading System from NVS ...\n");
        err = nvs_get_u8(my_handle, storage_name_NetMode, &g_SystemParam_Config.Net_Mode);
        err = nvs_get_u32(my_handle, storage_name_Local_IP, &g_SystemParam_Config.Local_IP);
        err = nvs_get_u16(my_handle, storage_name_Local_port, &g_SystemParam_Config.Local_port);
        err = nvs_get_u32(my_handle, storage_name_Target_IP, &g_SystemParam_Config.Target_IP);        
        err = nvs_get_u16(my_handle, storage_name_Target_port, &g_SystemParam_Config.Target_port);
        err = nvs_get_u8(my_handle, storage_name_DHCP_Flag, &g_SystemParam_Config.DHCP_Flag);
        
        nvs_close(my_handle);


        switch (err) {
            case ESP_OK:
                printf("NetMode:%x\n" , g_SystemParam_Config.Net_Mode);
                printf("Local_IP:%x\n" , g_SystemParam_Config.Local_IP);
                printf("Local_port:%x\n" , g_SystemParam_Config.Local_port);
                printf("Target_IP:%x\n" , g_SystemParam_Config.Target_IP);
                printf("Target_port:%x\n" , g_SystemParam_Config.Target_port);
                printf("DHCP_Flag:%x\n" , g_SystemParam_Config.DHCP_Flag);
                printf("Read System OK\n");
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("Read System Err!NOT_FOUND\n");
                SystemParam_Rest();
                break;
            default :
                printf("Read System Err!!\n");
        }
        // Close
        
    }
    return err;
}

void SystemParam_Rest(void)
{
    esp_err_t err ; 
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) 
    {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }
    else
    {        
        printf("Rest SystemParam in NVS ... \n");
        g_SystemParam_Config = SystemParam_Config_Default;

        err = nvs_set_u8(my_handle, storage_name_NetMode, g_SystemParam_Config.Net_Mode);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");
        err = nvs_set_u32(my_handle, storage_name_Local_IP, g_SystemParam_Config.Local_IP);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");
        err = nvs_set_u16(my_handle, storage_name_Local_port, g_SystemParam_Config.Local_port);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");
        err = nvs_set_u32(my_handle, storage_name_Target_IP, g_SystemParam_Config.Target_IP);      
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");  
        err = nvs_set_u16(my_handle, storage_name_Target_port, g_SystemParam_Config.Target_port);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");
        err = nvs_set_u8(my_handle, storage_name_DHCP_Flag, g_SystemParam_Config.DHCP_Flag);
         
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

        // Commit written value.
        // After setting any values, nvs_commit() must be called to ensure changes are written
        // to flash storage. Implementations may write to storage at other times,
        // but this is not guaranteed.
        printf("Committing updates in NVS ... \n");
        err = nvs_commit(my_handle);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");  

        nvs_close(my_handle);
    }
     


  
}


void SystemParam_Save(void)
{
    esp_err_t err ; 
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) 
    {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } 
    else 
    {
        // Write
        printf("Write SystemParam in NVS ... ");

        err = nvs_set_u8(my_handle, storage_name_NetMode, g_SystemParam_Config.Net_Mode);
        err = nvs_set_u32(my_handle, storage_name_Local_IP, g_SystemParam_Config.Local_IP);
        err = nvs_set_u16(my_handle, storage_name_Local_port, g_SystemParam_Config.Local_port);
        err = nvs_set_u32(my_handle, storage_name_Target_IP, g_SystemParam_Config.Target_IP);        
        err = nvs_set_u16(my_handle, storage_name_Target_port, g_SystemParam_Config.Target_port);
        err = nvs_set_u8(my_handle, storage_name_DHCP_Flag, g_SystemParam_Config.DHCP_Flag);
         
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

        // Commit written value.
        // After setting any values, nvs_commit() must be called to ensure changes are written
        // to flash storage. Implementations may write to storage at other times,
        // but this is not guaranteed.
        printf("Committing updates in NVS ... \n");
        err = nvs_commit(my_handle);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

        // Close
        nvs_close(my_handle);
    }
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

