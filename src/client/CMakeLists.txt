set(CLIENT_SOURCES
        client_server_interface/interface.c
        client_server_interface/interface.h
)

add_executable(Client client.c ${CLIENT_SOURCES})
target_link_libraries(Client gdi32 ws2_32)