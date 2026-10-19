#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/* Face exact același lucru ca comanda: ls /etc | wc -l */
int main(void) {
    int p[2];
    if (pipe(p) < 0) { perror("pipe"); return 1; }

    if (fork() == 0) {                           /* Primul copil: ls /etc */
        close(1);
        dup(p[1]);                               /* Descriptorul 1 devine p[1] */
        close(p[0]);
        close(p[1]);
        execlp("ls", "ls", "/etc", (char *)NULL);
        perror("exec ls");
        exit(127);
    }

    if (fork() == 0) {                           /* Al doilea copil: wc -l */
        close(0);
        dup(p[0]);                               /* Descriptorul 0 devine p[0] */
        close(p[0]);
        close(p[1]);
        execlp("wc", "wc", "-l", (char *)NULL);
        perror("exec wc");
        exit(127);
    }

    close(p[0]);                                 /* Părintele închide ambele capete */
    close(p[1]);
    wait(NULL);
    wait(NULL);
    return 0;
}
