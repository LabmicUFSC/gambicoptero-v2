#ifndef PTHREAD_H
#define PTHREAD_H

#include <thread.h>
#include <mutex.h>
#include <condition.h>
#include "epos_wrappers.h"

// pthread declartions

typedef System::Thread pthread_t;
typedef int pthread_attr_t;
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, int (*start_routine)(void*), void *arg);
int pthread_join(pthread_t thread, void **value_ptr);

// pthread mutex declarations

typedef System::Mutex pthread_mutex_t;
typedef int pthread_mutexattr_t;
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
int pthread_mutex_destroy(pthread_mutex_t *mutex);
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);

// pthread condition declarations

typedef System::Condition pthread_cond_t;
typedef int pthread_condattr_t;
int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);
int pthread_cond_destroy(pthread_cond_t *cond);
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_broadcast(pthread_cond_t *cond);

// pthread barrier declarations

typedef struct {
    int needed;
    int called;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} pthread_barrier_t;

typedef int pthread_barrierattr_t;
int pthread_barrier_destroy(pthread_barrier_t *barrier);
int pthread_barrier_init(pthread_barrier_t *barrier, const pthread_barrierattr_t *attr, unsigned count);
int pthread_barrier_wait(pthread_barrier_t *barrier);

#endif
