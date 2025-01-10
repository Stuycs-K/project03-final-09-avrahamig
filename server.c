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

  while (players < numPlayers) {
    from_client = server_setup();
    remove(WKP);
    p = fork();
    if (p) {
      childPids[players] = p;
      players++;
    }
    else {
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
    }
  }
  if (p) {
    for (int i = 0; i < numPlayers; i++) {
      printf("childPids i: %d\n", childPids[i]);
    }
  }










  return 0;
}
