#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_SIZE 10

int main() {
    int n, A[MAX_SIZE][MAX_SIZE];
    printf("Введите размерность матрицы (n): ");
    scanf("%d", &n);

    if (n <= 0 || n > MAX_SIZE) {
        printf("Некорректный размер матрицы.\n");
        return -1;
    }

    printf("Введите элементы матрицы A размерности %dx%d:\n", n, n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            scanf("%d", &A[i][j]);
        }
    }

    int fd[2];
    if (pipe(fd) < 0) {
        printf("Ошибка создания канала.\n");
        return -1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        printf("Ошибка при создании процесса.\n");
        return -1;
    } else if (pid == 0) {
        int sum1 = 0;
        for (int i = 0; i < n; ++i) {
            sum1 += A[i][i];
        }
        write(fd[1], &sum1, sizeof(sum1)); 
        close(fd[1]); 
        exit(0);
    } else {
        pid_t pid2 = fork();
        if (pid2 < 0) {
            printf("Ошибка при создании второго процесса.\n");
            return -1;
        } else if (pid2 == 0) {
            int sum2 = 0;
            for (int i = 0; i < n; ++i) {
                sum2 += A[i][n - i - 1];
            }
            write(fd[1], &sum2, sizeof(sum2)); 
            close(fd[1]); 
            exit(0);
        } else {
            //while(wait(NULL) > 0);
            int status;
            waitpid(pid, &status, 0);
            waitpid(pid2, &status, 0); 

            int sum1, sum2;
            //close(fd[1]); 

            read(fd[0], &sum1, sizeof(sum1));
            read(fd[0], &sum2, sizeof(sum2));
            close(fd[0]); 

            if (sum1 == sum2) {
                printf("Суммы двух главных диагоналей равны: %d\n", sum1);
            } else {
                printf("Суммы двух главных диагоналей не равны.\n");
            }
        }
    }

    return 0;
}
