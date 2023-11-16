#ifndef ELF_THREAD_POOL_H
#define ELF_THREAD_POOL_H

#include <pthread.h>

typedef struct ElfThreadPool
{
    int num_threads;
    pthread_t *threads;
} ElfThreadPool;

void elf_thread_pool_add_work(
    ElfThreadPool *thread_pool,
    void *function,
    void *arguments);

#endif
