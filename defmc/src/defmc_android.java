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
	private static AudioRecord audio;
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

		activity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				
			}
		});
	}
}