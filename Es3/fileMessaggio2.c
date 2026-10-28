#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#define MY_FIFO "/tmp/Es3_fifo2"
#define OTHER_FIFO "/tmp/Es3_fifo1"

void leggiMessaggio() {
    char testo[100];
    int fdR = open(OTHER_FIFO, O_RDONLY);
    if (fdR == -1) {
        if (errno != ENOENT) {
            perror("Errore open reader");
            exit(EXIT_FAILURE);
        }
        return;
    }
    ssize_t n = read(fdR, testo, sizeof(testo) - 1);

    if (n > 0) {
        testo[n] = '\0';

        if (strcmp(testo, "EXIT") == 0) {
            printf("[programma2] Comando 'EXIT' ricevuto. Chiusura in corso...\n");
            close(fdR);

            int fdW_exit = open(MY_FIFO, O_WRONLY | O_NONBLOCK);
            if (fdW_exit != -1) {
                write(fdW_exit, "EXIT", 4);
                close(fdW_exit);
            }

            if (unlink(MY_FIFO) == -1) {
                perror("Errore unlink");
                exit(EXIT_FAILURE);
            }

            printf("[programma2] rimossa pipe dal file system\n");
            exit(EXIT_SUCCESS);
        }

        printf("[programma2] testo ricevuto: %s\n", testo);
    } else if (n == 0) {
        printf("[programma2] il client si è disconnesso senza inviare dati\n");
    } else {
        perror("Errore read");
    }

    close(fdR);
}

void inviaMessaggio() {
    char testo[100];
    int fdW;
    printf("[Programma2] Messaggio da inviare: ");
    fflush(stdout);

    if (fgets(testo, sizeof(testo), stdin) != NULL) {
        testo[strcspn(testo, "\n")] = '\0';
    }

    fdW = open(MY_FIFO, O_WRONLY);
    if (fdW == -1) {
        perror("Errore open writer");
        return;
    }

    ssize_t n = write(fdW, testo, strlen(testo));
    if (n == -1) {
        if (errno == EPIPE)
            fprintf(stderr, "Nessun reader: canale chiuso.\n");
        else
            perror("Errore write");
    }
    close(fdW);
}

int main() {
    signal(SIGPIPE, SIG_IGN);

    if (mkfifo(MY_FIFO, 0666) == -1 && errno != EEXIST) {
        perror("Errore mkfifo FIFO1");
        exit(EXIT_FAILURE);
    }

    while (1) {
        leggiMessaggio();
        inviaMessaggio();
    }

    return 0;
}
