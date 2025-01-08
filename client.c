#include "pipe_networking.h"

int main() {
  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );

  char str[16];
  while (read(from_server, str, 16)) {
    printf("%s\n", str);
  }
  int pid = getpid();
  char pp[16];
  sprintf(pp, "%d", pid);
  remove(pp);
}
