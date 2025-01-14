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
    printf("Please include the number of players\n");
    return 0;
  }
  signal(SIGINT, sighandler);
  signal(SIGPIPE, sighandler);
  srand(time(NULL));

  //printf("1st spot\n");
  int to_client;
  int from_client;

  int numPlayers = atoi(argv[1]);
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
      //srand(getpid());
      childPids[players] = p;
      players++;
    }
    else {
      //srand(getpid());
      server_handshake_half(& to_client, from_client);

      /*//printf("hi 2nd\n");
      while (1) {
        char str[32] = "";
        read(from_client, str, 32);

        if (write(to_client, str, strlen(str)) == -1) {
          break;
        }
        //printf("2nd spot\n");
      }
      //printf("3rd spot\n");
      close(to_client);
      close(from_client);
      //printf("4th spot\n");*/

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
    for (int currRound = 0; currRound < numPlayers; currRound++) {
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
          write(to_client, argv[1], 16);
          for (int currRound = 0; currRound < numPlayers; currRound++) {
            char sent[64] = "";
            char sentFromPar[64] = "";
            read(from_client, sent, 64);
            printf("Received sentence: %s\n", sent);
            if (currRound < numPlayers - 1) {
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
