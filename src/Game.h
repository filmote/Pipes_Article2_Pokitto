#pragma once

#include "Pokitto.h"

#include "utils/Structs.h"
#include "puzzles/Puzzles.h"
#include "images/connectors/Connectors.h"
#include "images/nodes/Nodes.h"
#include "images/pipes/Pipes.h"
#include "images/Heading.h"

class Game {
    
    public:


      // Overall game play ..

      void setup();
      void loop(void);

      void play_InitGame();
      void play_NoSelection();
      void play_NodeSelected();
      void renderBoard();
      void play_GameOver();


      // Manu handling options ..

      void renderBackground();


      // Splash screen handling ..

      void initSplash();
      void renderSplash();


      // Utilities ..

      uint8_t leftValue(uint8_t val);
      uint8_t rightValue(uint8_t val);
      uint8_t getNodeValue(uint8_t x, uint8_t y);
      uint8_t getPipeValue(uint8_t x, uint8_t y);

      void initBoard(uint8_t puzzleNumber);
      bool nodeAlreadyPlayed(uint8_t value);
      void clearBoard(uint8_t nodeValue);
      bool isNode(uint8_t x, uint8_t y);
      bool isPipe(uint8_t x, uint8_t y);
      void setCellValue(uint8_t x, uint8_t y, uint8_t pipeValue, uint8_t nodeValue);
      void clearSelection();
      bool isPuzzleComplete();
      void updatePipeWhenReversing(uint8_t x, uint8_t y);
      bool validMove(uint8_t direction, Node selectedNode, int8_t x, int8_t y);


    private:

      Player player;
      Puzzle puzzle;

      uint8_t gameState = STATE_SPLASH_INIT;
      long splash_counter = 0;

      const uint8_t* nodes[11] = { 0, Node1, Node2, Node3, Node4, Node5, Node6, Node7, Node8, Node9, Node10 };
      const uint8_t* connectors[13] = { Connector_Nothing, Connector_Horizontal, Connector_Horizontal, Connector_Vertical, Connector_Vertical, Connector_Corner_TL, Connector_Corner_TL, 
                                      Connector_Corner_TR, Connector_Corner_TR, Connector_Corner_BL, Connector_Corner_BL, Connector_Corner_BR, Connector_Corner_BR};
      const uint8_t levels[5] = { PUZZLE_5X5, PUZZLE_6X6, PUZZLE_7X7, PUZZLE_8X8, PUZZLE_9X9 };

};