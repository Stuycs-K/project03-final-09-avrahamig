#include "pipe_networking.h"

static void sighandler(int signo) {
  int pid = getpid();
  char pp[16];
  sprintf(pp, "%d", pid);
  remove(pp);
  exit(0);
}

int main() {
  signal(SIGPIPE, sighandler);
  signal(SIGINT, sighandler);
  int to_server;
  int from_server;

  printf("Please wait for all players to join. You may think of your opening sentence if you would like.\n");

  from_server = client_handshake( &to_server );

  char ready[16] = "ready";
  char isReady[16];
  read(from_server, isReady, 16);
  if (! strcmp(ready, isReady)) {
    printf("The game of telephone has begun! Enter your sentence below:\n");
  }
  else {
    printf("There is a bug with the child telling the client the game is ready.\n");
  }
  char sentence[64];
  fgets(sentence, 64, stdin);
  write(to_server, sentence, strlen(sentence));

  /*while (1) {
    char pid[32] = "my pid is ";
    char id[16] = "";
    sprintf(id, "%d", getpid());
    strcat(pid, id);
    write(to_server, pid, strlen(pid));

    char str[16];
    read(from_server, str, 16);
    printf("Returned: %s\n", str);
    sleep(1);
  }*/
}
