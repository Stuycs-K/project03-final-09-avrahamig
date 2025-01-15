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
    printf("Please include the number of players as a command line argument. If you would like an more rounds than the number of players (more games will be created by pre-generated sentences), add a second command line argument with the number of rounds.\n");
    return 0;
  }

  signal(SIGINT, sighandler);
  signal(SIGPIPE, sighandler);

  int numPlayers = atoi(argv[1]);
  int numRounds = numPlayers - 1;
  if (argc == 3) {
    numRounds = atoi(argv[2]);
  }

  printf("numRounds: %d, numPlayers: %d\n", numRounds, numPlayers);

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

  char * extraSentences[64] = {"abcdefg", "hijklmnop", "qrstuv", "wxyz", "123456", "7890"};

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
    for (int currRound = 0; currRound < numPlayers - 1; currRound++) {
      for (int i = 0; i < numPlayers; i++) {
        char sentence[64] = "";
        read(fdsToParent[i][READ], sentence, 64);
        int j = i+1;
        if (j == numPlayers) {
          j = 0;
        }
        printf("Parent received sentence: %s\n", sentence);
        write(fds[j][WRITE], sentence, sizeof(sentence));
      }
    }
    for (int currRound = numPlayers - 1; currRound < numRounds; currRound++) {
      for (int i = 0; i < numPlayers; i++) {
        char sentence[64] = "";
        if (postRound == 0) {
          char finalSentence[64];
          read(fdsToParent[i][READ], finalSentence, 64);
          printf("Final sentence (in parent): %s\n", finalSentence);
          //write finalSentence to file, execvp
          int randSent = (int) rand() % 6;
          strcpy(sentence, extraSentences[randSent]);
        }
        else {
          read(fdsToParent[i][READ], sentence, 64);
        }
        int j = i+1;
        if (j == numPlayers) {
          j = 0;
        }
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
          char numRoundsStr[16];
          sprintf(numRoundsStr, "%d", numRounds);
          write(to_client, numRoundsStr, 16);
          char sentence[64] = "";
          read(from_client, sentence, 64);
          printf("Received sentence (round 0): %s\n", sent);
          for (int currRound = 0; currRound < numRounds; currRound++) {
            char sent[64] = "";
            char sentFromPar[64] = "";
            if (currRound % numPlayers == numPlayers - 1) {
              char randomSent[64] = "";
              read(fds[i][READ], randomSent, 64);
              write(to_client, randomSent, sizeof(randomSent));
            }
            read(from_client, sent, 64);
            printf("Received sentence: %s\n", sent);
            if (currRounds == numRounds - 1) {
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
