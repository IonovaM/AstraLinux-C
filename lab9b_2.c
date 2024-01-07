#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main() {
    const char *fifo_path = "fifoS1toS2";
    mkfifo(fifo_path, 0666);

    // Открываем канал для чтения
    int fifo_fd = open(fifo_path, O_RDONLY);

    char S1[256];
    read(fifo_fd, S1, sizeof(S1));

    // Открываем канал для записи
    fifo_fd = open(fifo_path, O_WRONLY);

    char S2[256];

    printf("Введите строку S2: ");
    fgets(S2, sizeof(S2), stdin);

    size_t len_S2 = strlen(S2);
    if (S2[len_S2 - 1] == '\n')
        S2[--len_S2] = '\0';

    write(fifo_fd, S2, len_S2);
    close(fifo_fd);

    printf("Символы, содержащиеся в S2 и не содержащиеся в S1: ");
    for (int i = 0; S2[i] != '\0'; ++i) {
        if (strchr(S1, S2[i]) == NULL) {
            printf("%c ", S2[i]);
        }
    }
    printf("\n");

    unlink(fifo_path);

    return 0;
}
