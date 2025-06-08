#include <jni.h>
#include <android/log.h>
#include "framework.h"
#include "Test.h"
Test test;
GLuint program;
GLuint uMVPMatrixLocation;
GLuint positionHandle;

const char *vertexShaderCode =
		"attribute vec4 aPosition;\n"
		"uniform mat4 uMVPMatrix;\n"
		"void main() {\n"
		"  gl_Position = uMVPMatrix * aPosition;\n"
		"}";

const char *fragmentShaderCode =
		"precision mediump float;\n"
		"void main() {\n"
		"  gl_FragColor = vec4(1.0, 0.5, 0.0, 1.0);\n"
		"}";

GLfloat triangleVertices[] = {
		0.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f
};

GLuint loadShader(GLenum type, const char *shaderCode) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderCode, nullptr);
	glCompileShader(shader);
	return shader;
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_gles20_1sample_MyGLRenderer_nativeInit(JNIEnv *, jobject) {
	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderCode);
	GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderCode);

	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	uMVPMatrixLocation = glGetUniformLocation(program, "uMVPMatrix");
	positionHandle = glGetAttribLocation(program, "aPosition");
	test.Init();
	test.Setup();
}

#if 1
extern "C" JNIEXPORT void JNICALL
Java_com_example_gles20_1sample_MyGLRenderer_nativeTouchEvent( JNIEnv* env, jobject MyGLRenderer, jint action, jint x, jint y)
{
	LOGD("C++ 0x%x %d,%d\n",(unsigned int)action,x,y);
}
#endif

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

extern "C" JNIEXPORT void JNICALL
Java_com_example_gles20_1sample_MyGLRenderer_nativeRender(JNIEnv *env, jobject, jfloatArray mvpArray) {
	glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);

	jfloat *mvp = env->GetFloatArrayElements(mvpArray, nullptr);
	glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, mvp);
	env->ReleaseFloatArrayElements(mvpArray, mvp, JNI_ABORT);

	glVertexAttribPointer(positionHandle, 3, GL_FLOAT, GL_FALSE, 0, triangleVertices);
	glEnableVertexAttribArray(positionHandle);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(positionHandle);
}
