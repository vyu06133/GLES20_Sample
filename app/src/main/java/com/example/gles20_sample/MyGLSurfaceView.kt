package com.example.gles20_sample

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.Log
import android.view.MotionEvent

class MyGLSurfaceView(context: Context) : GLSurfaceView(context) {
	private val renderer: MyGLRenderer
	
	private var previousX = 0f
	private var previousY = 0f
	
	init {
		setEGLContextClientVersion(3)
		renderer = MyGLRenderer()
		renderer.nativePassAssetManager(context.getAssets())
		setRenderer(renderer)
		renderMode = RENDERMODE_CONTINUOUSLY
	}
	
	override fun onTouchEvent(event: MotionEvent): Boolean {
		val x = event.x
		val y = event.y
		
		// as IntではFATAL EXCEPTION
		// toInt()で通る
		renderer.nativeTouchEvent(event.action.toInt(), x.toInt(), y.toInt())
		
		if (event.action == MotionEvent.ACTION_MOVE) {
			val dx = x - previousX
			val dy = y - previousY
			
			renderer.angle += dx * 0.5f
		}
		
		previousX = x
		previousY = y
		return true
	}
}
