cmake_minimum_required(VERSION 3.0)

project(Processing_util)

# source files ...
file(GLOB PROJECT_SRC_LIST
${PROJECT_NAME} ./src/*.c
${PROJECT_NAME} ./src/*.h

${PROJECT_NAME} ./*.c
${PROJECT_NAME} ./*.h
)

include_directories("./inc")

add_executable(${PROJECT_NAME} ${PROJECT_SRC_LIST})