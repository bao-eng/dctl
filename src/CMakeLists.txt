set(SOURCES dctl_client.cpp snake.cpp snake.h game.cpp game.h dctl_common.h dctl_state_generated.h dctl_input_generated.h)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
