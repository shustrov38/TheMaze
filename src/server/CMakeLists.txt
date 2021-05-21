set(SERVER_SOURCES
        MazeGenerator.c
        MazeGenerator.h
)

add_executable(Server server.c ${SERVER_SOURCES})
target_link_libraries(Server sqlite3 ws2_32)