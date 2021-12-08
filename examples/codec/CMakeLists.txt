project(codec)

# 递归地查找头文件，并存放到 INCLUDE_FILES 变量中
file(GLOB_RECURSE INCLUDE_FILES
    ${PROJECT_SOURCE_DIR}/*.h
    ${PROJECT_SOURCE_DIR}/*.hpp
)

# 递归地查找 .c/.cpp 文件，并存放到 SOURCE_FILES 变量中
file(GLOB_RECURSE SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/*.c
    ${PROJECT_SOURCE_DIR}/*.cpp
)

# 将文件分组
source_group("Header Files" FILES "${INCLUDE_FILES}")
source_group("Source Files" FILES "${SOURCE_FILES}")

# 指定当前子工程是编译为可执行文件
add_executable(
    ${PROJECT_NAME}
    ${INCLUDE_FILES}
    ${SOURCE_FILES}
)

# 当前子工程要包含的头文件路径
include_directories(
    ${CMAKE_SOURCE_DIR}/src
)

# 当前子工程要链接的库名称
target_link_libraries(
    ${PROJECT_NAME}
    ezbase
)

# 配置预编译头:
target_precompile_headers(
  ${PROJECT_NAME}
  PRIVATE
  "$<$<COMPILE_LANGUAGE:CXX>:${PROJECT_SOURCE_DIR}/pch.h>"
)