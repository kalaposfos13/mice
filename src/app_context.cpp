#include "app_context.h"
#include <orbis/UserService.h>

AppContext ctx{};

AppContext::AppContext() {
    OrbisUserServiceInitializeParams param;
    param.priority = ORBIS_KERNEL_PRIO_FIFO_LOWEST;
    sceUserServiceInitialize(&param);
    sceUserServiceGetInitialUser(&user_id);
    LOG_INFO("userid: {:x}", (u32)user_id);
    pad.Init(ctx.user_id);
    mice.Init(ctx.user_id);
}