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

  from_server = client_handshake( &to_server );
  
  while (1) {
    char pid[32] = "my pid is ";
    char id[16] = "";
    sprintf(id, "%d", getpid());
    strcat(pid, id);
    write(to_server, pid, strlen(pid));
    
    char str[16];
    read(from_server, str, 16);
    printf("Returned: %s\n", str);
    sleep(1);
  }
}
