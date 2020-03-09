/* ----------------------------------------------------------------------------
 *   Basic structure to hold a node value and coordinates ..
 */
struct Node {
  byte x;
  byte y;
  byte value;
};


/* ----------------------------------------------------------------------------
 *  Player structure to record the highlighted and selected node ..
 */
struct Player {
  Node highlightedNode;
  Node selectedNode;
};


/* ----------------------------------------------------------------------------
 *   Puzzle structure.
 */
struct Puzzle {
  byte index;                             // Puzzle number.
  Node maximum;                           // Used to store the dimensions of the puzzle based
                                          // on the puzzle level, eg. PUZZLE_5X5 has a maximum
                                          // x and y value of 5.
  byte board[9][9];                       // Actual board details - supports maximum of 9 x 9.
};