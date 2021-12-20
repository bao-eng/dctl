set(SOURCES main.cpp snake.cpp snake.h game.cpp game.h dctl_common.h dctl_generated.h)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
