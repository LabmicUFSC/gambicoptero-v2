/*
 * pc_pme_worker_proxy.c
 * Worker Proxy PC, double buffer at Workers, 1 thread per Worker.
 *
 * Author: Mateus Krepsky Ludwich.
 *
*/

#include <thread.h>
#include <machine.h>

#include <utility/malloc.h>

#include "../include/master_tracedefs.h"
#include "../include/traits.h"
#include "../include/dmec_picture.h"
#include "../include/picture_motion_estimator_worker_proxy.h"
#include "../include/picture_motion_estimator_worker.h"
#include "../include/synchronization_manager.h"
#include "../include/error.h"

#define __PC_PICTURE_MOTION_ESTIMATOR_WORKER_PROXY_GENERAL_TRACE (0 && ENABLE_TRACE)
#define __PC_PICTURE_MOTION_ESTIMATOR_WORKER_PROXY_DETAILED_TRACE (0 && ENABLE_TRACE)
#define __PC_PICTURE_MOTION_ESTIMATOR_WORKER_PROXY_GENERAL_DEBUG 1
#define __PC_PICTURE_MOTION_ESTIMATOR_WORKER_PROXY_DETAILED_DEBUG 1
#define __PC_PICTURE_MOTION_ESTIMATOR_WORKER_PROXY_DUMP 1 && ENABLE_DUMP

unsigned int picture_motion_estimator_worker_proxy_totalPictureWidth;
unsigned int picture_motion_estimator_worker_proxy_totalPictureHeight;


/* Private function declarations ------------------------------------------- */

// __setUp_realWorker is the function entry point of worker's thread
int __setUp_realWorker(PictureMotionEstimatorWorker * arg);

#if !LINUX
#include <utility/ostream.h>
typedef System::OStream OStream;
extern OStream cout;
#endif

/* ------------------------------------------------------------------------- */


/* Worker attributes ------------------------------------------------------- */
unsigned int picturePartitionWidthWorkers[NUM_WORKERS];
unsigned int picturePartitionHeightWorkers[NUM_WORKERS];

/* Ter referência aos workers é exclusividade do PC. No cell o
 * equivalente é obtido atraves da carga das imagens spu.elf, que contém
 * os objetos workers.
 */
static PictureMotionEstimatorWorker __workers[NUM_WORKERS];

//static pthread_t __workerThreads[NUM_WORKERS];
typedef System::Thread Thread;
static Thread *__workerThreads[NUM_WORKERS];

/* ------------------------------------------------------------------------- */


// -- Globals for debug
static int __picName__[NUM_WORKERS];

void setUp_PictureMotionEstimatorWorkerProxy(PictureMotionEstimatorWorkerProxy workerProxy,
        int picturePartitionX, int picturePartitionY,
        unsigned int picturePartitionWidth, unsigned int picturePartitionHeight,
        int picturePartitionFirstMacroblockID,
        unsigned long ea_info_dmec_currentPicturePartition,
        unsigned long * eas_info_dmec_list0Partition, unsigned int list0MaxSize,
        unsigned long * eas_info_dmec_list1Partition, unsigned int list1MaxSize,
        unsigned long ea_flatPMC)
{
    __picName__[workerProxy] = 0;

    picturePartitionWidthWorkers[workerProxy] = picturePartitionWidth;
    picturePartitionHeightWorkers[workerProxy] = picturePartitionHeight;

    __workers[workerProxy].__parameters.workerID = workerProxy;
    __workers[workerProxy].__parameters.picturePartitionX = picturePartitionX;
    __workers[workerProxy].__parameters.picturePartitionY = picturePartitionY;
    __workers[workerProxy].__parameters.picturePartitionWidth = picturePartitionWidth;
    __workers[workerProxy].__parameters.picturePartitionHeight = picturePartitionHeight;

    __workers[workerProxy].__parameters.totalPictureWidth = picture_motion_estimator_worker_proxy_totalPictureWidth;
    __workers[workerProxy].__parameters.totalPictureHeight = picture_motion_estimator_worker_proxy_totalPictureHeight;



    __workers[workerProxy].__parameters.ea_info_dmec_currentPicturePartition = ea_info_dmec_currentPicturePartition;

    unsigned int i;
    for (i = 0; i < list0MaxSize; i++) {
        __workers[workerProxy].__parameters.eas_info_dmec_list0Partition[i] = eas_info_dmec_list0Partition[i];
    }

    for (i = 0; i < list1MaxSize; i++) {
        __workers[workerProxy].__parameters.eas_info_dmec_list1Partition[i] = eas_info_dmec_list1Partition[i];
    }

    // On each match: workerParameters[workerProxy].list0ReferencePictureSize = ...
    // On each match: workerParameters[workerProxy].list1ReferencePictureSize = ...

    __workers[workerProxy].__parameters.ea_flatPMC = ea_flatPMC;
    // On each match: workerParameters[workerProxy].flatPictureMotionCounterpartSize = ...


    unsigned long long ea_workerParameters;
    unsigned long aux;
    aux = (unsigned long) &(__workers[workerProxy].__parameters);
    ea_workerParameters = (unsigned long long) aux;
    __workers[workerProxy].__ea_workerParameters = ea_workerParameters;

    /*int retPthreadCreate = pthread_create(&__workerThreads[workerProxy],
            null,
            __setUp_realWorker,
            &__workers[workerProxy]);*/
    __workerThreads[workerProxy] = 0;
    __workerThreads[workerProxy] = new Thread(&__setUp_realWorker, &__workers[workerProxy]);

    if (!__workerThreads[workerProxy]) {
        //perror ("Failed creating thread!");
        //exit(1);
        Abort();
    }
}

