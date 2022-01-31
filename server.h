// saves data about the game to the game_logs.txt text file)
void display_history(GameBoard* board);

// ends the game and performs cleanup functions. At a minimum, this function should do the following actions: 
//	-- save data to the games_logs.txt file
//	-- dealloacte memory used by the program
//	-- send a signal to the client to terminate
void end_game(GameBoard* board, int clientSocket, char *buffer, int bufferLength, int flags);

//=======================================================//
//                 	 Helper Function                     //
//=======================================================//

// Return 1 if the board is full and 0 otherwise
int fullBoard(GameBoard *board);