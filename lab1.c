#include <cstdio>
#include <pthread.h>

void *func(void *param) {

    for(int i = 0; i < 10; i++) {
        printf("child line %d\n", i);
    }

	return nullptr;
}

int main() {

    pthread_t th;

    if (pthread_create(&th, nullptr, func, nullptr) != 0) {
        printf("error while creating a thread\n");
        return 0;
    }

    for(int i = 0; i < 10; i++) {
        printf("parent line %d\n", i);
    }

    pthread_exit(nullptr);
    return 0;
}
