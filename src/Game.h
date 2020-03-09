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

      byte leftValue(byte val);
      byte rightValue(byte val);
      byte getNodeValue(byte x, byte y);
      byte getPipeValue(byte x, byte y);

      void initBoard(byte puzzleNumber);
      bool nodeAlreadyPlayed(byte value);
      void clearBoard(byte nodeValue);
      bool isNode(byte x, byte y);
      bool isPipe(byte x, byte y);
      void setCellValue(byte x, byte y, byte pipeValue, byte nodeValue);
      void clearSelection();
      bool isPuzzleComplete();
      void updatePipeWhenReversing(byte x, byte y);
      bool validMove(byte direction, Node selectedNode, int x, int y);


    private:

      Player player;
      Puzzle puzzle;

      byte gameState = STATE_SPLASH_INIT;
      long splash_counter = 0;

      const byte* nodes[11] = { 0, Node1, Node2, Node3, Node4, Node5, Node6, Node7, Node8, Node9, Node10 };
      const byte* connectors[13] = { Connector_Nothing, Connector_Horizontal, Connector_Horizontal, Connector_Vertical, Connector_Vertical, Connector_Corner_TL, Connector_Corner_TL, 
                                      Connector_Corner_TR, Connector_Corner_TR, Connector_Corner_BL, Connector_Corner_BL, Connector_Corner_BR, Connector_Corner_BR};
      const byte levels[5] = { PUZZLE_5X5, PUZZLE_6X6, PUZZLE_7X7, PUZZLE_8X8, PUZZLE_9X9 };

};