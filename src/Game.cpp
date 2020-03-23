#include "Game.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;



/* ----------------------------------------------------------------------------
 *   Get ready !
 */
void Game::setup() { 
    
  // Do nothing !

}


/* ----------------------------------------------------------------------------
 *   Play pipes !
 */
void Game::loop() {
    
  PC::buttons.pollButtons();
  PD::fillScreen(0);
  
  switch (gameState) {

    case STATE_SPLASH_INIT:
      initSplash();
      [[fallthrough]]

    case STATE_SPLASH_RENDER:
      renderSplash();
      break;

    case STATE_INIT_GAME:
      play_InitGame();
      [[fallthrough]]

    case STATE_NO_SELECTION:
      play_NoSelection();
      break;

    case STATE_NODE_SELECTED:
      play_NodeSelected();
      break;

    case STATE_GAME_OVER:
      play_GameOver();
      break;

  }

}


/* ----------------------------------------------------------------------------
 *   Initialise the game based on the selected level and puzzle numbers.
 */
void Game::play_InitGame() {

  puzzle.index = 0;
  puzzle.maximum.x = 5;
  puzzle.maximum.y = 5;

  initBoard(puzzle.index);
  gameState = STATE_NO_SELECTION;
  
}


/* ----------------------------------------------------------------------------
 *   Handle the game play before the user selects a node.
 *   
 *   The user can scroll to any node on the board using the directional arrows.
 *   Clicking the A button on a node selects it.  Clicking the B button returns
 *   the user to the level select (if currently on the first puzzle) or the 
 *   puzzle select screen (if they are currently working on a puzzle).
 */
void Game::play_NoSelection() {

  if (PC::buttons.pressed(BTN_LEFT) && player.highlightedNode.x > 0)                         { player.highlightedNode.x--; }
  if (PC::buttons.pressed(BTN_RIGHT) && player.highlightedNode.x < puzzle.maximum.x - 1)     { player.highlightedNode.x++; }
  if (PC::buttons.pressed(BTN_UP) && player.highlightedNode.y > 0)                           { player.highlightedNode.y--; }
  if (PC::buttons.pressed(BTN_DOWN) && player.highlightedNode.y < puzzle.maximum.y - 1)      { player.highlightedNode.y++; }

  if (PC::buttons.pressed(BTN_A) && isNode(player.highlightedNode.x, player.highlightedNode.y)) {

    if (nodeAlreadyPlayed(getNodeValue(player.highlightedNode.x, player.highlightedNode.y))) {

      clearBoard(getNodeValue(player.highlightedNode.x, player.highlightedNode.y));
      player.selectedNode.value = getNodeValue(player.highlightedNode.x, player.highlightedNode.y);
      player.selectedNode.x = player.highlightedNode.x;
      player.selectedNode.y = player.highlightedNode.y;
      gameState = STATE_NODE_SELECTED;

    }
    else {

      player.selectedNode.value = getNodeValue(player.highlightedNode.x, player.highlightedNode.y);
      player.selectedNode.x = player.highlightedNode.x;
      player.selectedNode.y = player.highlightedNode.y;
      gameState = STATE_NODE_SELECTED;

    }

  }
  
  renderBoard();
  
}


/* ----------------------------------------------------------------------------
 *   Handle the game play after the user selects a node.
 *   
 *   The user can only navigate through empty cells or onto the node that matches
 *   the one previously selected.  They can also back-track along the pipe they
 *   have already layed.
 *   
 *   Pressing the B button will clear the pipe that is currently being layed.
 */
