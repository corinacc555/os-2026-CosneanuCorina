#include "kernel/types.h"
#include "user/user.h"

// Cele trei etape ale shell-ului în xv6: echo hello de la copil
int main(int argc, char *argv[]) {
  printf("parent: my pid is %d\n", getpid());
  
  int pid = fork();                     // Pasul 1: copiem procesul
  if (pid < 0) {
    printf("fork failed\n");
    exit(1);
  }

  if (pid == 0) {                         // Executat doar de copil
    printf("child: my pid is %d\n", getpid());
    char *args[] = { "echo", "hello", "from", "the", "child", 0 };
    exec("echo", args);                 // Pasul 2: devine echo
    printf("exec failed\n");            // Se execută doar dacă exec eșuează
    exit(1);
  }

  int status;
  wait(&status);                        // Pasul 3: așteptăm copilul
  printf("parent: child %d ended with status %d\n", pid, status);
  exit(0);
}
