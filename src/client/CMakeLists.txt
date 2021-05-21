set(CLIENT_SOURCES
)

add_executable(Client client.c ${CLIENT_SOURCES})
target_link_libraries(Client gdi32 ws2_32)