void Game::play_NodeSelected() {

  if (PC::buttons.pressed(BTN_LEFT)) {
    
    if (validMove(LEFT, player.selectedNode, player.highlightedNode.x - 1, player.highlightedNode.y)) {

      switch (getPipeValue(player.highlightedNode.x, player.highlightedNode.y)) {

        case CONNECTOR_HORIZONTAL_LR:
        case CONNECTOR_CORNER_TR:
        case CONNECTOR_CORNER_BR:
          updatePipeWhenReversing(player.highlightedNode.x - 1, player.highlightedNode.y);
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, NOTHING, NOTHING);
          break;

        case CONNECTOR_HORIZONTAL_RL:
        case CONNECTOR_CORNER_TL:
        case CONNECTOR_CORNER_BL:
          if (!isNode(player.highlightedNode.x - 1, player.highlightedNode.y)) { setCellValue(player.highlightedNode.x - 1, player.highlightedNode.y, CONNECTOR_HORIZONTAL_RL, player.selectedNode.value); }
          break;

        case CONNECTOR_CORNER_LT:
        case CONNECTOR_CORNER_RT:
        case CONNECTOR_VERTICAL_BT:
          if (!isNode(player.highlightedNode.x - 1, player.highlightedNode.y)) { setCellValue(player.highlightedNode.x - 1, player.highlightedNode.y, CONNECTOR_HORIZONTAL_RL, player.selectedNode.value); }
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, CONNECTOR_CORNER_BL, player.selectedNode.value);
          break;

        case CONNECTOR_CORNER_LB:
        case CONNECTOR_CORNER_RB:
        case CONNECTOR_VERTICAL_TB:
          if (!isNode(player.highlightedNode.x - 1, player.highlightedNode.y)) { setCellValue(player.highlightedNode.x - 1, player.highlightedNode.y, CONNECTOR_HORIZONTAL_RL, player.selectedNode.value); }
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, CONNECTOR_CORNER_TL, player.selectedNode.value);
          break;

        case NODE:
          setCellValue(player.highlightedNode.x - 1, player.highlightedNode.y, CONNECTOR_HORIZONTAL_RL, player.selectedNode.value);
          break;

      }
  
  
      // Have we selected the original node?  If so clear the board of this pipe ..
      
      if (player.highlightedNode.x - 1 == player.selectedNode.x && player.highlightedNode.y == player.selectedNode.y) {
    
        clearBoard(player.selectedNode.value);
        clearSelection();
        gameState = STATE_NO_SELECTION;
          
      }
  
  
      // Have we selected a pair node?  
      
      if (isNode(player.highlightedNode.x - 1, player.highlightedNode.y)  && getNodeValue(player.highlightedNode.x - 1, player.highlightedNode.y) == player.selectedNode.value) {
    
        clearSelection();
        gameState = STATE_NO_SELECTION;
 

        // Is the level finished ?

        if (isPuzzleComplete()) {

          gameState = STATE_GAME_OVER;
    
        }
                 
      }  
      
      player.highlightedNode.x--;
      
    }
    
  }

  if (PC::buttons.pressed(BTN_RIGHT)) {

    if (validMove(RIGHT, player.selectedNode, player.highlightedNode.x + 1, player.highlightedNode.y)) {

      switch (getPipeValue(player.highlightedNode.x, player.highlightedNode.y)) {

        case CONNECTOR_HORIZONTAL_RL:
        case CONNECTOR_CORNER_TL:
        case CONNECTOR_CORNER_BL:
          updatePipeWhenReversing(player.highlightedNode.x + 1, player.highlightedNode.y);
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, NOTHING, NOTHING);
          break;

        case CONNECTOR_HORIZONTAL_LR:
        case CONNECTOR_CORNER_TR:
        case CONNECTOR_CORNER_BR:
          if (!isNode(player.highlightedNode.x + 1, player.highlightedNode.y)) { setCellValue(player.highlightedNode.x + 1, player.highlightedNode.y, CONNECTOR_HORIZONTAL_LR, player.selectedNode.value); }
          break;

        case CONNECTOR_CORNER_RT:
        case CONNECTOR_CORNER_LT:
        case CONNECTOR_VERTICAL_BT:
          if (!isNode(player.highlightedNode.x + 1, player.highlightedNode.y)) { setCellValue(player.highlightedNode.x + 1, player.highlightedNode.y, CONNECTOR_HORIZONTAL_LR, player.selectedNode.value); }
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, CONNECTOR_CORNER_BR, player.selectedNode.value);
          break;

        case CONNECTOR_CORNER_RB:
        case CONNECTOR_CORNER_LB:
        case CONNECTOR_VERTICAL_TB:
          if (!isNode(player.highlightedNode.x + 1, player.highlightedNode.y)) { setCellValue(player.highlightedNode.x + 1, player.highlightedNode.y, CONNECTOR_HORIZONTAL_LR, player.selectedNode.value); }
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, CONNECTOR_CORNER_TR, player.selectedNode.value);
          break;

        case NODE:
          setCellValue(player.highlightedNode.x + 1, player.highlightedNode.y, CONNECTOR_HORIZONTAL_LR, player.selectedNode.value);
          break;

      }
  
  
      // Have we selected the original node?  If so clear the board of this pipe ..
      
      if (player.highlightedNode.x + 1 == player.selectedNode.x && player.highlightedNode.y == player.selectedNode.y) {
    
        clearBoard(player.selectedNode.value);
        clearSelection();
        gameState = STATE_NO_SELECTION;
          
      }
  
  
      // Have we selected a pair node?  
      
      if (isNode(player.highlightedNode.x + 1, player.highlightedNode.y)  && getNodeValue(player.highlightedNode.x + 1, player.highlightedNode.y) == player.selectedNode.value) {
    
        clearSelection();
        gameState = STATE_NO_SELECTION;
  

        // Is the level finished ?

        if (isPuzzleComplete()) {

          gameState = STATE_GAME_OVER;
    
        }
                
      }
    
      player.highlightedNode.x++;
  
    }

  }

  if (PC::buttons.pressed(BTN_UP)) {
    
    if (validMove(UP, player.selectedNode, player.highlightedNode.x, player.highlightedNode.y - 1)) {

      switch (getPipeValue(player.highlightedNode.x, player.highlightedNode.y)) {

        case CONNECTOR_VERTICAL_TB:
        case CONNECTOR_CORNER_RB:
        case CONNECTOR_CORNER_LB:
          updatePipeWhenReversing(player.highlightedNode.x, player.highlightedNode.y - 1);
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, NOTHING, NOTHING);
          break;

        case CONNECTOR_VERTICAL_BT:
        case CONNECTOR_CORNER_LT:
        case CONNECTOR_CORNER_RT:
          if (!isNode(player.highlightedNode.x, player.highlightedNode.y - 1)) { setCellValue(player.highlightedNode.x, player.highlightedNode.y - 1, CONNECTOR_VERTICAL_BT, player.selectedNode.value); }
          break;

        case CONNECTOR_CORNER_TR:
        case CONNECTOR_CORNER_BR:
        case CONNECTOR_HORIZONTAL_LR:
          if (!isNode(player.highlightedNode.x, player.highlightedNode.y - 1)) { setCellValue(player.highlightedNode.x, player.highlightedNode.y - 1, CONNECTOR_VERTICAL_BT, player.selectedNode.value); }
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, CONNECTOR_CORNER_LT, player.selectedNode.value); 
          break;

        case CONNECTOR_CORNER_TL:
        case CONNECTOR_CORNER_BL:
        case CONNECTOR_HORIZONTAL_RL:
          if (!isNode(player.highlightedNode.x, player.highlightedNode.y - 1)) { setCellValue(player.highlightedNode.x, player.highlightedNode.y - 1, CONNECTOR_VERTICAL_BT, player.selectedNode.value); }
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, CONNECTOR_CORNER_RT, player.selectedNode.value);
          break;

        case NODE:
          setCellValue(player.highlightedNode.x, player.highlightedNode.y - 1, CONNECTOR_VERTICAL_BT, player.selectedNode.value);
          break;

      }
  
  
      // Have we selected the original node?  If so clear the board of this pipe ..
      
      if (player.highlightedNode.x == player.selectedNode.x && player.highlightedNode.y - 1 == player.selectedNode.y) {
    
        clearBoard(player.selectedNode.value);
        clearSelection();
        gameState = STATE_NO_SELECTION;
         
      }
  
  
      // Have we selected a pair node?  
      
      if (isNode(player.highlightedNode.x, player.highlightedNode.y - 1)  && getNodeValue(player.highlightedNode.x, player.highlightedNode.y - 1) == player.selectedNode.value) {
    
        clearSelection();
        gameState = STATE_NO_SELECTION;
 

        // Is the level finished ?

        if (isPuzzleComplete()) {

          gameState = STATE_GAME_OVER;
    
        }
                 
      }  
    
      player.highlightedNode.y--;
    }

  }

  if (PC::buttons.pressed(BTN_DOWN)) {
    
    if (validMove(DOWN, player.selectedNode, player.highlightedNode.x, player.highlightedNode.y + 1)) {

      switch (getPipeValue(player.highlightedNode.x, player.highlightedNode.y)) {

        case CONNECTOR_VERTICAL_BT:
        case CONNECTOR_CORNER_RT:
        case CONNECTOR_CORNER_LT:
          updatePipeWhenReversing(player.highlightedNode.x, player.highlightedNode.y + 1);
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, NOTHING, NOTHING);
          break;

        case CONNECTOR_VERTICAL_TB:
        case CONNECTOR_CORNER_LB:
        case CONNECTOR_CORNER_RB:
          if (!isNode(player.highlightedNode.x, player.highlightedNode.y + 1)) { setCellValue(player.highlightedNode.x, player.highlightedNode.y + 1, CONNECTOR_VERTICAL_TB, player.selectedNode.value); }
          break;

        case CONNECTOR_CORNER_TR:
        case CONNECTOR_CORNER_BR:
        case CONNECTOR_HORIZONTAL_LR:
          if (!isNode(player.highlightedNode.x, player.highlightedNode.y + 1)) { setCellValue(player.highlightedNode.x, player.highlightedNode.y + 1, CONNECTOR_VERTICAL_TB, player.selectedNode.value); }
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, CONNECTOR_CORNER_LB, player.selectedNode.value);
          break;

        case CONNECTOR_CORNER_TL:
        case CONNECTOR_CORNER_BL:
        case CONNECTOR_HORIZONTAL_RL:
          if (!isNode(player.highlightedNode.x, player.highlightedNode.y + 1)) { setCellValue(player.highlightedNode.x, player.highlightedNode.y + 1, CONNECTOR_VERTICAL_TB, player.selectedNode.value); }
          setCellValue(player.highlightedNode.x, player.highlightedNode.y, CONNECTOR_CORNER_RB, player.selectedNode.value);
          break;

        case NODE:
          setCellValue(player.highlightedNode.x, player.highlightedNode.y + 1, CONNECTOR_VERTICAL_TB, player.selectedNode.value);
          break;
        
      }

  
      // Have we selected the original node?  If so clear the board of this pipe ..
      
      if (player.highlightedNode.x == player.selectedNode.x && player.highlightedNode.y + 1 == player.selectedNode.y) {
    
        clearBoard(player.selectedNode.value);
        clearSelection();
        gameState = STATE_NO_SELECTION;
          
      }
   
  
      // Have we selected a pair node?  
      
      if (isNode(player.highlightedNode.x, player.highlightedNode.y + 1)  && getNodeValue(player.highlightedNode.x, player.highlightedNode.y + 1) == player.selectedNode.value) {
    
        clearSelection();
        gameState = STATE_NO_SELECTION;


        // Is the level finished ?

        if (isPuzzleComplete()) {

          gameState = STATE_GAME_OVER;
    
        }
  
      }  
   
      player.highlightedNode.y++;

    }

  }

  if (PC::buttons.pressed(BTN_B)) {

    clearBoard(player.selectedNode.value);
    clearSelection();
    gameState = STATE_NO_SELECTION;
      
  }


  renderBoard();
  
}


