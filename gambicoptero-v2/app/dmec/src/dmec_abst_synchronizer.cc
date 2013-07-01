/*
 * synchronizer.c
 * PC Synchronizer.
 *
 *  Created on: Mar 14, 2011
 *      Author: mateus
 */

#include "../include/abst_synchronizer.h"
#include <utility/malloc.h>


void synchronizer_mutex_setUp(Mutex * mt)
{
	//pthread_mutex_init(mt, NULL);
}


void synchronizer_mutex_tearDown(Mutex * mt)
{
	//pthread_mutex_destroy(mt);
	delete mt;
}


void synchronizer_mutex_lock(Mutex * mt)
{
	//pthread_mutex_lock(mt);
	mt->lock();
}


void synchronizer_mutex_unlock(Mutex * mt)
{
	//pthread_mutex_unlock(mt);
	mt->unlock();
}



void synchronizer_semaphore_setUp(Semaphore * sm, unsigned int value)
{
	//sem_init(sm, PTHREAD_PROCESS_PRIVATE, value);
}

void synchronizer_semaphore_wait(Semaphore * sm)
{
	//sem_wait(sm);
	sm->p();
}

void synchronizer_semaphore_signal(Semaphore * sm)
{
	//sem_post(sm);
	sm->v();
}


void synchronizer_semaphore_tearDown(Semaphore * sm)
{
	//sem_destroy(sm);
	delete sm;
}
