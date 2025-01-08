#include "pipe_networking.h"

static void sighandler(int signo) {
  if (signo == SIGINT) {
    remove("./toServer");
    exit(1);
  }
  if (signo == SIGPIPE) {
    //printf("In sigpipe\n");
  }
}

int main() {
  signal(SIGINT, sighandler);
  signal(SIGPIPE, sighandler);
  srand(time(NULL));

  //printf("1st spot\n");
  int to_client;
  int from_client;

  int x = 0;
  char str[16];

  while (1) {
    from_client = server_setup();
    remove(WKP);
    int p;
    p = fork();
    if (p) {
      //RESET TO TOP
    }
    else {
      server_handshake_half(& to_client, from_client);

      //printf("hi 2nd\n");
      while (1) {
        x = (int) rand() % 100 + 1;
        sprintf(str, "%d", x);

        if (write(to_client, str, 16) == -1) {
          break;
        }
        //printf("2nd spot\n");
        sleep(1);
      }
      //printf("3rd spot\n");
      close(to_client);
      close(from_client);
      //printf("4th spot\n");
    }
  }
  return 0;
}
