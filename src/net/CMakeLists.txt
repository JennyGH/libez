project(eznet)

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
GROUP_FILES(${INCLUDE_FILES} "Header Files")
GROUP_FILES(${SOURCE_FILES}  "Source Files")

# 指定当前子工程是编译为静态库文件
add_library(
    ${PROJECT_NAME}
    STATIC
    ${INCLUDE_FILES}
    ${SOURCE_FILES}
)

# 当前子工程要包含的头文件路径
target_include_directories(
    ${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
    PRIVATE ${CMAKE_SOURCE_DIR}/src
)

# 当前子工程要链接的库名称
set(EX_LINK_LIBRARIES )
if(MSVC)
    set(EX_LINK_LIBRARIES ${EX_LINK_LIBRARIES} ws2_32)
endif() # endif MSVC
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE ezbase
    PRIVATE ${EX_LINK_LIBRARIES}
)

# 配置预编译头:
target_precompile_headers(
  ${PROJECT_NAME}
  PRIVATE
  "$<$<COMPILE_LANGUAGE:CXX>:${PROJECT_SOURCE_DIR}/pch.h>"
)

install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)