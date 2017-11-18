package com.defold.android.defmc;

import android.media.AudioFormat;
import android.media.AudioRecord;
import android.util.Log;

class DefMcExtension {
	private final static String TAG = "defmc";

	private static final int sampleRate = 8000;
	private AudioRecord audio;
	private int bufferSize;

	public void InitRecorder() {
		// android.util.Log.v(TAG, "################### InitRecorder");
		// try {
		// 	bufferSize = AudioRecord.genMinBufferSize(
		// 		sampleRate, 
		// 		AudioFormat.CHANNEL_IN_MONO,
		// 		AudioFormat.ENCODING_PCM_16BIT);
		// } catch (Exception e) {
		// 	android.util.Log.e(TAG, "Exception", e);
		// }
	}
}