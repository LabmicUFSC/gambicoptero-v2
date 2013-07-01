/*
 * pc_synchronizer.c [For PC multi thread version]
 *
 * This is a implementation of synchronization_manager.h
 * It shall not be confused with an implementation of abst/synchronizer.h
 *
 *  Created on: Mar 11, 2011
 *      Author: mateus
 */

#include "../include/synchronization_manager.h"
#include "../include/traits.h"
#include "../include/error.h"

#include <semaphore.h>
#include <mutex.h>
#include <utility/malloc.h>
#include <utility/ostream.h>

/* Protected attributes for synchronization -------------------------------- */
//sem_t sm_coordinatorDataReady[NUM_WORKERS];
typedef System::Semaphore Semaphore;
Semaphore *sm_coordinatorDataReady[NUM_WORKERS];
typedef System::OStream OStream;
extern OStream cout;

//pthread_mutex_t mt_workersMustWork[NUM_WORKERS];
typedef System::Mutex Mutex;
Mutex *mt_workersMustWork[NUM_WORKERS];
bool _workersMustWork[NUM_WORKERS];

Semaphore *sm_workerResultsDone[NUM_WORKERS];

Semaphore *sm_matchStart[NUM_WORKERS];


/* --------------------------------------------------------------------------*/

void setUpSynchronizationManager(unsigned int numberOfWorkers)
{
	unsigned int i;
	for (i = 0; i < numberOfWorkers; i++) {
		//sem_init(&sm_coordinatorDataReady[i], PTHREAD_PROCESS_PRIVATE, 0);
		sm_coordinatorDataReady[i] = new Semaphore(0);

		_workersMustWork[i] = true;
		//sem_init(&sm_workersMustWork[i], PTHREAD_PROCESS_PRIVATE, 0);
		//pthread_mutex_init(&mt_workersMustWork[i], NULL);
		mt_workersMustWork[i] = new Mutex();

		//sem_init(&sm_workerResultsDone[i], PTHREAD_PROCESS_PRIVATE, 0);
		sm_workerResultsDone[i] = new Semaphore(0);

		//sem_init(&sm_matchStart[i], PTHREAD_PROCESS_PRIVATE, 0);
		sm_matchStart[i] = new Semaphore(0);
	}
}


void tearDownSynchronizationManager(unsigned int numberOfWorkers)
{
	unsigned int i;
	for (i = 0; i < numberOfWorkers; i++) {
		//sem_destroy(&sm_coordinatorDataReady[i]);
		//cout << "tearDownSynchronizationManager delete 1";
		delete sm_coordinatorDataReady[i];

		//sem_destroy(&sm_workersMustWork[i]);
		//pthread_mutex_destroy(&mt_workersMustWork[i]);
        //cout << "tearDownSynchronizationManager delete 2";
		delete mt_workersMustWork[i];

		//sem_destroy(&sm_workerResultsDone[i]);
        //cout << "tearDownSynchronizationManager delete 3";
		delete sm_workerResultsDone[i];

		//sem_destroy(&sm_matchStart[i]);
        //cout << "tearDownSynchronizationManager delete 4";
		delete sm_matchStart[i];
	}
}


void synchronizationManager_signalMatchStart(unsigned long long workerID, spe_context_ptr_t speWorkerContext)
{
	//sem_post(&sm_matchStart[workerID]);
	sm_matchStart[workerID]->v();
}


void synchronizationManager_setWorkerMustWork(unsigned long long workerID,
		spe_context_ptr_t speWorkerContext,
		bool mustWork)
{
	int id = (int) workerID;

	//pthread_mutex_lock(&mt_workersMustWork[id]);
	mt_workersMustWork[id]->lock();
	_workersMustWork[id] = mustWork;
	//pthread_mutex_unlock(&mt_workersMustWork[id]);
	mt_workersMustWork[id]->unlock();
}


void synchronizationManager_signalCoordinatorDataReady(unsigned long long workerID,
		int speWorkerContext)
{
	int id = (int) workerID;

	//sem_post(&sm_coordinatorDataReady[id]);
	sm_coordinatorDataReady[id]->v();
}

void synchronizationManager_waitWorkerResultsDone(unsigned long long workerID,
		int speWorkerContext)
{
	int id = (int) workerID;

	//sem_wait(&sm_workerResultsDone[id]);
	sm_workerResultsDone[id]->p();
}

void setUpSynchronizationManagerWorkerSide()
{
}

void synchronizationManager_workerWaitMatchStart(unsigned long long workerID)
{
	//sem_wait(&sm_matchStart[workerID]);
	sm_matchStart[workerID]->p();
}


bool synchronizationManager_workerMustWork(unsigned long long workerID)
{
	bool workerMustWork;

	//pthread_mutex_lock(&mt_workersMustWork[workerID]);
	mt_workersMustWork[workerID]->lock();
	workerMustWork = _workersMustWork[workerID];
	//pthread_mutex_unlock(&mt_workersMustWork[workerID]);
	mt_workersMustWork[workerID]->unlock();

	return workerMustWork;
}

void synchronizationManager_waitCoordinatorDataReady(unsigned long long workerID)
{
	//sem_wait(&sm_coordinatorDataReady[workerID]);
	sm_coordinatorDataReady[workerID]->p();
}

void synchronizationManager_signalWorkerResultsDone(unsigned long long workerID)
{
	//sem_post(&sm_workerResultsDone[workerID]);
	sm_workerResultsDone[workerID]->v();
}
