#include "pipe_networking.h"

static void sighandler(int signo) {
  if (signo == SIGINT) {
    remove(WKP);
    exit(0);
  }
  if (signo == SIGPIPE) {
  }
}

int editSentence(char * original, char * mode) {
  int len = strlen(original) - 1;
  if (mode[0] == 'x') {
    original = strsep(& original, "\n");
    strfry(original);
    return 0;
  }
  if (mode[0] == 'h') {
    original = strsep(& original, "\n");
    char new[128] = "";
    char space[2] = " ";
    char * curr = original;
    curr[len] = ' ';
    int i = 0;
    while (strlen(curr)) {
      if (i > 0) {
        strcat(new, space);
      }
      char * token;
      token = strsep(& curr, " ");
      strfry(token);
      strcat(new, token);
      i++;
    }
    for (int i = 0; i < len; i++) {
      original[i] = new[i];
    }
    return 0;
  }
  int numLets = len / 3;
  if (mode[0] == 'e') {
    numLets = len / 4;
  }
  for (int i = 0; i < numLets; i++) {
    int rando = (int) rand() % (len - 1);
    char letter = (int) rand() % 69 + 58;
    original[rando] = letter;
  }
  return 0;
}

int main(int argc, char * argv[]) {
  if (argc == 1) {
    printf("Please include the number of players as a command line argument. \nIf you would like an more rounds than the number of players (more games will be created by pre-generated sentences), add a second command line argument with the number of rounds.\n");
    return 0;
  }

  signal(SIGINT, sighandler);
  signal(SIGPIPE, sighandler);

  printf("What difficulty mode would you like? \nType e for easy (changing 1/4 of the letters)\nm for medium (changing 1/3 of the letters) \nh for hard (strfrying every word) \nand x for xtreme (strfrying the whole thing)\n");
  char difficulty[16];
  fgets(difficulty, 16, stdin);

  shmget(key, sizeof(int), IPC_CREAT | 0666);  
  char shm[64] = shmget(key, 64, 0);
  char * diff = shmat(shm, 0, 0);
  * diff = difficulty;

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

  char * extraSentences[64] = {"abcdefg\n", "hijklmnop\n", "qrstuv\n", "wxyz\n", "123456\n", "7890\n"};

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
      close(fds[i][READ]);
      write(fds[i][WRITE], line, sizeof(line));
      close(fdsToParent[i][WRITE]);
    }
    for (int currRound = 0; currRound < numPlayers - 1; currRound++) {
      for (int i = 0; i < numPlayers; i++) {
        char sentence[128] = "";
        read(fdsToParent[i][READ], sentence, 128);
        int j = i+1;
        if (j == numPlayers) {
          j = 0;
        }
        printf("Parent received sentence: %s\n", sentence);
        write(fds[j][WRITE], sentence, sizeof(sentence));
      }
    }
    for (int currRound = numPlayers; currRound < numRounds; currRound++) {
      for (int i = 0; i < numPlayers; i++) {
        char sentence[128] = "";
        if (currRound % numPlayers == 0) {
          char finalSentence[128] = "";
          read(fdsToParent[i][READ], finalSentence, 128);
          printf("Final sentence (in parent): %s\n", finalSentence);
          //write finalSentence to file, execvp
          int randSent = (int) rand() % 6;
          strcpy(sentence, extraSentences[randSent]);
          editSentence(sentence, difficulty);
        }
        else {
          read(fdsToParent[i][READ], sentence, 128);
        }
        int j = i+1;
        if (j == numPlayers) {
          j = 0;
        }
        printf("Parent received sentence: %s\n", sentence);
        write(fds[j][WRITE], sentence, sizeof(sentence));
      }
    }
    char shm[64] = shmget(key, 0, 0);
    shmctl(shm, IPC_RMID, 0);
  }
  else {
    for (int i = 0; i < numPlayers; i++) {
      //printf("hey\n");
      if (getpid() == childPids[i]) {
        char line[16];
        close(fds[i][WRITE]);
        read(fds[i][READ], line, sizeof(line));
        char numRoundsStr[16];
        sprintf(numRoundsStr, "%d", numRounds);
        write(to_client, numRoundsStr, 16);
        for (int currRound = 0; currRound < numRounds; currRound++) {
          char sent[128] = "";
          char sentFromPar[128] = "";
          if (currRound != 0 && currRound % numPlayers == 0) {
            char randomSent[128] = "";
            read(fds[i][READ], randomSent, 128);
            write(to_client, randomSent, sizeof(randomSent));
          }
          read(from_client, sent, 128);
          printf("Received sentence: %s\n", sent);
          if (currRound < numRounds - 1) {
            if (currRound % numPlayers == numPlayers - 1) {
              write(fdsToParent[i][WRITE], sent, sizeof(sent));
            }
            else {
              editSentence(sent, difficulty);
              printf("Edited sentence: %s\n", sent);
              write(fdsToParent[i][WRITE], sent, sizeof(sent));
              read(fds[i][READ], sentFromPar, 128);
              write(to_client, sentFromPar, sizeof(sentFromPar));
            }
          }
          else {
            printf("Final sentence: %s\n", sent);
            //execvp
          }
        }
      }
    }
  }



  return 0;
}