void tearDown_PictureMotionEstimatorWorkerProxy(PictureMotionEstimatorWorkerProxy _this)
{
	synchronizationManager_setWorkerMustWork(_this, 0, false);
	synchronizationManager_signalMatchStart(_this, 0);
	
	int status = __workerThreads[_this]->join();
	if(!status) {
	      Abort();
	}
	//cout << "####Coordinator: Thread has done.\n";
	/*if (pthread_join(__workerThreads[_this], NULL)) {
		perror ("Failed joining thread");
		exit (1);
	}*/
}


unsigned int pictureMotionEstimatorWorkerProxy_setWorkerToPerformMatch(PictureMotionEstimatorWorkerProxy _this,
        unsigned int eas_dmec_list0Size,
        unsigned int eas_dmec_list1Size,
        int lambda_factor)
{
    __workers[_this].__parameters.list0Size = eas_dmec_list0Size;
    __workers[_this].__parameters.list1Size = eas_dmec_list1Size;
    __workers[_this].__parameters.lambda_factor = lambda_factor;

    unsigned int numberOfMacroblocksInHorizontal = __workers[_this].__parameters.picturePartitionWidth / MACROBLOCK_SIZE;
    unsigned int numberOfMacroblocksInVertical = __workers[_this].__parameters.picturePartitionHeight /  MACROBLOCK_SIZE;
    unsigned int  numberOfMacroblocks = numberOfMacroblocksInHorizontal * numberOfMacroblocksInVertical;

    /* eas_dmec_list[0/1]Size ==  list[0/1]ReferencePictureSize */
    unsigned int numberOfRPMCsInMacroblock =
            NUMBER_OF_BLOCKS_FOR_ALL_MODES * eas_dmec_list0Size +
            NUMBER_OF_BLOCKS_FOR_ALL_MODES * eas_dmec_list1Size;


    __workers[_this].__parameters.flatPMCSize = numberOfMacroblocks * numberOfRPMCsInMacroblock;

    return __workers[_this].__parameters.flatPMCSize;
}


void pictureMotionEstimatorWorkerProxy_waitWorkersAlive()
{
}


void pictureMotionEstimatorWorkerProxy_signalMatchStart()
{
	int i;

	for (i = 0; i < NUM_WORKERS; i++) {
        synchronizationManager_setWorkerMustWork(__workers[i].__parameters.workerID, 0, true);
		synchronizationManager_signalMatchStart(__workers[i].__parameters.workerID, 0);
	}
}


void pictureMotionEstimatorWorkerProxy_signalCoordinatorDataReady()
{
    int i;

    for (i = 0; i < NUM_WORKERS; i++) {
        synchronizationManager_signalCoordinatorDataReady(__workers[i].__parameters.workerID, i);
    }
}

void pictureMotionEstimatorWorkerProxy_waitWorkerResultsDone()
{
    int i;

    for (i = 0; i < NUM_WORKERS; i++) {
        synchronizationManager_waitWorkerResultsDone(__workers[i].__parameters.workerID, i);
    }
}


/* Private function implementations ---------------------------------------- */
int __setUp_realWorker(PictureMotionEstimatorWorker * arg) {
    PictureMotionEstimatorWorker * worker = (PictureMotionEstimatorWorker *) arg;
    // int ret;

    pictureMotionEstimatorWorker_run_db_sth(worker);
    /*
    if (ret < 0) {
        perror("Failed running context\n");
        return NULL;
    }
    */
    //pthread_exit(NULL);
    return 1;
}


/* ------------------------------------------------------------------------- */



/* "Friends" methods ------------------------------------------------------- */
WorkerParameters * __pictureMotionEstimatorWorkerProxy_getWorkerParameters(int workerID)
{
    return &(__workers[workerID].__parameters);
}

/* ------------------------------------------------------------------------- */
