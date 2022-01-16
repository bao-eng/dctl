set(SOURCES dctl_client.cpp
            game.cpp
            game.h
            udp_client.h
            udp_client.cpp
            ./dctl-common/src/dctl_common.h
            ./dctl-common/src/dctl_common.cpp
            ./dctl-common/src/dctl_raylib.h
            ./dctl-common/src/dctl_raylib.cpp
            ./raygui/src/raygui.h
            ./dctl-common/src/dctl_message_generated.h)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
