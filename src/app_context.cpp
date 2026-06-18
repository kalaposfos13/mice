#include <orbis/UserService.h>
#include "app_context.h"

AppContext ctx{};

AppContext::AppContext() {
    OrbisUserServiceInitializeParams param;
    param.priority = ORBIS_KERNEL_PRIO_FIFO_LOWEST;
    sceUserServiceInitialize(&param);
    sceUserServiceGetInitialUser(&user_id);
    LOG_INFO("userid: {:x}", (u32)user_id);
    pad.Init(user_id);
    mice.Init(user_id);
}