/* ----------------------------------------------------------------------------
 *   Fix the pipe graphics when reversing back on yourself.
 */
void Game::updatePipeWhenReversing(uint8_t x, uint8_t y) {

  switch (getPipeValue(x, y)) {

    case CONNECTOR_CORNER_TR:           
    case CONNECTOR_CORNER_TL:                   
    case CONNECTOR_VERTICAL_TB:                 
      setCellValue(x, y, CONNECTOR_VERTICAL_TB, player.selectedNode.value);
      break;

    case CONNECTOR_CORNER_BR:          
    case CONNECTOR_CORNER_BL:          
    case CONNECTOR_VERTICAL_BT:        
      setCellValue(x, y, CONNECTOR_VERTICAL_BT, player.selectedNode.value);
      break;

    case CONNECTOR_CORNER_LT:
    case CONNECTOR_CORNER_LB:
    case CONNECTOR_HORIZONTAL_LR:
      setCellValue(x, y, CONNECTOR_HORIZONTAL_LR, player.selectedNode.value);
      break;
           
    case CONNECTOR_CORNER_RT:                   
    case CONNECTOR_CORNER_RB: 
    case CONNECTOR_HORIZONTAL_RL:        
      setCellValue(x, y, CONNECTOR_HORIZONTAL_RL, player.selectedNode.value);
      break;
 
  }
 
}


