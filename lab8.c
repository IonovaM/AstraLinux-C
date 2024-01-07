#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

double calculate(int a, int b, int c, int d, int e, int f, int g, int h, int k, int m) {
    return (double) a/b + c*d + (e+f)*(g+h) + k*m;
}

int main() {
    FILE *file;
    srand(time(NULL));
    signal(SIGCHLD, SIG_IGN);
    file = fopen("input.txt", "r");
    if (!file) {
        fprintf(stderr, "Ошибка открытия файла\n");
        return 1;
    }

    char line[100];
    int processNumber = 1;

    while (fgets(line, sizeof(line), file) != NULL) {
        pid_t pid = fork();
        //srand(getpid());
        int delay = rand()%8;
        if (pid == -1){
        fprintf(stderr, "Ошибка при создании процесса\n");
        return 1;
        } else if (pid == 0) {
            sleep(delay);
            int a, b, c, d, e, f, g, h, k, m;
            sscanf(line, "%d %d %d %d %d %d %d %d %d %d", &a, &b, &c, &d, &e, &f, &g, &h, &k, &m);
            //printf("Процесс #%d  начал обработку. (PID: %i)\n", processNumber, getpid());
            double result = calculate(a, b, c, d, e, f, g, h, k, m);
            printf("Входные данные: %d %d %d %d %d %d %d %d %d %d -> Результат: %.2f\n", a, b, c, d, e, f, g, h, k, m, result);
            printf("Процесс #%d завершил обработку. (PID: %d) Задержка выполнения: %d сек\n", processNumber, getpid(), delay);
            return 0;
        }
        processNumber++;
    }
    int status;
    wait(&status);
    /*
    for (int i=0; i<processNumber-1; i++){
    wait(&status);
    }*/
    fclose(file);
    return 0;
}
