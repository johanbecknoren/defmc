#include <dmsdk/sdk.h>
#include "defmc_private.h"

#if defined(DM_PLATFORM_ANDROID)

static JNIEnv* Attach()
{
	JNIEnv* env;
	JavaVM* vm = dmGraphics::GetNativeAndroidJavaVM();
	vm->AttachCurrentThread(&env, NULL);
	return env;
}

static bool Detach(JNIEnv* env)
{
	bool exception = (bool) env->ExceptionCheck();
	env->ExceptionClear();
	JavaVM* vm = dmGraphics::GetNativeAndroidJavaVM();
	vm->DetachCurrentThread();
	return !exception;
}

namespace {
	struct AttachScope
	{
		JNIEnv* m_Env;
		AttachScope() : m_Env(Attach())
		{
		}
		~AttachScope()
		{
			Detach(m_Env);
		}
	};
}

static jclass GetClass(JNIEnv* env, const char* classname)
{
	jclass activity_class = env->FindClass("android/app/NativeActivity");
	jmethodID get_class_loader = env->GetMethodID(activity_class,"getClassLoader", "()Ljava/lang/ClassLoader;");
	jobject cls = env->CallObjectMethod(dmGraphics::GetNativeAndroidActivity(), get_class_loader);
	jclass class_loader = env->FindClass("java/lang/ClassLoader");
	jmethodID find_class = env->GetMethodID(class_loader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");

	jstring str_class_name = env->NewStringUTF(classname);
	jclass outcls = (jclass)env->CallObjectMethod(cls, find_class, str_class_name);
	env->DeleteLocalRef(str_class_name);
	return outcls;
}

int DefMcPlatform_Init()
{
	dmLogInfo("ANDROID DefMcPlatform_Init ...");
	dmLogInfo("DONE!");

	return 0;
}

int DefMcPlatform_Start(uint32_t sample_rate, uint32_t sample_delay, float lowpass_alpha)
{
	dmLogInfo("ANDROID DefMcPlatform_Start ... rate: %u, delay: %u", sample_rate, sample_delay);

	// prepare JNI
	AttachScope attachscope;
	JNIEnv* env = attachscope.m_Env;
	jclass cls = GetClass(env, "com.defold.android.defmc.DefMcExtension");

	// get buffersize
	jmethodID get_buf_size = env->GetStaticMethodID(cls, "GetMinBufferSize", "(I)I");
	uint32_t buf_len = (uint32_t)env->CallStaticIntMethod(cls, get_buf_size, (int)sample_rate);
	dmLogInfo("buf_len: %u", buf_len);

	// start recording
	jmethodID start = env->GetStaticMethodID(cls, "StartRecorder", "(IIF)V");
	env->CallStaticVoidMethod(cls, start, (int)sample_rate, (int)sample_delay, lowpass_alpha);

	dmLogInfo("ANDROID DefMcPlatform_Start DONE");
	return 0;
}


int DefMcPlatform_Stop()
{
	dmLogInfo("ANDROID DefMcPlatform_Stop ...");
	// prepare JNI
	AttachScope attachscope;
	JNIEnv* env = attachscope.m_Env;
	jclass cls = GetClass(env, "com.defold.android.defmc.DefMcExtension");

	// call method
	dmLogInfo("GetStaticMethodID ...");
	jmethodID start = env->GetStaticMethodID(cls, "StopRecorder", "(Landroid/app/Activity;)V");
	dmLogInfo("DONE!");
	dmLogInfo("CallStaticVoidMethod ...");
	env->CallStaticVoidMethod(cls, start, dmGraphics::GetNativeAndroidActivity());
	dmLogInfo("DONE!");

	dmLogInfo("ANDROID DefMcPlatform_Stop DONE");
	return 0;
}

int DefMcPlatform_SampleAmplitude(float &amp)
{
	// prepare JNI
	AttachScope attachscope;
	JNIEnv* env = attachscope.m_Env;
	jclass cls = GetClass(env, "com.defold.android.defmc.DefMcExtension");

	// get lowpass filtered amplitude
	jmethodID start = env->GetStaticMethodID(cls, "SampleAmplitude", "(Landroid/app/Activity;)D");
	amp = (float)env->CallStaticDoubleMethod(cls, start, dmGraphics::GetNativeAndroidActivity());

	return 0;
}

#endif















