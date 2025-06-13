#include "framework.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>  // ← これが AAssetManager_fromJava() を定義
#include "Test.h"
Test test;

extern "C" JNIEXPORT void JNICALL
Java_com_example_gles20_1sample_MyGLRenderer_nativePassAssetManager(JNIEnv* env, jobject thiz, jobject javaAssetManager)
//MyGLRenderer.nativePassAssetManager(JNIEnv* env, jobject thiz, jobject javaAssetManager)
{
	AAssetManager* mgr = AAssetManager_fromJava(env, javaAssetManager);

	test.assetManager = mgr;
//
//	AAsset* asset = AAssetManager_open(assetManager, "file.png", AASSET_MODE_STREAMING);
//
//	if (asset) {
//		// ここで読み込み処理をする
//		AAsset_close(asset);
//	}
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_gles20_1sample_MyGLRenderer_nativeInit(JNIEnv *, jobject) {
	test.Init();
	test.Setup();
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_gles20_1sample_MyGLRenderer_nativeTouchEvent( JNIEnv* env, jobject MyGLRenderer, jint action, jint x, jint y)
{
	LOGD("C++ 0x%x %d,%d\n",(unsigned int)action,x,y);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_gles20_1sample_MyGLRenderer_nativeSurfaceChanged(JNIEnv *env, jobject MyGLRenderer, int width, int height)
{
	test.SurfaceChanged(width, height);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_gles20_1sample_MyGLRenderer_nativeDrawFrame(JNIEnv *env, jobject MyGLRenderer)
{
	test.DrawFrame();
}
