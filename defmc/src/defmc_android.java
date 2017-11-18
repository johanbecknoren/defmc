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

	private static final int sampleRate = 8000;
	private static double lastLevel = 0;
	private static final int SAMPLE_DELAY = 75;
	private static AudioRecord audio;
	private static Thread thread;
	private static int bufferSize;
	private static short buffer[];

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

	public static void StartRecorder(final Activity activity) {
		android.util.Log.v(TAG, "################### StartRecorder");

		if (thread != null) {
			return;
		}

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

		audio.startRecording();
		android.util.Log.v(TAG, "DONE");

		thread = new Thread(new Runnable() {
			public void run() {
				while(thread != null && !thread.isInterrupted()) {
					try {
						Thread.sleep(SAMPLE_DELAY);
					} catch(InterruptedException ie) {
						ie.printStackTrace();
						break;
					}
					try {
						int bufferReadResult = 1;

						if (audio != null) {
							bufferReadResult = audio.read(buffer, 0, bufferSize);
							double sumLevel = 0;
							for (int i = 0; i < bufferReadResult; i++) {
								sumLevel += buffer[i];
							}
							lastLevel = Math.abs((sumLevel / bufferReadResult));
							android.util.Log.v(TAG, "lastLevel: " + lastLevel);
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
		thread.interrupt();
		thread = null;
		try {
			if (audio != null) {
				audio.stop();
				audio.release();
				audio = null;
			}
		} catch (Exception e) {e.printStackTrace();}
	}

	public static double SampleAmplitude(final Activity activity) {
		return lastLevel;
	}
}