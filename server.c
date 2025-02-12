#include "pipe_networking.h"

static void sighandler(int signo) {
  if (signo == SIGINT) {
    remove(WKP);
    exit(0);
  }
  if (signo == SIGPIPE) {
  }
}

int editSentence(char * original, char mode) {
  int len = strlen(original) - 1;
  if (mode == 'x') {
    original = strsep(& original, "\n");
    strfry(original);
    return 0;
  }
  if (mode == 'h') {
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
  if (mode == 'e') {
    numLets = len / 4;
  }
  int letsChanged = 0;
  int * track = (int *) malloc(numLets * sizeof(int));
  for (int i = 0; i < numLets; i++) {
    int rando = (int) rand() % (len - 1);
    for (int j = 0; j < i; j++) {
      if (track[j] == rando) {
        letsChanged--;
        break;
      }
    }
    letsChanged++;
    char letter = (int) rand() % 69 + 58;
    original[rando] = letter;
    track[i] = rando;
  }
  free(track);
  return letsChanged;
}

int reset(int i, int numPlayers) {
  int j = i+1;
  if (j == numPlayers) {
    j = 0;
  }
  return j;
}

void transition(int numPlayers, int currRound, int * * changed, int story, int (* fds)[2], int (* fdsToParent)[2], int difficulty, int doIt) {
  char opener[32];
  sprintf(opener, "\n\n\nRound %d:\n\n", currRound+1);
  write(story, opener, strlen(opener));
  for (int i = 0; i < numPlayers; i++) {
    char sentence[128] = "";
    int j = reset(i, numPlayers);
    if (doIt && currRound % numPlayers == 0) {
      char done[16] = "";
      read(fdsToParent[i][READ], done, 16);
      char * extraSentences[64] = {"abcdefg\n", "hijklmnop\n", "qrstuv\n", "wxyz\n", "I have to type sentences.\n", "Dr. Seuss wrote The Cat in the Hat.\n", "Running out of ideas.\n", "Pneumonoultramicroscopicsilicovolcanoconiosis.\n", "I am having a good time.\n", "Writing these extra sentences at 2:30 AM.\n", "Life is enjoyable.\n", "This is propaganda.\n", "Whoever reads this, I hope you have a good day.\n", "This sentence was AI-generated.\n", "HELP! I NEED SOMEBODY!\n", "HELP! NOT JUST ANYBODY!\n", "HELP! YOU KNOW I NEED SOMEONE (to write these sentences for me)!\n", "The weather is cloudy.\n", "Banananananananana.\n", "I feel fulfillment writing these.\n", "life is meaningless anyway.\n", "We must fight back against industrial society!\n", "It's like 40 degrees in here help.\n", "How many sentences is this?\n", "I'm eating my sweater right now out of boredom.\n", "zzzzzzzzzzz... sorry, what happened?\n", "3.14159265358979.\n", "To be or not to be.\n", "The yawning maw of existentialism creeps in a steady pace from sentence to sentence.\n", "Whether 'tis nobler to---I'm done w/ Shakespeare.\n", "I hope this was worth it.\n", "I'll never get these 15 minutes back.\n", "Damnit I lost count of how many I've created.\n", "I predict THIS ONE will show up in my demo. Ha I told you.\n", "This is a stock sentence.\n", "This sentence is copyright-protected.\n", "I probably miscounted and this one can't even show up.\n", "Sigh.\n", "Number 39 *maybe*.\n", "Number 40 *probably not*.\n"};
      int randSent = (int) rand() % 40;
      strcpy(sentence, extraSentences[randSent]);
    }
    else {
      read(fdsToParent[i][READ], sentence, 128);
    }
    char middler[148];
    sprintf(middler, "%d: %s->\n", i+1, sentence);
    write(story, middler, strlen(middler));
    int letsChanged = editSentence(sentence, difficulty);
    if (! (difficulty == 'x' || difficulty == 'h')) {
      * changed[j] = letsChanged;
    }
    char middler2[148];
    sprintf(middler2, "%d: %s\n", i+1, sentence);
    write(story, middler2, strlen(middler2));
    write(fds[j][WRITE], sentence, sizeof(sentence));
  }
}

int main() {
  signal(SIGINT, sighandler);
  signal(SIGPIPE, sighandler);

  printf("Please indicate the number of players.\n");
  char playersStr[16];
  fgets(playersStr, 16, stdin);
  int numPlayers = atoi(playersStr);

  int numRounds = numPlayers;
  printf("How many rounds would you like to play? To get the generic amount just hit enter\n");
  char roundsStr[16];
  fgets(roundsStr, 16, stdin);
  if (strcmp(roundsStr, "\n")) {
    numRounds = atoi(roundsStr);
  }

  printf("What difficulty mode would you like? \nType e for easy (changing 1/4 of the letters)\nm for medium (changing 1/3 of the letters) \nh for hard (strfrying every word) \nand x for xtreme (strfrying the whole thing)\n");
  char difficultyStr[16];
  fgets(difficultyStr, 16, stdin);
  char difficulty = difficultyStr[0];

  int shm = shmget(KEY, sizeof(char), IPC_CREAT | 0666);
  char * diff = shmat(shm, 0, 0);
  * diff = difficulty;
  shmdt(diff);

  int shmArr[numPlayers];
  int * changed[numPlayers];
  for (int i = 0; i < numPlayers; i++) {
    shmArr[i] = shmget(KEY+i+1, sizeof(int), IPC_CREAT | 0666);
    changed[i] = shmat(shmArr[i], 0, 0);
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

  int story = open("story.txt", O_WRONLY | O_CREAT | O_APPEND, 0666);
  chmod("story.txt", 0666);

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
      transition(numPlayers, currRound, changed, story, fds, fdsToParent, difficulty, 0);
    }
    for (int currRound = numPlayers; currRound < numRounds; currRound++) {
      transition(numPlayers, currRound, changed, story, fds, fdsToParent, difficulty, 1);
    }
    for (int i = 0; i < numPlayers; i++) {
      shmdt(changed[i]);
      shmctl(shmArr[i], IPC_RMID, 0);
    }
    shmctl(shm, IPC_RMID, 0);
  }
  else {
    for (int i = 0; i < numPlayers; i++) {
      if (getpid() == childPids[i]) {
        char line[16];
        close(fds[i][WRITE]);
        read(fds[i][READ], line, sizeof(line));
        char numRoundsStr[16];
        sprintf(numRoundsStr, "%d", numRounds);
        write(to_client, numRoundsStr, 16);
        char iStr[16];
        sprintf(iStr, "%d", i);
        write(to_client, iStr, 16);
        for (int currRound = 0; currRound < numRounds; currRound++) {
          char sent[128] = "";
          char sentFromPar[128] = "";
          if (currRound != 0 && currRound % numPlayers == 0) {
            char randomSent[128] = "";
            read(fds[i][READ], randomSent, 128);
            write(to_client, randomSent, sizeof(randomSent));
          }
          read(from_client, sent, 128);
          if (currRound % numPlayers == numPlayers - 1) {
            char finaler[148];
            sprintf(finaler, "\n\nEND %d: %s", i+1, sent);
            write(story, finaler, strlen(finaler));
            char done[16] = "done";
            write(fdsToParent[i][WRITE], done, sizeof(done));
          }
          else {
            write(fdsToParent[i][WRITE], sent, sizeof(sent));
            read(fds[i][READ], sentFromPar, 128);
            write(to_client, sentFromPar, sizeof(sentFromPar));
          }
        }
      }
    }
  }
  return 0;
}
