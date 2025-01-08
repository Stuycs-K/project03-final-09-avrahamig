#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#ifndef NETWORKING_H
#define NETWORKING_H
#define WKP "./toServer"

#define HANDSHAKE_BUFFER_SIZE 10
#define BUFFER_SIZE 1000
#define MAX_CLIENTS 100

#define SYN 0
#define SYN_ACK 1
#define ACK 2
#define MESSAGE 3
#define EXIT 4


int server_handshake(int *to_client);
int client_handshake(int *to_server);

void server_handshake_half(int *to_client, int from_client);

int server_setup();

//multi_server
int multi_server_setup();
int multi_server_connect(int from_client, struct message m);

#endif
