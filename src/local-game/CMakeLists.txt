set(LOCAL_GAME_SOURCES
        game.c
        game.h
        maze_generator.c
        maze_generator.h
        utilities.h)

#add_compile_definitions(__DFS_DEBUG__)

add_executable(Local_game game.c ${LOCAL_GAME_SOURCES})
target_link_libraries(Local_game gdi32 ws2_32)


add_executable(MyGame game.c game.h playerInfo.c playerInfo.h maze_generator.c maze_generator.h utilities.c utilities.h)
target_link_libraries(MyGame SDL2main SDL2_image SDL2_ttf SDL2)