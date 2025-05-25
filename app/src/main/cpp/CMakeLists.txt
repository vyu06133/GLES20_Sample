cmake_minimum_required(VERSION 3.22.1)

project("gles20_sample")

# C++17を使用
set(CMAKE_CXX_STANDARD 17)

# glmを含める（プロジェクトのcpp/glmにヘッダを置く場合）
include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/glm)

add_library(
        native-lib
        SHARED
        native-lib.cpp
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
