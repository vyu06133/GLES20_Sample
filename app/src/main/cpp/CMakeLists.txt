cmake_minimum_required(VERSION 3.22.1)

project("gles20_sample")

# C++17を使用
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ANDROID_PLATFORM android-18) # または build.gradle の minSdk と同じ値

set(ANDROID_STL c++_static)

# glmを含める（プロジェクトのcpp/glmにヘッダを置く場合）
# CMakeLists.txt が app/src/main/cpp/ にある場合
# CMAKE_CURRENT_SOURCE_DIR は app/src/main/cpp/ を指す
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/
        ${CMAKE_CURRENT_SOURCE_DIR}/stb/
)

add_compile_options(-fexceptions)

add_library(
        native-lib
        SHARED
        native-lib.cpp
        TaskBase.cpp
        TaskBase.h
        TaskSystem.cpp
        TaskSystem.h
        framework.h
        MyMath.h
        Shader.h
        Vertex.h
        Texture2D.cpp
        Texture2D.h
        Test.h
        stb/stb_include.h
)

find_library(log-lib log)

find_library(android-lib android)

target_link_libraries(
        native-lib
        GLESv3
        ${log-lib}
        ${android-lib}
)
