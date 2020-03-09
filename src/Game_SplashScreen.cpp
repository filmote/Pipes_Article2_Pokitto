#include "Game.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;



/* ----------------------------------------------------------------------------
 *  Initialise spalsch screen counter (used to delay the heading) ..
 */
void Game::initSplash() { 

  splash_counter = 100;
  gameState = STATE_SPLASH_RENDER;

}


/* ----------------------------------------------------------------------------
 *  Render the splash screen ..
 */
void Game::renderSplash() { 

  if (splash_counter > 0) { splash_counter--; }
    
  renderBackground();

  if (splash_counter == 0) {

    PD::setColor(0);
    PD::fillRect(9, 25, 92, 37);
    PD::drawBitmap(10, 26, Heading);

  }

  if (PC::buttons.pressed(BTN_A)) {
    gameState = STATE_INIT_GAME;
  }
  
}


