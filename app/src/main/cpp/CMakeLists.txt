cmake_minimum_required(VERSION 3.22.1)

project("gles20_sample")

# C++17を使用
set(CMAKE_CXX_STANDARD 17)

# glmを含める（プロジェクトのcpp/glmにヘッダを置く場合）
# CMakeLists.txt が app/src/main/cpp/ にある場合
# CMAKE_CURRENT_SOURCE_DIR は app/src/main/cpp/ を指す
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)

add_compile_options(-fexceptions)

add_library(
        native-lib
        SHARED
        native-lib.cpp
        TaskBase.cpp
        TaskSystem.cpp
)

find_library(
        log-lib
        log
)

target_link_libraries(
        native-lib
        GLESv2
        ${log-lib}
)
