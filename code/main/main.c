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
    DEBUG("Start\n");
    SystemParam_Init();
    RTOS_Init();
}

