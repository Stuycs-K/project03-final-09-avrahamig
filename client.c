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
  char sents[numRounds*2+1][64];

  fgets(sentence, 64, stdin);
  write(to_server, sentence, strlen(sentence));

  for (int i = 0; i < numRounds; i++) {
    read(from_server, rec1, strlen(rec1));
    printf("Here is the sentence you received: %s\nWhat do you think it was supposed to say? Enter below:\n", rec1);
    fgets(sent[1], 64, stdin);
    write(to_server, sent1, strlen(sent1));
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
