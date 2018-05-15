#include <dmsdk/sdk.h>
#include "defmc_private.h"

#if defined(DM_PLATFORM_OSX)
#import <AppKit/AppKit.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreAudio/CoreAudioTypes.h>

@interface AmplitudeViewController : NSViewController {
	AVAudioRecorder *recorder;
	NSTimer *levelTimer;
}

- (void)levelTimerCallback:(NSTimer *)timer;
- (void)startRecord;
- (void)stopRecord;

@end

@implementation AmplitudeViewController

- (void)startRecord {
	NSURL *url = [NSURL fileURLWithPath:@"/dev/null"];

	NSDictionary *settings = [NSDictionary dictionaryWithObjectsAndKeys:
	[NSNumber numberWithFloat: 44100.0],                 AVSampleRateKey,
	[NSNumber numberWithInt: kAudioFormatAppleLossless], AVFormatIDKey,
	[NSNumber numberWithInt: 1],                         AVNumberOfChannelsKey,
	[NSNumber numberWithInt: AVAudioQualityMax],         AVEncoderAudioQualityKey,
	nil];

	NSError *error;

	recorder = [[AVAudioRecorder alloc] initWithURL:url settings:settings error:&error];

	if (recorder) {
		[recorder prepareToRecord];
		recorder.meteringEnabled = YES;
		[recorder record];
		levelTimer = [NSTimer scheduledTimerWithTimeInterval: 0.03 target: self selector: @selector(levelTimerCallback:) userInfo: nil repeats: YES];
	} else
	NSLog([error description]);	
}

- (void) stopRecord {
	if (levelTimer) {
		[levelTimer invalidate];
		levelTimer = nil;
	}
}

float lowpass_result = 0.0f;
float lp_alpha = 0.05f;

- (void)levelTimerCallback:(NSTimer *)timer {
	[recorder updateMeters];
	double peakPowerForChannel = pow(10, (0.04 * [recorder peakPowerForChannel:0]));
	lowpass_result = lp_alpha * peakPowerForChannel + (1.0 - lp_alpha) * lowpass_result;
}

- (void)dealloc {
	[levelTimer release];
	[recorder release];
	[super dealloc];
}

@end

AmplitudeViewController* controller;

int DefMcPlatform_Init()
{
	dmLogInfo("OSX DefMcPlatform_Init ...");
	dmLogInfo("DONE!");
	return 0;
}

int DefMcPlatform_Start(uint32_t sample_rate, uint32_t sample_delay, float lowpass_alpha)
{
	dmLogInfo("OSX DefMcPlatform_Start ... rate: %u, delay: %u, alpha: %f", sample_rate, sample_delay, lowpass_alpha);
	lp_alpha = lowpass_alpha;
	controller = [[AmplitudeViewController alloc] init];
	[controller startRecord];
	dmLogInfo("OSX DefMcPlatform_Start DONE");
	return 0;
}

int DefMcPlatform_Stop()
{
	dmLogInfo("OSX DefMcPlatform_Stop ...");
	if (controller) {
		[controller stopRecord];
		[controller release];
		controller = nil;
	}
	dmLogInfo("OSX DefMcPlatform_Stop DONE");
	return 0;
}

int DefMcPlatform_SampleAmplitude(float &amp)
{
	amp = lowpass_result * 10.0f;
	return 0;
}

#endif