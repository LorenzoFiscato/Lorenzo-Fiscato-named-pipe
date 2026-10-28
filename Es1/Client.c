#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define FIFO_PATH "/tmp/Es1_fifo"

int main() {
    if (mkfifo(FIFO_PATH, 0666) == -1) {
        if (errno != EEXIST) {
            perror("Errore mkfifo");
            exit(EXIT_FAILURE);
        }
    }

    char testo[100];
    printf("[Client] Messaggio da inviare: ");
    fflush(stdout);

    if (fgets(testo, sizeof(testo), stdin) != NULL) {
        testo[strcspn(testo, "\n")] = '\0';
    }

    int fd = open(FIFO_PATH, O_WRONLY);
    if (fd == -1) {
        perror("Errore open writer");
        exit(EXIT_FAILURE);
    }

    write(fd, testo, strlen(testo));
    close(fd);

    return 0;
}
