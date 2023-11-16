#ifndef THREAD_POOL_H
#define THREAD_POOL_H

typedef struct ThreadPool
{
    int num_threads;
} ThreadPool;

void thread_pool_add_work(
    ThreadPool thread_pool, 
    void *function, 
    void *arguments
);

#endif
