// represents the board in the tic-tac-toe game
typedef struct{
  char board[3][3]; // array to hold elements on the board
  int X[5][2];		  // holds positions on the board played by X (e.g., a value of [1,2] in this array indicates that X is placed at [1,2])
  int O[5][2];		  // holds positions on the board played by O (e.g., a value of [1,2] in this array indicates that X is placed at [1,2])
  int num_X; 		    // number of moves played by X
  int num_O; 		    // number of moves played by O
} GameBoard;

// displays the game board to the screen
void display_board(GameBoard *b);

// plays a turn for the server
void play_turn_server(GameBoard *b, char play);

// plays a turn for the client. Returns the following values: 
//	-- -1: if client is trying to play on a space that is not available (i.e., occupied by an X or O)
//	-- 0: if client is able to successfully place their piece on an empty space on the board
int play_turn_client(GameBoard *b, char play, int row, int col);

// determines the winner of the game (if a winner exists). Returns the following: 
//	-- X: if the winner is X
//	-- O: if the winner is O
//	-- N: if there is no winner. There will be no winner in two cases: the game is a tie or there are still spaces on the board left to play
char find_winner(GameBoard *b);