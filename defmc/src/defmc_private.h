#pragma once

#include <dmsdk/sdk.h>

extern int DefMcPlatform_Init();
extern int DefMcPlatform_Start(uint32_t sample_rate, uint32_t sample_delay, float lowpass_alpha);
extern int DefMcPlatform_Stop();
extern int DefMcPlatform_SampleAmplitude(float &amp);