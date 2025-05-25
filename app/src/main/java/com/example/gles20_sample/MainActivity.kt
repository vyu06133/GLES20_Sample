package com.example.gles20_sample

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity

class MainActivity : AppCompatActivity() {
	private lateinit var glView: MyGLSurfaceView
	
	override fun onCreate(savedInstanceState: Bundle?) {
		super.onCreate(savedInstanceState)
		glView = MyGLSurfaceView(this)
		setContentView(glView)
	}
}
