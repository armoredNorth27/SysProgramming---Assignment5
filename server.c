#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "tic-tac-toe.h"
#include "server.h"

#define MY_PORT 60002

				
int main(){
	printf("Running server...\n");
  
	int myListenSocket, clientSocket;
	struct sockaddr_in  myAddr, clientAddr;
  	int i, addrSize, bytesRcv, requestedRow, requestedCol, playResult, boardFull;
	char winResult;

	// Make a new game board and display it
	GameBoard* newBoard = (GameBoard*)malloc(sizeof(GameBoard));
	newBoard->num_X = 0;
	newBoard->num_O = 0;
	display_board(newBoard);

  	char buffer[30];

	/* create socket */
  	myListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  	if (myListenSocket < 0) {
   		printf("eek! couldn't open socket\n");
    	exit(-1);
  	}

	/* setup my server address */
  	memset(&myAddr, 0, sizeof(myAddr));
  	myAddr.sin_family = AF_INET;
  	myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  	myAddr.sin_port = htons((unsigned short) MY_PORT);

	/* bind my listen socket */
  	i = bind(myListenSocket, (struct sockaddr *) &myAddr, sizeof(myAddr));
	  
  	if (i < 0) {
    	printf("eek! couldn't bind socket\n");
    	exit(-1);
  	}

	/* listen */
  	i = listen(myListenSocket, 5);
  	if (i < 0) {
    	printf("eek! couldn't listen\n");
    	exit(-1);
  	}

	/* wait for connection request */
  	addrSize = sizeof(clientAddr);

  	clientSocket = accept(myListenSocket,(struct sockaddr *) &clientAddr, &addrSize);
  
  	if (clientSocket < 0) {
    	printf("eek! couldn't accept the connection\n");
    	exit(-1);
  	}


	/* read message from client and do something with it */
  	while (1) {
    	bytesRcv = recv(clientSocket, buffer, sizeof(buffer), 0);
    	buffer[bytesRcv] = 0;

		// Setup information to stop client and server
    	if(strcmp(buffer, "q") == 0){
			printf("Quitting...\n");
			strcpy(buffer, "quit");
      		break;
      	}
		
		// Handle invalid inputs
		else if(strcmp(buffer, "invalid") == 0){
			printf("Invalid input. Please enter (P) or (Q)\n");
			strcpy(buffer, "invalid");
			send(clientSocket, buffer, strlen(buffer), 0);
		}
		
		// Play game
      	else{
			// Process client's turn
			requestedRow = buffer[0] - '0';
			requestedCol = buffer[1] - '0';
			playResult = play_turn_client(newBoard, 'O', requestedRow, requestedCol);

			// Handles invalid location selection
			if(playResult == -1){
				printf("Invalid move, please select an empty location.\n");
				printf("\n");
				display_board(newBoard);
				strcpy(buffer, "invalid");
				send(clientSocket, buffer, strlen(buffer), 0);
			}

			// Handles valid moves
			else if(playResult == 0){
				//??? Execute client's turn ???//
				printf("Added location of [%d, %d]\n", requestedRow, requestedCol);
				display_board(newBoard);

				//Check if client won after their move
				winResult = find_winner(newBoard);

				// Client won, so end the game
				if(winResult == 'O'){
					printf("The client wins! Ending game...\n");
					strcpy(buffer, "clientWon");
					break;
				}

				// Finds out if the game is a tie or not
				else if(winResult == 'N'){
					boardFull = fullBoard(newBoard);

					// Game is tied, so finish game
					if(boardFull){
						printf("Game ended in a tie! Ending game...\n");
						strcpy(buffer, "tie");
						break;
					}
				}



				//??? Server's turn ???//
				printf("Server playing it's turn...\n");
				play_turn_server(newBoard, 'X');
				display_board(newBoard);

				//Check if server won after their move
				winResult = find_winner(newBoard);

				// Client won, so end the game
				if(winResult == 'X'){
					printf("The server wins! Ending game...\n");
					strcpy(buffer, "serverWon");
					break;
				}

				// Finds out if the game is a tie or not
				else if(winResult == 'N'){
					boardFull = fullBoard(newBoard);

					// Game is tied, so finish game
					if(boardFull){
						printf("Game ended in a tie! Ending game...\n");
						strcpy(buffer, "tie");
						break;
					}
				}



				//??? Server response ???//
				strcpy(buffer, "validMove");
				send(clientSocket, buffer, strlen(buffer), 0);
			}
	   	}
  	} 

	/* close sockets */
	end_game(newBoard, clientSocket, buffer, strlen(buffer), 0);
	printf("The SERVER is shutting down....\n");
	close(myListenSocket);

	return 0;
}

//=======================================================//
//                 Function Implementations              //
//=======================================================//

// saves data about the game to the game_logs.txt text file)
void display_history(GameBoard* board){

	//? X represents server moves
	//? O represents client moves

	// Print server moves
	printf("The moves made by the server are: \n");
	for(int i=0; i<board->num_X; i++){
		printf("[%d, %d]\n", board->X[i][0], board->X[i][1]);
	}
	printf("\n");

	// Print client moves
	printf("The moves made by the client are: \n");
	for(int i=0; i<board->num_O; i++){
		printf("[%d, %d]\n", board->O[i][0], board->O[i][1]);
	}
	printf("\n");
}

// ends the game and performs cleanup functions. At a minimum, this function should do the following actions: 
//	-- save data to the games_logs.txt file
//	-- dealloacte memory used by the program
//	-- send a signal to the client to terminate
void end_game(GameBoard* board, int clientSocket, char *buffer, int bufferLength, int flags){
	display_history(board);
	free(board);
	
	send(clientSocket, buffer, bufferLength, flags);
}

//=======================================================//
//                 	 Helper Function                     //
//=======================================================//

// Return 1 if the board is full and 0 otherwise
int fullBoard(GameBoard *board){
	for(int row=0; row<3; row++){
		for(int col=0; col<3; col++){
			// If we find an empty location, then board isn't full
			if(board->board[row][col] == '\0'){
				return 0;
			}
		}
	}
	
	return 1;
}