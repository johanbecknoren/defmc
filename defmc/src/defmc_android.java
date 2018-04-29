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
	private static double lastLevel = 11;
	private static int sampleDelay = 75;
	private static AudioRecord audio;
	private static Thread thread;
	private static int bufferSize = 0;
	private static short buffer[];
	private static short buffer_filtered[];
	private static Boolean stopping = false;
	private static float alpha = 0.5f;

	public static void InitRecorder() { }

	public static int GetMinBufferSize(int sample_rate) {
		try {
			bufferSize = AudioRecord.getMinBufferSize(
				sample_rate, 
				AudioFormat.CHANNEL_IN_MONO,
				AudioFormat.ENCODING_PCM_16BIT);
		} catch (Exception e) {
			android.util.Log.e(TAG, "Exception", e);
		}

		return bufferSize;
	}

	public static void StartRecorder(int sample_rate, int sample_delay, float lp_alpha) {
		android.util.Log.v(TAG, "################### StartRecorder, sample_rate: "+sample_rate+", sample_delay: "+sample_delay);

		if (thread != null) {
			return;
		}

		sampleRate = sample_rate;
		sampleDelay = sample_delay;
		alpha = lp_alpha;
		stopping = false;

		buffer = new short[bufferSize];
		buffer_filtered = new short[bufferSize];
		for(int i=0; i < bufferSize; ++i) {
			buffer[i] = 0;
			buffer_filtered[i] = 0;
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
						int buffer_read_result = 1;
						if (audio != null) {
							buffer_read_result = audio.read(buffer, 0, bufferSize);
							double sum_level = 0;
							for (int i = 1; i < buffer_read_result; i++) {
								buffer_filtered[i] *= (short) (1.0 - alpha);
								buffer_filtered[i] += (short)(alpha * (float)(buffer[i] - buffer_filtered[i]));
								sum_level += buffer_filtered[i];
							}
							lastLevel = Math.abs((sum_level / buffer_read_result));
						}

					} catch (Exception e) {
						e.printStackTrace();
					}
				}
			}
		});
		thread.start();
	}

	public static void GetAudioBuffer(short[] nativeBuf) {
		android.util.Log.v(TAG, "##################### GetAudioBuffer");
	}

	public static void StopRecorder(final Activity activity) {
		stopping = true;
	}

	public static double SampleAmplitude(final Activity activity) {
		return lastLevel;
	}
}