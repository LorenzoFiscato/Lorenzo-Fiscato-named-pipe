#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#define FIFO_PATH "/tmp/Es2_fifo"

int main() {
    signal(SIGPIPE, SIG_IGN);
    int fd;

    if (mkfifo(FIFO_PATH, 0666) == -1) {
        if (errno != EEXIST) {
            perror("Errore mkfifo");
            exit(EXIT_FAILURE);
        }
    }

    char testo[100];
    while (1) {
        printf("[Client] Messaggio da inviare: ");
        fflush(stdout);

        if (fgets(testo, sizeof(testo), stdin) != NULL) {
            testo[strcspn(testo, "\n")] = '\0';
        }

        fd = open(FIFO_PATH, O_WRONLY);
        if (fd == -1) {
            perror("Errore open writer");
            exit(EXIT_FAILURE);
        }

        ssize_t n = write(fd, testo, strlen(testo));
        if (n == -1) {
            if (errno == EPIPE)
                fprintf(stderr, "Nessun reader: canale chiuso.\n");
            else
                perror("Errore write");
        }
    }

    close(fd);

    return 0;
}
