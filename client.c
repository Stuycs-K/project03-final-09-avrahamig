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

  char sentence[128];

  fgets(sentence, 128, stdin);
  write(to_server, sentence, sizeof(sentence));

  for (int i = 0; i < numRounds - 1; i++) {
    char rec[128] = "";
    char sent[128] = "";
    read(from_server, rec, 128);
    int shm = shmget(KEY, 16, 0);
    char * diff = shmat(shm, 0, 0);
    shmdt(diff);
    printf("Here is the sentence you received: %s\nWhat do you think it was supposed to say? (Reminder, difficulty is %s). Enter below:\n", rec, diff);
    fgets(sent, 128, stdin);
    write(to_server, sent, sizeof(sent));
  }
}
