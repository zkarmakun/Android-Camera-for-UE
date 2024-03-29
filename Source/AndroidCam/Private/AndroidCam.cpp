// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AndroidCam.h"

#if PLATFORM_ANDROID
#include "../../../Core/Public/Android/AndroidApplication.h"
#include "../../../Launch/Public/Android/AndroidJNI.h"
#include <android/log.h>

#define LOG_TAG "CameraLOG"

int SetupJNICamera(JNIEnv* env);
JNIEnv* ENV = NULL;
static jmethodID jToast;
static jmethodID AndroidThunkJava_startCamera;
static jmethodID AndroidThunkJava_stopCamera;
static bool newFrame = false;
static char* rawDataAndroid;
#endif

#define LOCTEXT_NAMESPACE "FAndroidCamModule"

void FAndroidCamModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
#if PLATFORM_ANDROID
	JNIEnv* env = FAndroidApplication::GetJavaEnv();
	SetupJNICamera(env);
#endif
}

void FAndroidCamModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAndroidCamModule, AndroidCam)

#if PLATFORM_ANDROID
int SetupJNICamera(JNIEnv* env)
{
	if (!env) return JNI_ERR;

	ENV = env;

	AndroidThunkJava_startCamera = FJavaWrapper::FindMethod(ENV, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_startCamera", "()V", false);
	if (!AndroidThunkJava_startCamera)
	{
		__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "ERROR: AndroidThunkJava_startCamera Method cant be found T_T ");
		return JNI_ERR;
	}

	AndroidThunkJava_stopCamera = FJavaWrapper::FindMethod(ENV, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_stopCamera", "()V", false);
	if (!AndroidThunkJava_stopCamera)
	{
		__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "ERROR: AndroidThunkJava_stopCamera Method cant be found T_T ");
		return JNI_ERR;
	}

	//FJavaWrapper::CallVoidMethod(ENV, FJavaWrapper::GameActivityThis, jToast);
	__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "module load success!!! ^_^");

	return JNI_OK;
}

void  AndroidThunkCpp_startCamera()
{
	if (!AndroidThunkJava_startCamera || !ENV) return;
	FJavaWrapper::CallVoidMethod(ENV, FJavaWrapper::GameActivityThis, AndroidThunkJava_startCamera);
	__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "start camera");
}

void AndroidThunkCpp_stopCamera()
{
	if (!AndroidThunkJava_stopCamera || !ENV) return;
	FJavaWrapper::CallVoidMethod(ENV, FJavaWrapper::GameActivityThis, AndroidThunkJava_stopCamera);
	__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "shutdown camera");
}

extern "C" bool Java_com_epicgames_ue4_GameActivity_nativeGetFrameData(JNIEnv* LocalJNIEnv, jobject LocalThiz, jint frameWidth, jint frameHeight, jbyteArray data)
{
	//get the new frame
	int length = LocalJNIEnv->GetArrayLength(data);
	char* buffer = new char[length];
	LocalJNIEnv->GetByteArrayRegion(data, 0, length, reinterpret_cast<jbyte*>(buffer));
	rawDataAndroid = buffer;
	newFrame = true;
	return JNI_TRUE;
}

#endif