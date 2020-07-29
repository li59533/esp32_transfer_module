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
#include "app_protocol.h"
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



const char storage_name_workingmode[]   = "workingmode";
const char storage_name_ssid[]          = "ssid";
const char storage_name_password[]      = "password"; 
const char storage_name_dhcp_flag[]       = "dhcp_flag";
const char storage_name_DNS[]             = "DNS";
const char storage_name_gateway[]         = "gateway";
const char storage_name_domainname_flag[] = "domainname_flag";
const char storage_name_domain_name[]     = "domain_name";
const char storage_name_target_ip[]       = "target_ip";
const char storage_name_target_port[]     = "target_port";
const char storage_name_local_ip[]        = "local_ip";
const char storage_name_local_port[]      = "local_port";
const char storage_name_local_conf_port[] = "local_conf_port";
const char storage_name_netmask[]         = "netmask";

const char sys_default_ssid[]       = "Bigym";
const char sys_default_password[]   = "1234567890";
const char sys_default_domainname[] = "none";

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

nvs_handle_t nvs_sys_handle;

static SystemParam_Config_t SystemParam_Config_Default = 
{

    .workingmode = WorkingMode_UDP,
    .dhcp_flag = DHCP_F_CLOSE,
    .DNS = 0x08080808,
    .gateway = 0x0102a8c0, // 192.168.2.1 0102a8c0
    .domainname_flag = DOMAIN_F_CLOSE,
    .target_ip = 0x6502a8c0, // 192.168.2.102
    .target_port = 20000 , 
    .local_ip = 0xc802a8c0 , //192.168.2.200
    .local_port = 0, // random port if value equal 0
    .local_conf_port = SYS_UDP_CONF_PORT , 
    .netmask = 0x00ffffff,
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
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) 
    {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );
    SystemParam_Read(); 
}


