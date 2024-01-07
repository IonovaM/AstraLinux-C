#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <limits.h>

#define MAX_SIZE 100

int main() {
    int n;

    printf("Введите размерность матрицы: ");
    scanf("%d", &n);

    // Создание разделяемой памяти для хранения матрицы А
    int shmid = shmget(IPC_PRIVATE, MAX_SIZE * MAX_SIZE * sizeof(int), IPC_CREAT | 0666);
    if (shmid < 0) {
        fprintf(stderr, "Ошибка создания разделяемой памяти\n");
        return 1;
    }

    int* shared_memory = (int *)shmat(shmid, NULL, 0);

    printf("Введите элементы матрицы А размерности %d x %d:\n", n, n);
    for (int i = 0; i < n * n; ++i) {
        scanf("%d", &shared_memory[i]);
    }

    pid_t pid;
    int maxDiagonalValue = INT_MIN;
    int minSecondaryDiagonalValue = INT_MAX;
    int maxDiagonalRowIndex = -1;
    int minSecondaryDiagonalColumnIndex = -1;

    for (int i = 0; i < n; ++i) {
        pid = fork();

        if (pid < 0) {
            fprintf(stderr, "Ошибка создания процесса\n");
            return 1;
        } else if (pid == 0) { // Дочерний процесс
            int localMaxDiagonalValue = INT_MIN;
            int localMinSecondaryDiagonalValue = INT_MAX;
            int localMaxDiagonalRowIndex = -1;
            int localMinSecondaryDiagonalColumnIndex = -1;

            for (int j = 0; j < n; ++j) {
                if (shared_memory[i * n + j] >= localMaxDiagonalValue && i == j) {
                    localMaxDiagonalValue = shared_memory[i * n + j];
                    localMaxDiagonalRowIndex = i;
                }

                if (shared_memory[i * n + j] <= localMinSecondaryDiagonalValue && i + j == n - 1) {
                    localMinSecondaryDiagonalValue = shared_memory[i * n + j];
                    localMinSecondaryDiagonalColumnIndex = j;
                }
            }

            // Запись результатов в разделяемую память для обмена информацией с главным процессом
            shared_memory[i * MAX_SIZE + 0] = localMaxDiagonalRowIndex;
            shared_memory[i * MAX_SIZE + 1] = localMaxDiagonalValue;
            shared_memory[i * MAX_SIZE + 2] = localMinSecondaryDiagonalColumnIndex;
            shared_memory[i * MAX_SIZE + 3] = localMinSecondaryDiagonalValue;

            exit(0);
        }
    }

    for (int i = 0; i < n; ++i) {
        int status;
        wait(&status);

        int localMaxDiagonalRowIndex = shared_memory[i * MAX_SIZE + 0];
        int localMaxDiagonalValue = shared_memory[i * MAX_SIZE + 1];
        int localMinSecondaryDiagonalColumnIndex = shared_memory[i * MAX_SIZE + 2];
        int localMinSecondaryDiagonalValue = shared_memory[i * MAX_SIZE + 3];

        if (localMaxDiagonalValue > maxDiagonalValue) {
            maxDiagonalValue = localMaxDiagonalValue;
            maxDiagonalRowIndex = localMaxDiagonalRowIndex;
        }

        if (localMinSecondaryDiagonalValue < minSecondaryDiagonalValue) {
            minSecondaryDiagonalValue = localMinSecondaryDiagonalValue;
            minSecondaryDiagonalColumnIndex = localMinSecondaryDiagonalColumnIndex;
        }
    }

    printf("Индекс строки с наибольшим элементом на главной диагонали: %d\n", maxDiagonalRowIndex+1);
    printf("Индекс столбца с наименьшим элементом на побочной диагонали: %d\n", minSecondaryDiagonalColumnIndex+1);

    int sumOfIndices = maxDiagonalRowIndex + minSecondaryDiagonalColumnIndex+2;
    printf("Сумма индексов: %d\n", sumOfIndices);

    // Освобождение разделяемой памяти
    shmdt(shared_memory);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
