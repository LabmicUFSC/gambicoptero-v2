/*
 * Author: Mateus Krepsky Ludwich.
*/

#ifndef PME_WORKER_INTERNALS_H_
#define PME_WORKER_INTERNALS_H_

#include "mec_search_window.h"

#include "lazy_picture.h"
#include "worker_parameters.h"
#include "traits.h"
#include "lazy_flat_h264_pmc.h"

#include "abst_synchronizer.h"
#include "match_algorithm.h"

struct __Worker {
    LazyPicture __list0[MAX_REFERENCE_PICTURE_LIST0_SIZE];
    LazyPicture __list1[MAX_REFERENCE_PICTURE_LIST1_SIZE];
    LazyPicture __currentPicture;
    MEC_SearchWindow searchWindow;
    MatchAlgorithm * __bma;

    WorkerParameters __parameters __attribute__((aligned(ALIGNMENT)));

    unsigned long long __ea_workerParameters;

    LazyFlatH264PMC __flatPMC;

    /* Double buffer / double thread synchronization */
    Semaphore sm_getterPicCanStart;
    Semaphore sm_newMatchStarted;
    Mutex mt_workerMainIsAlive;

    bool __workerMainIsAlive;
};

typedef struct __Worker PictureMotionEstimatorWorker;



#endif /* PME_WORKER_INTERNALS_H_ */
