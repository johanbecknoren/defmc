#include <dmsdk/sdk.h>
#include "defmc_private.h"

#if !defined(DM_PLATFORM_ANDROID) && !defined(DM_PLATFORM_IOS)
int DefMcPlatform_Init()
{
	dmLogInfo("NULL DefMcPlatform_Init");
	return 0;
}

int DefMcPlatform_Start()
{
	dmLogInfo("NULL DefMcPlatform_Start");
	return 0;
}
#endif