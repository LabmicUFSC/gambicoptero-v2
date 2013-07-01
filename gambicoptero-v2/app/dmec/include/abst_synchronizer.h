/*
 * synchronizer.h
 *
 *  Created on: Mar 14, 2011
 *      Author: mateus
 */

#ifndef SYNCHRONIZER_H_
#define SYNCHRONIZER_H_

/* Local memory synchronization functions. By local memory it means that works
 * between PictureMotionEstimatorWorker threads, or
 * between PictureMotionEstimatorCoordinator threads, but not between
 * PictureMotionEstimatorWorker and PictureMotionEstimatorCoordinator threads
 * (supposing they are in distinct memory areas, which is the CELL case).
 * */

#include "traits.h"

#if LINUX
#include <pthread.h>
#include </usr/include/semaphore.h>

typedef sem_t Semaphore;
typedef pthread_mutex_t Mutex;

#else

#include <semaphore.h>
#include <mutex.h>

typedef System::Semaphore Semaphore;
typedef System::Mutex Mutex; 

#endif

void synchronizer_mutex_setUp(Mutex * mt);
void synchronizer_mutex_lock(Mutex * mt);
void synchronizer_mutex_unlock(Mutex * mt);
void synchronizer_mutex_tearDown(Mutex * mt);

void synchronizer_semaphore_setUp(Semaphore * sm, unsigned int value);
void synchronizer_semaphore_wait(Semaphore * sm);
void synchronizer_semaphore_signal(Semaphore * sm);
void synchronizer_semaphore_tearDown(Semaphore * sm);


#endif /* SYNCHRONIZER_H_ */

