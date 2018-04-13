#if defined(DM_PLATFORM_IOS)

#include <dmsdk/sdk.h>
#include "defmc_private.h"

int DefMcPlatform_Init()
{
	dmLogInfo("iOS DefMcPlatform_Init ...");
	dmLogInfo("DONE!");

	return 0;
}

int DefMcPlatform_Start(uint32_t sample_rate, uint32_t sample_delay)
{
	dmLogInfo("iOS DefMcPlatform_Start ... rate: %u, delay: %u", sample_rate, sample_delay);
	dmLogInfo("iOS DefMcPlatform_Start DONE");
	return 0;
}


int DefMcPlatform_Stop()
{
	dmLogInfo("iOS DefMcPlatform_Stop ...");
	dmLogInfo("iOS DefMcPlatform_Stop DONE");
	return 0;
}

int DefMcPlatform_SampleAmplitude(float &amp)
{
	dmLogInfo("iOS DefMcPlatform_SampleAmplitude ...");
	dmLogInfo("iOS DefMcPlatform_SampleAmplitude DONE");
	return 0;
}

#endif