/* ----------------------------------------------------------------------------
 *   Is the nominated move valid?
 *   
 *   The user can only navigate through empty cells or onto the node that matches
 *   the one previously selected.  They can also back-track along the pipe they
 *   have already layed.
 *   
 *   direction:     The button the user clicked, eg. UP, DOWN, LEFT and Right.
 *   selectedNode:  The node previously selected.
 *   x and y:       Coordinates to test.  Passed as int8_t to allow negatives.
 */
bool Game::validMove(uint8_t direction, Node selectedNode, int8_t x, int8_t y) {

  
  // Off the grid!

  if (x < 0 || x >= puzzle.maximum.x || y < 0 || y >= puzzle.maximum.y) return false;

  
  // Is it a clear cell or the matching node?
  
  if (
      (!isNode(x,y) && getPipeValue(x,y) == NOTHING) ||
      (isNode(x,y) && getNodeValue(x,y) == selectedNode.value && (x != selectedNode.x || y != selectedNode.y))
     ) return true;
  

  // Is the pipe turning back on itself?

  switch (direction) {

    case (UP):
    
      switch (getPipeValue(player.highlightedNode.x, player.highlightedNode.y)) {

        case CONNECTOR_VERTICAL_TB:
        case CONNECTOR_CORNER_RB:
        case CONNECTOR_CORNER_LB:
          return true;

      }

      break;
      
    case (DOWN):
    
      switch (getPipeValue(player.highlightedNode.x, player.highlightedNode.y)) {
          
        case CONNECTOR_VERTICAL_BT:
        case CONNECTOR_CORNER_LT:
        case CONNECTOR_CORNER_RT:
          return true;

      }

      break;

    case (LEFT):
    
      switch (getPipeValue(player.highlightedNode.x, player.highlightedNode.y)) {
   
        case CONNECTOR_CORNER_TR:
        case CONNECTOR_CORNER_BR:
        case CONNECTOR_HORIZONTAL_LR:
          return true;

      }

      break;

    case (RIGHT):
    
      switch (getPipeValue(player.highlightedNode.x, player.highlightedNode.y)) {

        case CONNECTOR_CORNER_TL:
        case CONNECTOR_CORNER_BL:
        case CONNECTOR_HORIZONTAL_RL:
          return true;

      }

      break;

  }
  
  return false;
  
}


/* ----------------------------------------------------------------------------
 *   Place holder for the Game Over function.
 */
void Game::play_GameOver() {

  PC::wait(5000);
  gameState = STATE_SPLASH_INIT;
  
}