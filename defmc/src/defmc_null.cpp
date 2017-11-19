#include <dmsdk/sdk.h>
#include "defmc_private.h"

#if !defined(DM_PLATFORM_ANDROID) && !defined(DM_PLATFORM_IOS)
int DefMcPlatform_Init()
{
	dmLogInfo("NULL DefMcPlatform_Init");
	return 0;
}

int DefMcPlatform_Start(uint32_t sample_rate, uint32_t sample_delay)
{
	dmLogInfo("NULL DefMcPlatform_Start");
	return 0;
}

int DefMcPlatform_Stop()
{
	dmLogInfo("NULL DefMcPlatform_Stop");
	return 0;
}

int DefMcPlatform_SampleAmplitude(float &amp)
{
	//dmLogInfo("NULL DefMcPlatform_SampleAmplitude");
	amp = 50.0f;
	return 0;
}
#endif