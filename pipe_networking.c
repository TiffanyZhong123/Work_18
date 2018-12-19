#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  char arr1[HANDSHAKE_BUFFER_SIZE];
  int wkp = mkfifo("pipey",0644);

  int y = open("pipey",O_RDONLY);
  printf("Received client pipe\n");
  read(y,arr1,HANDSHAKE_BUFFER_SIZE);
  printf("First message from client pipe: %s\n",arr1);
  //done with pipe
  remove("pipey");
  printf("Pipey is removed!\n");
  //send message to client
  *to_client = open(arr1,O_WRONLY);
  write(*to_client,ACK,HANDSHAKE_BUFFER_SIZE);
  printf("Written to client");
  //wait for client response
  read(y,arr1,HANDSHAKE_BUFFER_SIZE);
  printf("Second message from client: %s\n",arr1);
  printf("handshake is completed!!");
  return y;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  char arr2[HANDSHAKE_BUFFER_SIZE];
  mkfifo("pipe", 0644);

  printf("Connecting to server...\n");
  *to_server = open("pipey",O_WRONLY);

  write(*to_server,"pipe",HANDSHAKE_BUFFER_SIZE);
  printf("Sent name of FIFO\n");

  int x = open("pipe",O_RDONLY);
  read(x,arr2,HANDSHAKE_BUFFER_SIZE);
  printf("Message from server: %s\n", arr2);

  remove("pipe");
  printf("Pipe is removed!\n");

  write(*to_server,ACK,sizeof(ACK));
  printf("Client sent message to server.\n");
  return x;
}
