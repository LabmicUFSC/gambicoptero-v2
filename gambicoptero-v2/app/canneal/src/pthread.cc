/* Pthread wrappers for EPOS thread, mutex, and condition variable
 * Author: Giovani Gracioli <giovani@lisha.ufsc.br>
 */

#include "../include/pthread.h"

#include <utility/ostream.h>

extern System::OStream cout;

// pthread implementation

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, int (*start_routine)(void*), void *arg)
{
    thread = new System::Thread(start_routine, arg);
    return 0;
}

int pthread_join(pthread_t thread, void **value_ptr)
{
    cout << "pthread_join()" << endl;
    int status = thread.join();
    cout << "pthread_join() status = " << status << endl;
    return status;
}

//pthread mutex implementation

int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
    if(mutex == NULL)
        mutex = new System::Mutex();
    return 0;
}

int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    if(mutex != NULL) {
        mutex->~Mutex();
        mutex = NULL;
    }
    return 0;
}
int pthread_mutex_lock(pthread_mutex_t *mutex)
{
    mutex->lock();
    return 0;
}

int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
    mutex->lock();
    return 0;
}

int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
    mutex->unlock();
    return 0;
}

// pthread condition implementation

int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr)
{
    if(cond == NULL)
        cond = new System::Condition();
    return 0;
}

int pthread_cond_destroy(pthread_cond_t *cond)
{
    if(cond != NULL) {
        cond->~Condition();
        cond = NULL;
    }
    return 0;
}

int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
    mutex->unlock();
    cond->wait();
    return 0;
}

int pthread_cond_signal(pthread_cond_t *cond)
{
    cond->signal();
    return 0;
}

int pthread_cond_broadcast(pthread_cond_t *cond)
{
    cond->broadcast();
    return 0;
}

//pthread barrier implementation

int pthread_barrier_init(pthread_barrier_t *barrier, const pthread_barrierattr_t *attr, unsigned count)
{
    barrier->needed = count;
    barrier->called = 0;
    pthread_mutex_init(&barrier->mutex,NULL);
    pthread_cond_init(&barrier->cond,NULL);
    return 0;
}

int pthread_barrier_destroy(pthread_barrier_t *barrier)
{
    pthread_mutex_destroy(&barrier->mutex);
    pthread_cond_destroy(&barrier->cond);
    return 0;
}

int pthread_barrier_wait(pthread_barrier_t *barrier)
{
    pthread_mutex_lock(&barrier->mutex);
    barrier->called++;
    if (barrier->called == barrier->needed) {
        barrier->called = 0;
        //cout << "pthread_barrier_wait() - calling cond broadcast\n";
        System::Thread::yield();
        pthread_cond_broadcast(&barrier->cond);
    } else {
        //cout << "pthread_barrier_wait() - calling cond wait\n";
        pthread_cond_wait(&barrier->cond,&barrier->mutex);
    }
    pthread_mutex_unlock(&barrier->mutex);
    return 0;
}
