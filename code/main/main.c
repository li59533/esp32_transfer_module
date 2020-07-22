#include <stdio.h>
#include "clog.h"
#include "rtos_tools.h"
#include "task_list.h"
#include "system_param.h"
#include "esp_system.h"

#include "version.h"

void app_main()
{
    DEBUG("Version:%s\n",Version_Get_Str());
    RTOS_Delay_ms(1000);
    DEBUG("Start\n");
    SystemParam_Init();
    int16_t temp = 0;
    temp = SystemParam_Read();
    printf("temp: %d\n" , temp);

    RTOS_Init();



/*
     // Restart module
    for (int i = 1200; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        RTOS_Delay_ms(1000);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();  
*/
     
}

