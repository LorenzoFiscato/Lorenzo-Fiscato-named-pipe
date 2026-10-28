#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define FIFO_PATH "/tmp/Es1_fifo"

int main () {
    char testo[100];

    int fd = open(FIFO_PATH, O_RDONLY);
    if (fd == -1) {
        perror("Errore open reader");
        exit(EXIT_FAILURE);
    }

    ssize_t n = read(fd, testo, sizeof(testo) - 1);
    if (n > 0) {
        testo[n] = '\0';
        printf("[server] testo ricevuto: %s\n", testo);
    } else if (n == 0) {
        printf("[server] il writer ha chiuso senza inviare dati\n");
    } else {
        perror("Errore read");
    }

    close(fd);

    if (unlink(FIFO_PATH) == -1) {
        perror("Errore unlink");
    } else {
        printf("[server] rimossa pipe dal file system\n");
    }

    return 0;
}
