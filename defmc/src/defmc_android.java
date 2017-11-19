package com.defold.android.defmc;

import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.util.Log;
import android.app.Activity;
import android.content.Context;
import java.lang.Runnable;

class DefMcExtension {
	private final static String TAG = "defmc";

	private static int sampleRate = 8000;
	private static double lastLevel = 0;
	private static int sampleDelay = 75;
	private static AudioRecord audio;
	private static Thread thread;
	private static int bufferSize;
	private static short buffer[];
	private static Boolean stopping = false;

	public static void InitRecorder() {
		// android.util.Log.v(TAG, "################### InitRecorder");
		// try {
		// 	bufferSize = AudioRecord.getMinBufferSize(
		// 		sampleRate, 
		// 		AudioFormat.CHANNEL_IN_MONO,
		// 		AudioFormat.ENCODING_PCM_16BIT);
		// 	buffer = new short[bufferSize];
		// } catch (Exception e) {
		// 	android.util.Log.e(TAG, "Exception", e);
		// }
	}

	//public static void StartRecorder(final Activity activity) {
	public static void StartRecorder(int sample_rate, int sample_delay) {
		android.util.Log.v(TAG, "################### StartRecorder, sample_rate: "+sample_rate+", sample_delay: "+sample_delay);

		if (thread != null) {
			return;
		}

		sampleRate = sample_rate;
		sampleDelay = sample_delay;
		stopping = false;

		try {
			bufferSize = AudioRecord.getMinBufferSize(
				sampleRate, 
				AudioFormat.CHANNEL_IN_MONO,
				AudioFormat.ENCODING_PCM_16BIT);
			buffer = new short[bufferSize];
			android.util.Log.v(TAG, "bufferSize: " + bufferSize);
		} catch (Exception e) {
			android.util.Log.e(TAG, "Exception", e);
		}

		android.util.Log.v(TAG, "Allocating new AudioRecord...");
		audio = new AudioRecord(MediaRecorder.AudioSource.MIC, sampleRate,
				AudioFormat.CHANNEL_IN_MONO,
				AudioFormat.ENCODING_PCM_16BIT, bufferSize);
		android.util.Log.v(TAG, "DONE, starting recording...");

		thread = new Thread(new Runnable() {
			public void run() {
				audio.startRecording();
				android.util.Log.v(TAG, "DONE");
				while(thread != null && !thread.isInterrupted()) {
					if (stopping) {
						try {
							if (audio != null) {
								audio.stop();
								audio.release();
								audio = null;
							}
						} catch (Exception e) {e.printStackTrace();}
						thread.interrupt();
						thread = null;
						break;
					}

					try {Thread.sleep(sampleDelay);} catch(InterruptedException ie) {ie.printStackTrace();break;}
					try {
						int bufferReadResult = 1;
						if (audio != null) {
							bufferReadResult = audio.read(buffer, 0, bufferSize);
							double sumLevel = 0;
							for (int i = 0; i < bufferReadResult; i++) {
								sumLevel += buffer[i];
							}
							lastLevel = Math.abs((sumLevel / bufferReadResult));
							//android.util.Log.v(TAG, "lastLevel: " + lastLevel);
						}

					} catch (Exception e) {
						e.printStackTrace();
					}
				}
			}
		});
		thread.start();
	}

	public static void StopRecorder(final Activity activity) {
		android.util.Log.v(TAG, "################################## STOPPA DÅÅÅÅÅÅÅÅÅÅÅÅÅÅÅ");
		stopping = true;
	}

	public static double SampleAmplitude(final Activity activity) {
		return lastLevel;
	}
}