int16_t SystemParam_Read(void)
{
    esp_err_t err ; 
    // Open
    printf("\n");
    printf("Opening Non-Volatile Storage (NVS) handle... \n");
    err = nvs_open("storage", NVS_READWRITE, &nvs_sys_handle);
    if (err != ESP_OK) 
    {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } 
    else 
    {
        // Read
        printf("Reading System from NVS ...\n");

        size_t length = 0;
        // ---------- workingmode ----------
        err = nvs_get_u8(nvs_sys_handle, storage_name_workingmode, &g_SystemParam_Config.workingmode);
        // ---------------------------------
        // ---------- ssid -----------------    
        err = nvs_get_str(nvs_sys_handle, storage_name_ssid , NULL, &length);
        if(length >= SYS_SSID_MAX)
        {
            err = ESP_ERR_NVS_NOT_FOUND;
        }
        else
        {
            err = nvs_get_str(nvs_sys_handle, storage_name_ssid , g_SystemParam_Config.ssid, &length);
            g_SystemParam_Config.ssid[length] = 0;
        }
        printf("ssid err:%d\n" , err);
        // ---------------------------------
        // ----------- password ------------
        err = nvs_get_str(nvs_sys_handle, storage_name_password , NULL, &length);
        if(length >= SYS_PASSWORD_MAX)
        {
            err = ESP_ERR_NVS_NOT_FOUND;
        }
        else
        {
            err = nvs_get_str(nvs_sys_handle, storage_name_password , g_SystemParam_Config.password, &length);
            g_SystemParam_Config.password[length] = 0;
        }
        printf("password err:%d\n" , err);
        // ---------------------------------
        // ----------- dhcp flag -----------
        err = nvs_get_u8(nvs_sys_handle, storage_name_dhcp_flag, &g_SystemParam_Config.dhcp_flag);
        printf("dhcp err:%d\n" , err);
        // ---------------------------------
        // ----------- DNS -----------------
        err = nvs_get_u32(nvs_sys_handle, storage_name_DNS, &g_SystemParam_Config.DNS);
        printf("dns err:%d\n" , err);
        // ---------------------------------
        // ----------- gateway -------------
        err = nvs_get_u32(nvs_sys_handle, storage_name_gateway, &g_SystemParam_Config.gateway);
        printf("gateway err:%d\n" , err);
        // ---------------------------------
        // ----------- domainname flag------
        err = nvs_get_u8(nvs_sys_handle, storage_name_domainname_flag, &g_SystemParam_Config.domainname_flag);
        printf("domainname flag err:%d\n" , err);
        // ---------------------------------
        // ----------- domain name ---------
        err = nvs_get_str(nvs_sys_handle, storage_name_domain_name , NULL, &length);
        if(length >= SYS_PASSWORD_MAX)
        {
            err = ESP_ERR_NVS_NOT_FOUND;
        }
        else
        {
            err = nvs_get_str(nvs_sys_handle, storage_name_domain_name , g_SystemParam_Config.domain_name, &length);
            g_SystemParam_Config.domain_name[length] = 0;
        }
        printf("domain name err:%d\n" , err);
        // ---------------------------------
        // ----------- target_ip -----------
        err = nvs_get_u32(nvs_sys_handle, storage_name_target_ip, &g_SystemParam_Config.target_ip);
        printf("target ip err:%d\n" , err);
        // ---------------------------------
        // ----------- target port ---------
        err = nvs_get_u16(nvs_sys_handle, storage_name_target_port, &g_SystemParam_Config.target_port);
        printf("target port err:%d\n" , err);
        // ---------------------------------
        // ----------- local ip ------------
        err = nvs_get_u32(nvs_sys_handle, storage_name_local_ip, &g_SystemParam_Config.local_ip);
        printf("local ip err:%d\n" , err);
        // ---------------------------------
        // ----------- local port ----------
        err = nvs_get_u16(nvs_sys_handle, storage_name_local_port, &g_SystemParam_Config.local_port);
        printf("local port err:%d\n" , err);
        // ---------------------------------
        // ----------- local conf port -----
        err = nvs_get_u16(nvs_sys_handle, storage_name_local_conf_port, &g_SystemParam_Config.local_conf_port);
        printf("local conf port err:%d\n" , err);
        // ---------------------------------
        // ----------- netmask -------------
        err = nvs_get_u32(nvs_sys_handle, storage_name_netmask, &g_SystemParam_Config.netmask);
        printf("netmask err:%d\n" , err);
        // ---------------------------------
        nvs_close(nvs_sys_handle);


        switch (err) {
            case ESP_OK:

                printf("workingmode: %x\n" , g_SystemParam_Config.workingmode);
                printf("ssid:%s\n" , g_SystemParam_Config.ssid);
                printf("password:%s\n" , g_SystemParam_Config.password);
                printf("dhcp_flag:%x\n" , g_SystemParam_Config.dhcp_flag);
                printf("dns:%x\n" , g_SystemParam_Config.DNS);
                printf("gateway:%x\n" , g_SystemParam_Config.gateway);    
                printf("demainname_flag:%x\n" , g_SystemParam_Config.domainname_flag);            
                printf("domain_name:%s\n" , g_SystemParam_Config.domain_name);  
                printf("target ip:%x\n" , g_SystemParam_Config.target_ip);
                printf("target port:%d\n" , g_SystemParam_Config.target_port);
                printf("local ip :%x\n" , g_SystemParam_Config.local_ip);
                printf("local port:%d\n" , g_SystemParam_Config.local_port);
                printf("local conf port:%d\n" ,g_SystemParam_Config.local_conf_port );
                printf("netmask :%x\n" ,g_SystemParam_Config.netmask );
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
    err = nvs_open("storage", NVS_READWRITE, &nvs_sys_handle);
    if (err != ESP_OK) 
    {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }
    else
    {        
        printf("Rest SystemParam in NVS ... \n");
        memcpy(SystemParam_Config_Default.ssid , sys_default_ssid , strlen(sys_default_ssid));
        memcpy(SystemParam_Config_Default.password , sys_default_password , strlen(sys_default_password));
        memcpy(SystemParam_Config_Default.domain_name , sys_default_domainname , strlen(sys_default_domainname));
        g_SystemParam_Config = SystemParam_Config_Default;
        // ----------- write -----------
        // 
        // ---------- workingmode -------
        err = nvs_set_u8(nvs_sys_handle, storage_name_workingmode, g_SystemParam_Config.workingmode);
        // ------------------------------
        // ---------- ssid --------------
        err = nvs_set_str(nvs_sys_handle, storage_name_ssid , g_SystemParam_Config.ssid);
        // ------------------------------
        // ---------- password ----------
        err = nvs_set_str(nvs_sys_handle, storage_name_password , g_SystemParam_Config.password);
        // ------------------------------
        // ---------- dhcp flag ---------
        err = nvs_set_u8(nvs_sys_handle, storage_name_dhcp_flag, g_SystemParam_Config.dhcp_flag);
        // ------------------------------
        // ---------- DNS ---------------
        err = nvs_set_u32(nvs_sys_handle, storage_name_DNS, g_SystemParam_Config.DNS);
        // ------------------------------
        // ---------- gateway -----------
        err = nvs_set_u32(nvs_sys_handle, storage_name_gateway, g_SystemParam_Config.gateway);
        // ------------------------------
        // ---------- domain name flag---
        err = nvs_set_u8(nvs_sys_handle, storage_name_domainname_flag, g_SystemParam_Config.domainname_flag);
        // ------------------------------
        // ---------- domian name -------
        err = nvs_set_str(nvs_sys_handle, storage_name_domain_name , g_SystemParam_Config.domain_name);
        // ------------------------------
        // ---------- target ip ---------
        err = nvs_set_u32(nvs_sys_handle, storage_name_target_ip, g_SystemParam_Config.target_ip);
        // ------------------------------
        // ---------- target port -------
        err = nvs_set_u16(nvs_sys_handle, storage_name_target_port, g_SystemParam_Config.target_port);
        // ------------------------------
        // ---------- local ip ----------
        err = nvs_set_u32(nvs_sys_handle, storage_name_local_ip, g_SystemParam_Config.local_ip);
        // ------------------------------
        // ---------- local port --------
        err = nvs_set_u16(nvs_sys_handle, storage_name_local_port, g_SystemParam_Config.local_port);
        // ------------------------------
        // ---------- local conf port ---
        err = nvs_set_u16(nvs_sys_handle, storage_name_local_conf_port, g_SystemParam_Config.local_conf_port);
        // ------------------------------
        // ---------- netmask -----------
        err = nvs_set_u32(nvs_sys_handle, storage_name_netmask, g_SystemParam_Config.netmask);
        // ------------------------------
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

        // Commit written value.
        // After setting any values, nvs_commit() must be called to ensure changes are written
        // to flash storage. Implementations may write to storage at other times,
        // but this is not guaranteed.
        printf("Committing updates in NVS ... \n");
        err = nvs_commit(nvs_sys_handle);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");  

        nvs_close(nvs_sys_handle);
    }
     


  
}


void SystemParam_Save(void)
{
    esp_err_t err ; 
    err = nvs_open("storage", NVS_READWRITE, &nvs_sys_handle);
    if (err != ESP_OK) 
    {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } 
    else 
    {
        // Write
        printf("Write SystemParam in NVS ... ");

        // ----------- write -----------
        // 
        // ---------- workingmode -------
        err = nvs_set_u8(nvs_sys_handle, storage_name_workingmode, g_SystemParam_Config.workingmode);
        // ------------------------------
        // ---------- ssid --------------
        err = nvs_set_str(nvs_sys_handle, storage_name_ssid , g_SystemParam_Config.ssid);
        // ------------------------------
        // ---------- password ----------
        err = nvs_set_str(nvs_sys_handle, storage_name_password , g_SystemParam_Config.password);
        // ------------------------------
        // ---------- dhcp flag ---------
        err = nvs_set_u8(nvs_sys_handle, storage_name_dhcp_flag, g_SystemParam_Config.dhcp_flag);
        // ------------------------------
        // ---------- DNS ---------------
        err = nvs_set_u32(nvs_sys_handle, storage_name_DNS, g_SystemParam_Config.DNS);
        // ------------------------------
        // ---------- gateway -----------
        err = nvs_set_u32(nvs_sys_handle, storage_name_gateway, g_SystemParam_Config.gateway);
        // ------------------------------
        // ---------- domain name flag---
        err = nvs_set_u8(nvs_sys_handle, storage_name_domainname_flag, g_SystemParam_Config.domainname_flag);
        // ------------------------------
        // ---------- domian name -------
        err = nvs_set_str(nvs_sys_handle, storage_name_domain_name , g_SystemParam_Config.domain_name);
        // ------------------------------
        // ---------- target ip ---------
        err = nvs_set_u32(nvs_sys_handle, storage_name_target_ip, g_SystemParam_Config.target_ip);
        // ------------------------------
        // ---------- target port -------
        err = nvs_set_u16(nvs_sys_handle, storage_name_target_port, g_SystemParam_Config.target_port);
        // ------------------------------
        // ---------- local ip ----------
        err = nvs_set_u32(nvs_sys_handle, storage_name_local_ip, g_SystemParam_Config.local_ip);
        // ------------------------------
        // ---------- local port --------
        err = nvs_set_u16(nvs_sys_handle, storage_name_local_port, g_SystemParam_Config.local_port);
        // ------------------------------
        // ---------- local conf port ---
        err = nvs_set_u16(nvs_sys_handle, storage_name_local_conf_port, g_SystemParam_Config.local_conf_port);
        // ------------------------------
        // ---------- netmask -----------
        err = nvs_set_u32(nvs_sys_handle, storage_name_netmask, g_SystemParam_Config.netmask);
        // ------------------------------

        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

        // Commit written value.
        // After setting any values, nvs_commit() must be called to ensure changes are written
        // to flash storage. Implementations may write to storage at other times,
        // but this is not guaranteed.
        printf("Committing updates in NVS ... \n");
        err = nvs_commit(nvs_sys_handle);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

        // Close
        nvs_close(nvs_sys_handle);
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

