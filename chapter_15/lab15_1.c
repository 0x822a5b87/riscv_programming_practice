#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int x;
int y;

void *thread_func_1(void *arg) {
    x = 1;
    int a = y;
    printf("x = %d, y = %d, a = %d\n", x, y, a);
    return NULL;
}

void *thread_func_2(void *arg) {
    y = 1;
    int b = x;
    printf("x = %d, y = %d, b = %d\n", x, y, b);
    return NULL;
}

int main() {
    pthread_t tid1;
    int ret = pthread_create(&tid1, NULL, thread_func_1, NULL);
    if (ret != 0) {
        fprintf(stderr, "Failed to create thread: %d\n", ret);
        return 1;
    }

    pthread_t tid2;
    ret = pthread_create(&tid2, NULL, thread_func_2, NULL);
    if (ret != 0) {
        fprintf(stderr, "Failed to create thread: %d\n", ret);
        return 1;
    }

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    printf("Main thread: Child thread finished\n");
    return 0;
}
