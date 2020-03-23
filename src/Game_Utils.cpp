#include "Game.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;



/* ----------------------------------------------------------------------------
 *  Return the upper 4 bits of a byte.
 */
uint8_t Game::leftValue(uint8_t val) {

  return val >> 4; 
      
}


/* ----------------------------------------------------------------------------
 *  Return the lower 4 bits of a byte.
 */
uint8_t Game::rightValue(uint8_t val) {

  return val & 0x0F; 
      
}


/* ----------------------------------------------------------------------------
 *  Initialise the board.
 */
void Game::initBoard(uint8_t puzzleNumber) {

  uint8_t x = 0;
  uint8_t y = 0;
  uint8_t byteRead = 0;

  for (uint8_t i = (puzzleNumber * 15); i < (puzzleNumber + 1) * 15; i++) {

    byteRead = puzzles_5x5[i];

  
    // Load up the left hand value ..

    puzzle.board[y][x] = 0;
    if (leftValue(byteRead) > 0) {
      puzzle.board[y][x] = 0xF0 | leftValue(byteRead);
    }
    x++;

  
    // Are we still in the confines of the board?

    if (x <= puzzle.maximum.x) {         
      puzzle.board[y][x] = 0;
      if (rightValue(byteRead) > 0) {
        puzzle.board[y][x] = 0xF0 | rightValue(byteRead);
      }
    }

      
    x++;
      
    if (x >= puzzle.maximum.x) { y++; x = 0; }
        
  }

}



/* ----------------------------------------------------------------------------
 *  Has the node already been played?
 */
bool Game::nodeAlreadyPlayed(uint8_t value) { 

  for (uint8_t y = 0; y < puzzle.maximum.y; y++) {
    
    for (uint8_t x = 0; x < puzzle.maximum.x; x++) {
      
      if (getNodeValue(x, y) == value && !isNode(x, y)) {
        
        return true;
        
      }
      
    }
      
  }
    
  return false;
    
}


/* ----------------------------------------------------------------------------
 *  Clear the board of pipes for a certain node value.
 */
void Game::clearBoard(uint8_t nodeValue) {

  for (uint8_t y = 0; y < puzzle.maximum.y; y++) {
    
    for (uint8_t x = 0; x < puzzle.maximum.x; x++) {
      
      if (getNodeValue(x, y) == nodeValue && !isNode(x, y)) {
        
        puzzle.board[y][x] = NOTHING;
        
      }
      
    }
    
  }
  
}


/* ----------------------------------------------------------------------------
 *  Is the position nominated a node?
 */
bool Game::isNode(uint8_t x, uint8_t y) {

  return (puzzle.board[y][x] & 0xF0) == 0xF0;
  
}


/* ----------------------------------------------------------------------------
 *  Get the node value for the position.
 */
uint8_t Game::getNodeValue(uint8_t x, uint8_t y) {
  
  return (puzzle.board[y][x] & 0x0F);

}


/* ----------------------------------------------------------------------------
 *  Is the position nominated a pipe?
 */
bool Game::isPipe(uint8_t x, uint8_t y) {

  return (puzzle.board[y][x] & 0xF0) > 0x00 && (puzzle.board[y][x] & 0xF0) != 0xF0;
  
}


/* ----------------------------------------------------------------------------
 *  Get the pipe value for the position.
 */
uint8_t Game::getPipeValue(uint8_t x, uint8_t y) {
  
  return (puzzle.board[y][x] & 0xF0) >> 4;

}


/* ----------------------------------------------------------------------------
 *  Set the pipe value for the position.
 */
void Game::setCellValue(uint8_t x, uint8_t y, uint8_t pipeValue, uint8_t nodeValue) {
  
  puzzle.board[y][x] = (pipeValue << 4) | nodeValue;
  
}


/* ----------------------------------------------------------------------------
 *  Clear the player's selection.
 */
void Game::clearSelection() {

  player.selectedNode.value = 0;
  player.selectedNode.x = 0;
  player.selectedNode.y = 0;

}


/* ----------------------------------------------------------------------------
 *  Is the puzzle complete?  All cells should have a 'node' value.
 */
bool Game::isPuzzleComplete() {

  for (uint8_t y = 0; y < puzzle.maximum.y; y++) {
    
    for (uint8_t x = 0; x < puzzle.maximum.x; x++) {
      
      if (getNodeValue(x, y) == 0) {
        
        return false;
        
      }
      
    }
    
  }

  return true;
  
}
