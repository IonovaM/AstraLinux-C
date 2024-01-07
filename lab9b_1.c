#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main() {
    const char *fifo_path = "fifoS1toS2";
    mkfifo(fifo_path, 0666);

    int fifo_fd = open(fifo_path, O_WRONLY);

    char S1[256], S2[256];

    printf("Введите строку S1: ");
    fgets(S1, sizeof(S1), stdin);

    size_t len_S1 = strlen(S1);
    if (S1[len_S1 - 1] == '\n')
        S1[--len_S1] = '\0';

    write(fifo_fd, S1, len_S1);
    close(fifo_fd);

    // Открытие канала для чтения ответа от второго приложения
    fifo_fd = open(fifo_path, O_RDONLY);

    int bytes_read = 0;
    while (bytes_read == 0) {
        bytes_read = read(fifo_fd, S2, sizeof(S2));
    }

    printf("Символы, содержащиеся в S1 и не содержащиеся в S2: ");
    for (int i = 0; i < len_S1; ++i) {
        if (strchr(S2, S1[i]) == NULL) {
            printf("%c ", S1[i]);
        }
    }
    printf("\n");

    close(fifo_fd);
    unlink(fifo_path);

    return 0;
}
