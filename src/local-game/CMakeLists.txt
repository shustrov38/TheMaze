set(LOCAL_GAME_SOURCES
        playerInfo.c
        playerInfo.h
        maze_generator.c
        maze_generator.h
        utilities.c
        utilities.h
        ../client/client_server_interface/interface.c
        ../client/client_server_interface/interface.h
)

#add_compile_definitions(__DFS_DEBUG__)

add_executable(MyGame game.c game.h ${LOCAL_GAME_SOURCES})
target_link_libraries(MyGame gdi32 ws2_32 SDL2main SDL2_image SDL2_ttf SDL2)