#include <dmsdk/sdk.h>
#include "defmc_private.h"

#if !defined(DM_PLATFORM_ANDROID) && !defined(DM_PLATFORM_IOS)
int DefMcPlatform_Init()
{
	dmLogError("NULL DefMcPlatform_Init");
	return 0;
}
#endif