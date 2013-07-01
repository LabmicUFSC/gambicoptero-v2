
/*!
 ******************************************************************************
 * \file
 * 	picture_motion_estimator_worker.c
 *
 * \brief
 *    -
 *
 * \author
 *      - Alexandre Massayuki Okazaki   (alexandre@lisha.ufsc.br)
 *      - Mateus Krepsky Ludwich        (mateus@lisha.ufsc.br)
 *      - Tiago de Albuquerque Reis     (reis@lisha.ufsc.br)
 *
 ******************************************************************************
 */

#include "../include/picture_motion_estimator_worker.h"
#include "../include/flat_h264_pmc_node.h"
#include "../include/synchronization_manager.h"
#include "../include/lazy_flat_h264_pmc.h"
#include "../include/lazy_picture.h"
#include "../include/master_tracedefs.h"
#include "../include/mec_picture.h"
#include "../include/block_mode_type.h"
#include "../include/traits.h"

#include "../include/full_search_match_algorithm.h"


#define __WORKER_GENERAL_TRACE (1 && ENABLE_TRACE)
#define __WORKER_DETAILED_TRACE (1 && ENABLE_TRACE)
#define __WORKER_GENERAL_DEBUG 1
#define __WORKER_DETAILED_DEBUG 1

#if LINUX
#include <stdlib.h>
#include <assert.h>
#include <iostream>

#include "../../../../linux/include/adapter_full_search_match_algorithm.h"

using namespace std;

#else
#include <utility/malloc.h>
#include <utility/ostream.h>
typedef System::OStream OStream;
extern OStream cout;
#endif


/* ------------------------------------------------------------------------- */


/* Private functions declaration ------------------------------------------- */
inline void __setUpWorker(PictureMotionEstimatorWorker * worker);

inline void __tearDownWorker(PictureMotionEstimatorWorker * worker);

static inline void __setUpLazyPictures__(PictureMotionEstimatorWorker * worker);

static inline void __tearDownLazyPictures__(PictureMotionEstimatorWorker * worker);

/* ------------------------------------------------------------------------- */


void pictureMotionEstimatorWorker_run_db_sth(PictureMotionEstimatorWorker * worker)
{
	__setUpWorker(worker);

	synchronizationManager_workerWaitMatchStart(worker->__parameters.workerID);

	while (synchronizationManager_workerMustWork(worker->__parameters.workerID)) {
	    synchronizationManager_waitCoordinatorDataReady(worker->__parameters.workerID);

	    //printf("Worker: %lli read to match\n", worker->__parameters.workerID);

	    /* Must call this here again to get some parameters that are known only on
	     * match time such as, lists reference sizes.  */
	    WorkerParameters * p = (WorkerParameters *) ((unsigned long) worker->__ea_workerParameters);
	    worker->__parameters = *p;

	    // Starting lazy pictures and lazy FlatPMC
	    __setUpLazyPictures__(worker);

	    setUpLazyFlatH264PMC(&worker->__flatPMC,
	            worker->__parameters.ea_flatPMC,
	            worker->__parameters.flatPMCSize);

	    //~~~ __lazyPictures_gatherFirstSamples__(worker); /* Comentado, pois __db_commomGetSamples está fazendo um update a cada "novo acesso" a uma picture */

	    // Match
        // cout << "%%%%%%%pictureMotionEstimatorWorker_run_db_sth() -> match_algorithm_biPredictionMatch()\n";
        worker->__bma->match_algorithm_biPredictionMatch(worker->__parameters.workerID,
				&(worker->__currentPicture),
				worker->__list0, worker->__parameters.list0Size,
				worker->__list1, worker->__parameters.list1Size,
				worker->__parameters.picturePartitionX, worker->__parameters.picturePartitionY,
				worker->__parameters.picturePartitionWidth, worker->__parameters.picturePartitionHeight,
				worker->__parameters.totalPictureWidth, worker->__parameters.totalPictureHeight,
				&(worker->searchWindow),
				worker->__parameters.lambda_factor,
				&worker->__flatPMC);

		synchronizationManager_signalWorkerResultsDone(worker->__parameters.workerID);

		synchronizationManager_workerWaitMatchStart(worker->__parameters.workerID);
	}

	__tearDownLazyPictures__(worker);


	__tearDownWorker(worker);

}


void __setUpWorker(PictureMotionEstimatorWorker * worker)
{
    // cout << "__setUpWorker() 6\n";
    setUpSynchronizationManagerWorkerSide();

    // cout << "__setUpWorker() 7\n";
    worker->searchWindow.searchRange = SEARCH_RANGE;
    // cout << "__setUpWorker() 8\n";
    
    // cout << "__setUpWorker() 9\n";
    WorkerParameters * p = (WorkerParameters *) ((unsigned long) worker->__ea_workerParameters);
    worker->__parameters = *p;

    worker->__bma = new ActualMatchAlgorithm();
}


void __tearDownWorker(PictureMotionEstimatorWorker * worker)
{
}


/* Implementation of functions to run when Double Buffer support for
 * LazyPicture is enabled
 */
void __setUpLazyPictures__(PictureMotionEstimatorWorker * worker)
{
    setUpLazyPicture(worker->__parameters.workerID,
            &(worker->__currentPicture),
            CurrentPicture,
            worker->__parameters.picturePartitionX,
            worker->__parameters.picturePartitionY,
            worker->__parameters.picturePartitionWidth,
            worker->__parameters.picturePartitionHeight,
            worker->__parameters.totalPictureWidth,
            worker->__parameters.totalPictureHeight,
            worker->__parameters.ea_info_dmec_currentPicturePartition);

    unsigned int i;

    for (i = 0; i < worker->__parameters.list0Size; ++ i) {
        setUpLazyPicture(worker->__parameters.workerID,
                &(worker->__list0[i]),
                ReferencePicture,
                worker->__parameters.picturePartitionX,
                worker->__parameters.picturePartitionY,
                worker->__parameters.picturePartitionWidth,
                worker->__parameters.picturePartitionHeight,
                worker->__parameters.totalPictureWidth,
                worker->__parameters.totalPictureHeight,
                worker->__parameters.eas_info_dmec_list0Partition[i]);
    }

    for (i = 0; i < worker->__parameters.list1Size; ++ i) {
        setUpLazyPicture(worker->__parameters.workerID,
                &(worker->__list1[i]),
				ReferencePicture,
                worker->__parameters.picturePartitionX,
                worker->__parameters.picturePartitionY,
                worker->__parameters.picturePartitionWidth,
                worker->__parameters.picturePartitionHeight,
                worker->__parameters.totalPictureWidth,
                worker->__parameters.totalPictureHeight,
                worker->__parameters.eas_info_dmec_list1Partition[i]);
    }

}


void __tearDownLazyPictures__(PictureMotionEstimatorWorker * worker)
{
	tearDownLazyPicture(&worker->__currentPicture);
	unsigned int i;

	for (i = 0; i < worker->__parameters.list0Size; ++ i) {
		tearDownLazyPicture(&worker->__list0[i]);
	}

	for (i = 0; i < worker->__parameters.list1Size; ++ i) {
		tearDownLazyPicture(&worker->__list1[i]);
	}


	lp_generateTimeReport(worker->__parameters.workerID);

}



/* ------------------------------------------------------------------------- */




