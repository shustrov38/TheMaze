set(LOCAL_GAME_SOURCES
        game.c
        game.h
        unrefatored_maze_generator.c
)

#add_compile_definitions(__DFS_DEBUG__)

add_executable(Local_game game.c ${LOCAL_GAME_SOURCES})
target_link_libraries(Local_game gdi32 ws2_32)