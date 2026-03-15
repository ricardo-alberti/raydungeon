#include "math.h"
#include "stdlib.h"
#include "stdio.h"
#include "raylib.h" 

#include "game.h"

#include "arena.c"
#include "level_up_menu.c"
#include "game.c"

int
main(void) 
{ 
    GameMemory gameMemory = { 0 };

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raydungeon");
    setup_game(&gameMemory);

    while (!WindowShouldClose())
    {
        update_game(&gameMemory);
        render_game(&gameMemory);
    }

    CloseWindow();        

    free(gameMemory.permanentStorage.base);
    free(gameMemory.transientStorage.base);
    return 0;
}
