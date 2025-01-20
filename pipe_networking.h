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

#define READ 0
#define WRITE 1

#define KEY 3595893

int server_handshake(int *to_client);
int client_handshake(int *to_server);

void server_handshake_half(int *to_client, int from_client);

int server_setup();

#endif
