project(unit_test_for_path)

file(GLOB_RECURSE INCLUDE_FILES
    ${PROJECT_SOURCE_DIR}/*.h
    ${PROJECT_SOURCE_DIR}/*.hpp
)

file(GLOB_RECURSE SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/*.c
    ${PROJECT_SOURCE_DIR}/*.cpp
)

source_group("Header Files" FILES "${INCLUDE_FILES}")
source_group("Source Files" FILES "${SOURCE_FILES}")

add_executable(
    ${PROJECT_NAME}
    ${INCLUDE_FILES}
    ${SOURCE_FILES}
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/src
)

if(UNIX)
    set(EX_LINK_LIBRARIES pthread)
endif() # endif UNIX
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE ezbase
    PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main
    PRIVATE ${EX_LINK_LIBRARIES}
)
