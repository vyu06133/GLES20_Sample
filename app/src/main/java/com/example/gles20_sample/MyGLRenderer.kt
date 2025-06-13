package com.example.gles20_sample

import android.util.Log
import android.content.res.AssetManager;
import android.opengl.GLES20
import android.opengl.GLSurfaceView
import android.opengl.Matrix

class MyGLRenderer : GLSurfaceView.Renderer {
	
	// JNI関数（native-lib.cppに対応）
	external fun nativeInit()
	external fun nativeTouchEvent(action:Int, x:Int, y:Int)
	external fun nativeSurfaceChanged(width: Int, height: Int)
	external fun nativeDrawFrame()
	external fun nativePassAssetManager(assetManager: AssetManager)
	
	// 行列関連
	private val projectionMatrix = FloatArray(16)
	private val viewMatrix = FloatArray(16)
	private val modelMatrix = FloatArray(16)
	private val mvpMatrix = FloatArray(16)
	
	// 回転角（タッチ操作で更新）
	var angle = 0f
	
	override fun onSurfaceCreated(unused: javax.microedition.khronos.opengles.GL10?, config: javax.microedition.khronos.egl.EGLConfig?) {
		// GLES20 初期化（native関数呼び出し）
		nativeInit()
		GLES20.glClearColor(0.2f, 0.3f, 0.4f, 1.0f)
	}
	
	override fun onSurfaceChanged(unused: javax.microedition.khronos.opengles.GL10?, width: Int, height: Int) {
		GLES20.glViewport(0, 0, width, height)
		
		val aspectRatio = width.toFloat() / height
		Matrix.frustumM(projectionMatrix, 0, -aspectRatio, aspectRatio, -1f, 1f, 2f, 10f)
		nativeSurfaceChanged(width, height)
	}
	
	override fun onDrawFrame(unused: javax.microedition.khronos.opengles.GL10?) {
		nativeDrawFrame()
	}
	
	companion object {
		init {
			System.loadLibrary("native-lib")
		}
	}
}
