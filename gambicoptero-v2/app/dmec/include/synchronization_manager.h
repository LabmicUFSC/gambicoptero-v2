
/*!
 ******************************************************************************
 * \file
 *
 * \brief
 *    - Methods for synchronization between the coordinator and workers modules.
 *
 * \author
 *      - Alexandre Massayuki Okazaki   (alexandre@lisha.ufsc.br)
 *      - Mateus Krepsky Ludwich        (mateus@lisha.ufsc.br)
 *      - Tiago de Albuquerque Reis      (reis@lisha.ufsc.br)
 *
 ******************************************************************************
 */

/* The interaction between coordinator and workers occurs in the following way:
   ** Coordinator

    + match {
        fill shared data()
        synchronizationManager_signalCoordinatorDataReady()
        for each Worker/WorkerProxy: synchronizationManager_waitWorkerResultsDone(workerID)
        if there is no more data, for each worker: synchronizationManager_setWorkerMustWork(workerID, false)
    }

    ** Worker
    
    + run {
        while (synchronizationManager_workerMustWork()) {
            synchronizationManager_waitCoordinatorDataReady()
            gather shared data()
            compute results()
            fill shared results()
            synchronizationManager_signalWorkerResultsDone(workerID)
        }
    }

*/

#ifndef SYNCHRONIZATION_MANAGER
#define SYNCHRONIZATION_MANAGER

//#include <stdbool.h>

#if CELL_PPU // :p
#include <libspe2.h>
#else
typedef int spe_context_ptr_t;
#endif

void setUpSynchronizationManager(unsigned int numberOfWorkers); // currently used only in PC - threads version


void tearDownSynchronizationManager(unsigned int numberOfWorkers);


void setUpSynchronizationManagerWorkerSide();

/*
 * Workers use this method to wait for a new match to start.
 */
void synchronizationManager_workerWaitMatchStart(unsigned long long workerID);


/*
 * Coordinator use this method to indicate each worker that a new match is
 * about to began.
 */
void synchronizationManager_signalMatchStart(unsigned long long workerID,
		spe_context_ptr_t speWorkerContext);


/* bool synchronizationManager_workerMustWork;
 * 
 * Workers use this method to know is they must continue working.
 */
bool synchronizationManager_workerMustWork(unsigned long long workerID);


void synchronizationManager_setWorkerMustWork(unsigned long long workerID,
		spe_context_ptr_t speWorkerContext,
		bool mustWork);


/* void synchronizationManager_waitCoordinatorDataReady();
 * Workers use this method to wait for data to process.
 * 
 * The semantics of this method is like to call a p()
 * method in a semaphore shared between the Coordinator and the Worker 
 * identified by workerID.
 * 
 * The dual of this method is:
 *      synchronizationManager_signalCoordinatorDataReady
 * 
 * 
 */
void synchronizationManager_waitCoordinatorDataReady(unsigned long long workerID);


/* void synchronizationManager_signalCoordinatorDataReady();
 * Coordinator uses this method to tell all workers that there are data 
 * (current picture and reference picture) to process.
 * 
 * The semantics of this method is like to call a v() method in 
 * "n" semaphores shared between Coordinator and Workers. Where "n" is the
 * number of workers.
 * 
 * The "signal" remains even if there is no Worker waiting in the shared semaphore.
 * 
 * The dual of this method is:
 *      synchronizationManager_waitCoordinatorDataReady
 * 
 * 
 */
void synchronizationManager_signalCoordinatorDataReady(unsigned long long workerID, spe_context_ptr_t speWorkerContext);

void synchronizationManager_signalWorkerResultsDone(unsigned long long workerID); /// Used by Workers.
void synchronizationManager_waitWorkerResultsDone(unsigned long long workerID, spe_context_ptr_t speWorkerContext); /// Used by Coordinator.


#endif

