#include "pipe_networking.h"
//UPSTREAM = to the server / from the client
//DOWNSTREAM = to the client / from the server
/*=========================
  server_setup

  creates the WKP and opens it, waiting for a connection.
  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
  char * wkp = WKP;
  mkfifo(wkp, 0666);
  //printf("1.1\n");
  int from_client = open(wkp, O_RDONLY);
  //printf("1.2\n");
  remove(wkp);
  //printf("1.3\n");
  return from_client;
}

/*=========================
  server_handshake
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe (Client's private pipe).

  returns the file descriptor for the upstream pipe (see server setup).
  =========================*/
int server_handshake(int *to_client) {
  //printf("1\n");
  int from_client = server_setup();
  //printf("2\n");
  int buffer;
  read(from_client, & buffer, sizeof(int));
  //printf("3\n");
  char bufferr[16];
  sprintf(bufferr, "%d", buffer);
  *to_client = open(bufferr, O_WRONLY);
  //printf("4\n");
  //printf("5\n");
  int x = (int) rand();
  char randint[16];
  sprintf(randint, "%d", x);
  write(*to_client, randint, 16);
  //printf("6\n");
  char randIntPlusOne[16];
  read(from_client, randIntPlusOne, 16);
  //printf("7\n");
  int xPlusOne = atoi(randIntPlusOne);
  if (xPlusOne == x+1) {
    //printf("YAY it all works\n");
  }
  else {

    //printf("rand: %d, returned rand: %d\n", x, xPlusOne);
  }
  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  //printf("client 1\n");
  int p = getpid();
  //printf("client 2\n");
  char pp[4];
  sprintf(pp, "%d", p);
  //printf("client 3\n");
  mkfifo(pp, 0666);
  //printf("client 4\n");
  *to_server = open(WKP, O_WRONLY);
  //printf("client 5\n");
  write(*to_server, & p, sizeof(int));
  //printf("client 6\n");
  int from_server = open(pp, O_RDONLY);
  //printf("client 7\n");
  char buffer[16];
  read(from_server, buffer, 16);
  int rando = atoi(buffer);
  //printf("client 7, rand: %d\n", rando);
  rando++;
  char randoPlusOne[16];
  sprintf(randoPlusOne, "%d", rando);
  write(*to_server, randoPlusOne, 16);
  //printf("client 8\n");
  return from_server;
}


/*=========================
  server_handshake_half
  args: int from_client, int * to_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
void server_handshake_half(int *to_client, int from_client) {
  int buffer;
  read(from_client, & buffer, sizeof(int));
  //printf("3\n");
  char bufferr[16];
  sprintf(bufferr, "%d", buffer);
  *to_client = open(bufferr, O_WRONLY);
  //printf("4\n");
  //printf("5\n");
  int x = (int) rand();
  char randint[16];
  sprintf(randint, "%d", x);
  write(*to_client, randint, 16);
  //printf("6\n");
  char randIntPlusOne[16];
  read(from_client, randIntPlusOne, 16);
  //printf("7\n");
  int xPlusOne = atoi(randIntPlusOne);
  if (xPlusOne == x+1) {
    //printf("YAY it all works\n");
  }
  else {

    //printf("rand: %d, returned rand: %d\n", x, xPlusOne);
  }
}
