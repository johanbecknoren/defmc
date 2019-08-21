// Good start; https://stackoverflow.com/questions/1010343/how-do-i-record-audio-on-iphone-with-avaudiorecorder
// Even goodier start; https://stackoverflow.com/questions/31230854/ios-detect-blow-into-mic-and-convert-the-results-swift

#include <dmsdk/sdk.h>
#include "defmc_private.h"

#if defined(DM_PLATFORM_IOS)
#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreAudio/CoreAudioTypes.h>

@interface AmplitudeViewController : UIViewController {
	AVAudioRecorder *recorder;
	NSTimer *levelTimer;
}

- (void)levelTimerCallback:(NSTimer *)timer;
- (void)startRecord;
- (void)stopRecord;

@end

float lowpass_result;
float lp_alpha;

void initState() {
	lowpass_result = 0.0f;
	lp_alpha = 0.8f;
}

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

	AVAudioSession *session = [AVAudioSession sharedInstance];
	[session setCategory:AVAudioSessionCategoryPlayAndRecord error:nil];
	[session setActive:YES error:nil];

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

- (void)levelTimerCallback:(NSTimer *)timer {
	[recorder updateMeters];
	//dmLogInfo("Metering enabled?: %i", recorder.meteringEnabled);
	double peakPowerForChannel = pow(10, (0.05 * [recorder peakPowerForChannel:0]));
	lowpass_result = lp_alpha * peakPowerForChannel + (1.0 - lp_alpha) * lowpass_result;
	NSLog(@"Average input: %f Peak input: %f Low pass results: %f", [recorder averagePowerForChannel:0], [recorder peakPowerForChannel:0], lowpass_result);
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
	dmLogInfo("iOS DefMcPlatform_Init ...");
	dmLogInfo("DONE!");

	return 0;
}

int DefMcPlatform_Start(uint32_t sample_rate, uint32_t sample_delay, float lowpass_alpha)
{
	dmLogInfo("iOS DefMcPlatform_Start ... rate: %u, delay: %u, lowpass_alpha: %f", sample_rate, sample_delay, lowpass_alpha);
	initState();
	lp_alpha = lowpass_alpha;
	controller = [[AmplitudeViewController alloc] init];
	[controller startRecord];
	dmLogInfo("iOS DefMcPlatform_Start DONE");
	return 0;
}


int DefMcPlatform_Stop()
{
	dmLogInfo("iOS DefMcPlatform_Stop ...");
	if (controller) {
		[controller stopRecord];
		[controller release];
		controller = nil;
	}
	dmLogInfo("iOS DefMcPlatform_Stop DONE");
	return 0;
}

int DefMcPlatform_SampleAmplitude(float &amp)
{
	//dmLogInfo("iOS DefMcPlatform_SampleAmplitude ...");
	//dmLogInfo("mm lowpass_result: %f", lowpass_result);
	amp = lowpass_result * 100.0f;
	//dmLogInfo("iOS DefMcPlatform_SampleAmplitude DONE");
	return 0;
}

#endif
