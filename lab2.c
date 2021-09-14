#include <stdio.h>
#include <pthread.h>

void *func(void *param) {

    for(int i = 0; i < 10; i++) {
        printf("child line %d\n", i);
    }

	return NULL;
}

int main() {

    pthread_t th;

    if (pthread_create(&th, NULL, func, NULL) != 0) {
        printf("error while creating a thread\n");
        return 0;
    }

    pthread_join(th, NULL);

    for(int i = 0; i < 10; i++) {
        printf("parent line %d\n", i);
    }

    pthread_exit(NULL);
    return 0;
}
