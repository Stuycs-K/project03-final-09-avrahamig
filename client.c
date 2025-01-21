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
  char whichClientStr[16];
  read(from_server, whichClientStr, 16);
  int numRounds = atoi(numRoundsStr);
  int whichClient = atoi(whichClientStr);
  printf("The game of telephone has begun! Enter your sentence below:\n");

  char sentence[128];

  fgets(sentence, 128, stdin);
  write(to_server, sentence, sizeof(sentence));

  int shm = shmget(KEY, sizeof(char), 0);
  char * diff = shmat(shm, 0, 0);

  int shm2 = shmget(KEY+whichClient+1, sizeof(int), 0);
  int * changed = shmat(shm2, 0, 0);

  for (int i = 0; i < numRounds - 1; i++) {
    char rec[128] = "";
    char sent[128] = "";
    read(from_server, rec, 128);
    printf("Here is the sentence you received: %s\n", rec);
    if (!(* diff == 'h' || * diff == 'x')) {
      printf("%d letters were changed\n", * changed);
    }
    printf("What do you think it was supposed to say? (Reminder, difficulty is %c). Enter below:\n", * diff);
    fgets(sent, 128, stdin);
    write(to_server, sent, sizeof(sent));
  }
  shmdt(diff);
  return 0;
}
