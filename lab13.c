#include <stdio.h>
#include <pthread.h>

#define CHILD_FINISHED  0
#define PARENT_FINISHED  1

typedef struct Context {
    pthread_mutex_t* cond_mutex;
    pthread_cond_t* cond;
    int printed_thread_value;
} Context;

Context* initContext() {
    Context* ctx = (Context*) malloc(sizeof(Context));
    ctx->cond_mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
    ctx->cond = (pthread_cond_t*) malloc(sizeof(pthread_cond_t));

    pthread_mutex_init(ctx->cond_mutex, NULL);
    pthread_cond_init(ctx->cond, NULL);
    ctx->printed_thread_value = CHILD_FINISHED;

    return ctx;
}

void deleteContext(Context* ctx) {
    pthread_cond_destroy(ctx->cond);
    pthread_mutex_destroy(ctx->cond_mutex);

    free(ctx->cond);
    free(ctx->cond_mutex);

    free(ctx);
}

void *childFunc(void *param) {
    Context* ctx = (Context*) param;

    for(int i = 0; i < 10; i++) {
        pthread_mutex_lock(ctx->cond_mutex);

        if (ctx->printed_thread_value != PARENT_FINISHED) {
            pthread_cond_wait(ctx->cond, ctx->cond_mutex);
        }

        printf("child line %d\n", i);

        ctx->printed_thread_value = CHILD_FINISHED;
        pthread_cond_signal(ctx->cond);

        pthread_mutex_unlock(ctx->cond_mutex);
    }

    return NULL;
}

int main() {

    Context* ctx = initContext();
    pthread_t child;

    if (pthread_create(&child, NULL, childFunc, (void*)ctx) != 0) {
        printf("error while creating a thread\n");
        return 0;
    }

    for(int i = 0; i < 10; i++) {
        pthread_mutex_lock(ctx->cond_mutex);

        if (ctx->printed_thread_value != CHILD_FINISHED) {
            pthread_cond_wait(ctx->cond, ctx->cond_mutex);
        }

        printf("parent line %d\n", i);

        ctx->printed_thread_value = PARENT_FINISHED;
        pthread_cond_signal(ctx->cond);

        pthread_mutex_unlock(ctx->cond_mutex);
    }

    pthread_join(child, NULL);
    deleteContext(ctx);
    pthread_exit(NULL);
}
