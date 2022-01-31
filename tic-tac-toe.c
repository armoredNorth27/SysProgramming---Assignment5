#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tic-tac-toe.h"

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


//=======================================================//
//                 Function Implementations              //
//=======================================================//

void display_board(GameBoard *b){

    for(int row=0; row<3; row++){
        printf("-----------------------\n");

        for(int col=0; col<3; col++){
            // Conditionals ensure game board lines up regarless of
            // whether we're printing empty spaces of played moves
            if(b->board[row][col] == '\0'){
                printf("     ");
            }
            else{
                printf("  %c  ", b->board[row][col]);
            }

            printf(" | ");
        }
        printf("\n");
    }

    printf("-----------------------\n");
    printf("\n");
}

void play_turn_server(GameBoard *b, char play){
    
    srand(time(NULL));
    int row, col;

    while(1){
        // Generate random numbers between 0-2(inclusive)
        row = rand()%3;
        col = rand()%3;

        // If randomly selected spot is valid, then record move
        if(b->board[row][col] == '\0'){
            b->board[row][col] = play;
            b->X[b->num_X][0] = row+1;
            b->X[b->num_X][1] = col+1;
            b->num_X += 1;

            break;
        }
    }

}

int play_turn_client(GameBoard *b, char play, int row, int col){

    // If the desired location is taken, then we can't play
    if(b->board[row-1][col-1] != '\0'){
        printf("Location is occupied.\n");
        return -1;
    }
    else{
        // Save desired move
        b->board[row-1][col-1] = play;
        b->O[b->num_O][0] = row;
        b->O[b->num_O][1] = col;
        b->num_O += 1;
        return 0;
    }

}

char find_winner(GameBoard *b){
    int gameBoardFull = 1;

    //??? Check if gameboard is full ???//
    for(int row=0; row<3; row++){
        for(int col=0; col<3; col++){
            // If we find an empty location, then we can continue playing
            if(b->board[row][col] == '\0'){
                gameBoardFull = 0;
                break;
            }
        }
        if(!gameBoardFull){
            break;
        }
    }

    if(gameBoardFull){
        return 'N';
    }

    //??? Check for a match ???//

    // Check rows for a match
    for(int i=0; i<3; i++){
        if((b->board[i][0] == 'X') && (b->board[i][1] == 'X') && (b->board[i][2] == 'X')){
            return 'X';
        }
        else if((b->board[i][0] == 'O') && (b->board[i][1] == 'O') && (b->board[i][2] == 'O')){
            return 'O';
        }
    }

    // Check cols for a match
    for(int i=0; i<3; i++){
        if((b->board[0][i] == 'X') && (b->board[1][i] == 'X') && (b->board[2][i] == 'X')){
            return 'X';
        }
        else if((b->board[0][i] == 'O') && (b->board[1][i] == 'O') && (b->board[2][i] == 'O')){
            return 'O';
        }
    }

    // Check diagonals for a match
    if( ((b->board[0][0] == 'X') && (b->board[1][1] == 'X') && (b->board[2][2] == 'X')) || ((b->board[2][0] == 'X') && (b->board[1][1] == 'X') && (b->board[0][2] == 'X')) ){
        return 'X';
    }
    else if( ((b->board[0][0] == 'O') && (b->board[1][1] == 'O') && (b->board[2][2] == 'O')) || ((b->board[2][0] == 'O') && (b->board[1][1] == 'O') && (b->board[0][2] == 'O')) ){
        return 'O';
    }
    
    //??? Board still have playable moves ???//
    return 'N';
}