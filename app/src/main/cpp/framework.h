//
// Created by vyu06133 on 2025/05/25 日.
//

#pragma once

#include <algorithm> // for sort
#include <cmath>
#include <cstddef> // offsetof のために必要
#include <cstdint>
#include <cstdio>
#include <cstdarg> // for va_list
#include <cassert>
#include <cstring>
#include <exception>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <system_error>
#include <vector>
#include <array>
#include <queue>
#include <stack>
#include <list>
#include <map>
#include <unordered_map>
#include <functional>

#include <jni.h> // JNI の基本ヘッダー
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>

#include <GLES3/gl3.h>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp> // lookAt を使うために必要
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/scalar_relational.hpp> // glm::sum() を使用するために必要
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::quat;
using glm::mat3;
using glm::mat4;
using GlmVariant = std::variant<int, float, vec2, vec3, vec4, quat, mat3, mat4>;

struct Rect
{
	int    left;
	int    top;
	int    right;
	int    bottom;
	int Width() const
	{
		return right-left;
	}
	int Height() const
	{
		return bottom-top;
	}
};
#define LOG_TAG "GLES20_Sample"

// 各ログレベルに対応するマクロを定義
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL, LOG_TAG, __VA_ARGS__)

#include "MyMath.h"
