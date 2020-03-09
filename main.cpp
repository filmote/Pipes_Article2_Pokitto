#include "Pokitto.h"

#include "src/Game.h"
#include "src/utils/GameCookie.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;


Game game;

int main() {


    // Initialise pokitto ..

    PC::begin();
    PD::persistence = true;
    PD::setInvisibleColor(0);
    PD::setFont(font3x5);
    PD::setColor(0);


    // Play game!

    game.setup();

    while (PC::isRunning()) {
    
        if (!PC::update()) continue;
    
        game.loop();
    
    }
    
    return 0;

}