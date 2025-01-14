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

  char numRoundsStr[16];
  read(from_server, numRoundsStr, 16);
  int numRounds = atoi(numRoundsStr);
  printf("The game of telephone has begun! Enter your sentence below:\n");

  char sentence[64];

  fgets(sentence, 64, stdin);
  write(to_server, sentence, strlen(sentence));

  for (int i = 0; i < numRounds; i++) {
    char rec[64] = "";
    char sent[64] = "";
    read(from_server, sent, strlen(sent));
    printf("Here is the sentence you received: %s\nWhat do you think it was supposed to say? Enter below:\n", sent);
    fgets(rec, 64, stdin);
    write(to_server, rec, strlen(rec));
  }

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
