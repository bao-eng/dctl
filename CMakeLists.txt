cmake_minimum_required(VERSION 3.11)
project(dctl-client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

INCLUDE_DIRECTORIES(
  ${PROJECT_SOURCE_DIR}
)

set(SOURCES main.cpp snake.cpp snake.h game.cpp game.h dctl_common.h dctl_generated.h)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
