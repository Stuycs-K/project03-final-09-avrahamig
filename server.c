#include "pipe_networking.h"

static void sighandler(int signo) {
  if (signo == SIGINT) {
    remove(WKP);
    exit(0);
  }
  if (signo == SIGPIPE) {
    //printf("In sigpipe\n");
  }
}

void editSentence(char * original, int mode) {
  int len = strlen(original) - 1;
  for (int i = 0; i < mode; i++) {
    int rando = (int) rand() % len;
    char letter = (int) rand() % 69 + 58;
    original[rando] = letter;
  }
}

int main(int argc, char * argv[]) {
  if (argc == 1) {
    printf("Please include the number of players as a command line argument. If you would like an amount of rounds that is not equal to the number of players, add a second command line argument with the number of rounds.\n");
    return 0;
  }

  signal(SIGINT, sighandler);
  signal(SIGPIPE, sighandler);

  int numPlayers = atoi(argv[1]);
  int numRounds = numPlayers;
  if (argc == 3) {
    numRounds = atoi(argv[2]);
  }

  int to_client;
  int from_client;

  int players = 0;
  int childPids[numPlayers];

  int p;

  int fds[numPlayers][2];
  int fdsToParent[numPlayers][2];
  for (int i = 0; i < numPlayers; i++) {
    pipe(fds[i]);
    pipe(fdsToParent[i]);
  }

  while (players < numPlayers) {
    from_client = server_setup();
    remove(WKP);
    p = fork();
    if (p) {
      srand(getpid());
      childPids[players] = p;
      players++;
    }
    else {
      srand(getpid());
      server_handshake_half(& to_client, from_client);

      childPids[players] = getpid();
      players = numPlayers;
    }
  }
  if (p) {
    char line[16] = "ready";
    for (int i = 0; i < numPlayers; i++) {
      //printf("childPids i: %d\n", childPids[i]);
      close(fds[i][READ]);
      write(fds[i][WRITE], line, sizeof(line));
      close(fdsToParent[i][WRITE]);
    }
    for (int currRound = 0; currRound < numRounds - 1; currRound++) {
      for (int i = 0; i < numPlayers; i++) {
        int j = i+1;
        if (j == numPlayers) {
          j = 0;
        }
        char sentence[64] = "";
        read(fdsToParent[i][READ], sentence, 64);
        printf("Parent received sentence: %s\n", sentence);
        write(fds[j][WRITE], sentence, sizeof(sentence));
      }
    }
  }
  else {
    for (int i = 0; i < numPlayers; i++) {
      //printf("hey\n");
      if (getpid() == childPids[i]) {
        char line[16];
        char ready[16] = "ready";
        close(fds[i][WRITE]);
        read(fds[i][READ], line, sizeof(line));
        if (! strcmp(line, ready)) {
          write(to_client, argv[2], 16);
          for (int currRound = 0; currRound < numRounds; currRound++) {
            char sent[64] = "";
            char sentFromPar[64] = "";
            read(from_client, sent, 64);
            printf("Received sentence: %s\n", sent);
            if (currRound < numRounds - 1) {
              editSentence(sent, 5);
              printf("Edited sentence: %s\n", sent);
              write(fdsToParent[i][WRITE], sent, sizeof(sent));
              read(fds[i][READ], sentFromPar, 64);
              write(to_client, sentFromPar, sizeof(sentFromPar));
            }
            else {
              printf("Final sentence: %s\n", sent);
              //execvp
            }
          }
        }
        else {
          printf("There is a bug with the code saying the game is ready to begin.\n");
        }
      }
    }
  }










  return 0;
}
