#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "client.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 60002

int main(){
  int mySocket;
  struct sockaddr_in  addr;
  char command = ' ';
  int i;
  int bytesRcv;
  int row, column;

  char inStr[10];
  char buffer[10];

  /* create socket */
  mySocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (mySocket < 0) {
    printf("eek! couldn't open socket\n");
    exit(-1);
  }
  
  /* setup address */
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(SERVER_IP);
  addr.sin_port = htons((unsigned short) SERVER_PORT);

  /* connect to server */
  i = connect(mySocket, (struct sockaddr *) &addr, sizeof(addr));

  if (i<0) {
    printf("client could not connect!\n");
    exit(-1);
  }

  /* get input from user and send to server */
  while (1) {
    show_menu();
    printf("Enter a choice from the menu (P, Q): ");
    scanf(" %[^\n]s", inStr);

    // Quit game
    if(strcmp(inStr, "q") == 0 || strcmp(inStr, "Q") == 0){
      strcpy(buffer, "q");
    }
    
    // Plays the game
    else if(strcmp(inStr, "p") == 0 || strcmp(inStr, "P") == 0){
      // Ask client where to place character
      printf("Enter the row of the spot you want to occupy(1-3): ");
      scanf(" %d", &row);
      printf("Enter the column of the spot you want to occupy(1-3): ");
      scanf(" %d", &column);
      printf("\n");

      // Ensures chosen location is valid
      if(row < 1 || row > 3 || column < 1 || column > 3){
        printf("The location of [%d, %d] is invalid. Please try again.\n", row, column);
        continue;
      }

      // Store move in buffer and pass it to the server
      sprintf(&buffer[0], "%d", row);
      sprintf(&buffer[1], "%d", column);
    }
    
    // Handles invalid input
    else{
      strcpy(buffer, "invalid");
    }

    send(mySocket, buffer, strlen(buffer), 0);
    
    // Get response from server
    bytesRcv = recv(mySocket, buffer, 10, 0);
    buffer[bytesRcv] = 0; // put a 0 at the end so we can display the string

    // Stop the client if the server asks it to stop
    if((strcmp(buffer, "quit") == 0)){
      break;
    }
    else if(strcmp(buffer, "clientWon") == 0){
      break;
    }
    else if(strcmp(buffer, "serverWon") == 0){
      break;
    }
    else if(strcmp(buffer, "tie") == 0){
      break;
    }
  } 

  /* close the socket */
  printf("The CLIENT is shutting down....\n");
  close(mySocket);

  return 0;
}

// shows a menu to the user
void show_menu(){
  printf("==========================\n");
  printf("What would you like to do?\n");
  printf("==========================\n");
  printf("Play a turn (P)\n");
  printf("Quit (Q)\n");
  printf("----------------------------\n");
}