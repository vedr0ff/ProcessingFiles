cmake_minimum_required(VERSION 3.0)

project(Test_Util)

# source files ...
file(GLOB PROJECT_SRC_LIST
${PROJECT_NAME} ./src/*.c
${PROJECT_NAME} ./src/*.h
${PROJECT_NAME} ./Processing_util/src/*.c
${PROJECT_NAME} ./Processing_util/src/*.h
)

add_subdirectory("./Processing_util")

include_directories("./Processing_util/inc")

add_executable(${PROJECT_NAME} ${PROJECT_SRC_LIST})

