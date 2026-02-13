#include "game.h"

int main(void) {
    Game_Init();

    while (!Game_ShouldExit()) {
        Game_RunFrame();
    }

    Game_Shutdown();
    return 0;
}
