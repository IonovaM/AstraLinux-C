#include <pthread.h>
#include <stdio.h>

struct ThreadData {
    int value1;
    int value2;
    int PROC;
    int FLAG;
};

void* threadFunction1(void* arg);
void* threadFunction2(void* arg);
void* threadFunction3(void* arg);

void* threadFunction1(void* arg) {
    struct ThreadData* data = (struct ThreadData*)arg;

    int isFirstTime = 1;
    while (1){
        if (data->PROC == 1){
        if (data->FLAG == 1 && isFirstTime) {
            printf("1.Лейтенант Смирнов получил значение: %d, %d\n", data->value1, data->value2);
            isFirstTime = 0;
            data->PROC = 2;
        } else if (data->FLAG != 1) {
            printf("6.Лейтенант Смирнов получил значение: %d\n", data->value1);
            break;
        }}
    }

    pthread_exit(NULL);
}

void* threadFunction2(void* arg) {
    struct ThreadData* data = (struct ThreadData*)arg;
    int isFirstTime = 1;
    while (1){
        if (data->PROC == 2){
        if (data->FLAG == 1 && isFirstTime) {
            printf("2.Сержанта Петров получил задание от Смирнова: %d, %d\n", data->value1, data->value2);
            isFirstTime = 0;
            data->PROC = 3;
        } else {
            printf("5.Сержант Петров получил ответ от Иванова: %d\n", data->value1);
            data->PROC = 1;
            break;
        }}
    }
    pthread_exit(NULL);
}

void* threadFunction3(void* arg) {
    struct ThreadData* data = (struct ThreadData*)arg;
    if (data->FLAG == 1) {
        printf("3.Рядовой Иванов получил задание от сержанта Петрова: %d, %d\n", data->value1, data->value2);
        data->value1 *= data->value2;
        printf("4.Рядовой Иванов выполнил задание от сержанта Петрова: %d\n", data->value1);
        data->FLAG = 0;
        data->PROC = 2;
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t tid[3];
    struct ThreadData data_to_pass;

    data_to_pass.value1 = 42;
    data_to_pass.value2 = 10;
    data_to_pass.FLAG = 1;
    data_to_pass.PROC = 1;
    printf("Командиру воинской части полковнику Кузнецову требуется\nперемножить два секретных числа: %d, %d\n", data_to_pass.value1, data_to_pass.value2);

    pthread_create(&tid[0], NULL, threadFunction1, (void*)&data_to_pass);
    pthread_create(&tid[1], NULL, threadFunction2, (void*)&data_to_pass);
    pthread_create(&tid[2], NULL, threadFunction3, (void*)&data_to_pass);

    pthread_join(tid[2], NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[0], NULL);

    printf("Полковник Кузнецов получил ответ: %d\n", data_to_pass.value1);

    return 0;
}
