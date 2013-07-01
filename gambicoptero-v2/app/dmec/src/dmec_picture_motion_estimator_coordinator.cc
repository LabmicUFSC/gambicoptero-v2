
/*!
 ******************************************************************************
 * \file
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
#include "../include/master_tracedefs.h"
#include "../include/traits.h"
#include "../include/mec_macroblock.h"
#include "../include/search_metric.h"
#include "../include/block_mode_factory.h"
#include "../include/picture_partitioner.h"
#include "../include/picture_motion_estimator_coordinator.h"
#include "../include/synchronization_manager.h"
#include "../include/match_algorithm.h"
#include "../include/error.h"
#include "../include/abst_assert.h"

#if LINUX
#include <stdlib.h>
#include <assert.h>
#include <iostream> // trace
#include <stdio.h>

using namespace std;

#else
#include <machine.h>
#include <utility/malloc.h>
#include <chronometer.h>
#include <utility/ostream.h>
typedef System::OStream OStream;
extern OStream cout;
#endif

#define COORDINATOR_GENERAL_TRACE (0 && ENABLE_TRACE)
#define COORDINATOR_DETAILED_TRACE 0 // (1 && ENABLE_TRACE)
#define COORDINATOR_GENERAL_DEBUG 0
#define COORDINATOR_DETAILED_DEBUG 0
#define COORDINATOR_PRECONDITIONS 1


// <<singleton>>
PictureMotionEstimator * PictureMotionEstimator::__instance = 0;


// Attributes -----------------------------------------------------------------

// -- Match inputs
DMEC_Picture __currentPicturePartitions[NUM_WORKERS];

DMEC_Picture __list0Partitions[MAX_REFERENCE_PICTURE_LIST0_SIZE][NUM_WORKERS];
//~ unsigned int __list0Size;

DMEC_Picture __list1Partitions[MAX_REFERENCE_PICTURE_LIST1_SIZE][NUM_WORKERS];
//~ unsigned int __list1Size;

// -- Match outputs
// There is one flat PMC (Picture Motion Counterpart) for each worker
FlatH264PMC_Node __flatPMC_Workers[NUM_WORKERS][MAX_FLAT_PMC_SIZE_FOR_COORDINATOR];
unsigned int __flatPMC_Size_Workers[NUM_WORKERS];

// -- Effective Addresses for current and reference pictures.
EAs_DMEC_Picture __info_dmec_currentPicturePartitions[NUM_WORKERS];


/*  The size for info_dmec_listX is the same than for listX.
    Because of that there is no info_dmec_listXSize attribute.
	The same is valid for list 1.
*/
EAs_DMEC_Picture __info_dmec_list0Partitions[NUM_WORKERS][MAX_REFERENCE_PICTURE_LIST0_SIZE];

EAs_DMEC_Picture __info_dmec_list1Partitions[NUM_WORKERS][MAX_REFERENCE_PICTURE_LIST1_SIZE];

// -- Globals for debug
static int __picName__[NUM_WORKERS];

// ----------------------------------------------------------------------------

extern WorkerParameters * __pictureMotionEstimatorWorkerProxy_getWorkerParameters(int workerID);


extern unsigned int picture_motion_estimator_worker_proxy_totalPictureWidth;
extern unsigned int picture_motion_estimator_worker_proxy_totalPictureHeight;

// "Private Methods" implementation
void PictureMotionEstimator::pp_copyPartitioning(MEC_Picture * picture,
		DMEC_Picture * partitions,
		unsigned int numPartitions)
{
	// TODO: Fatorar este para copiar já particionando ao invés de usar o copy_MEC_Picture_to_DMEC_Picture
	DMEC_Picture aux;
	copy_MEC_Picture_to_DMEC_Picture(picture, &aux);

	WorkerParameters * wp;
	DMEC_Picture * pp;
	unsigned int i;

	for (i = 0; i < numPartitions; i++) {
		wp = __pictureMotionEstimatorWorkerProxy_getWorkerParameters(i);
		pp = &(partitions[i]);

		pp->width = wp->picturePartitionWidth;
		pp->height = wp->picturePartitionHeight;

		unsigned int totalWidth = wp->totalPictureWidth;

		unsigned int row, col, srow, scol;

		for (row = 0; row < pp->height; ++ row) {
			srow = row + wp->picturePartitionY;

			for (col = 0; col < pp->width; ++ col) {
				scol = col + wp->picturePartitionX;

				pp->Y_samples[row * pp->width + col] =
						aux.Y_samples[srow * totalWidth + scol];
			}
		}
	}

}


unsigned int PictureMotionEstimator::__computeTotalFlatPMC_Size(unsigned int list0ReferencePictureSize,
		unsigned int list1ReferencePictureSize)
{
	unsigned int numberOfMacroblocksInHorizontal = __pictureWidth / MACROBLOCK_SIZE;
	unsigned int numberOfMacroblocksInVertical = __pictureHeight /  MACROBLOCK_SIZE;
	unsigned int numberOfMacroblocks = numberOfMacroblocksInHorizontal * numberOfMacroblocksInVertical;

	unsigned int numberOfRPMCsInMacroblock =
			NUMBER_OF_BLOCKS_FOR_ALL_MODES * list0ReferencePictureSize +
			NUMBER_OF_BLOCKS_FOR_ALL_MODES * list1ReferencePictureSize;

	unsigned int flatPictureMotionCounterpartSize = numberOfMacroblocks * numberOfRPMCsInMacroblock;

	return flatPictureMotionCounterpartSize;
}


PictureMotionCounterpart * PictureMotionEstimator::__commonMatch(MEC_Picture * currentPicture,
    MEC_Picture ** list0, unsigned int list0Size,
    MEC_Picture ** list1, unsigned int list1Size)
{
/* Pre-Conditions */
#if COORDINATOR_PRECONDITIONS
    Assert(list0Size > 0, "Coordinator::__commonMatch", "list0Size > 0");
#endif

/* -------------- */

    unsigned int i;
    
    pp_copyPartitioning(currentPicture, __currentPicturePartitions, NUM_WORKERS);

    for (i = 0; i < list0Size; i++) { /* Considerando que os pictures vem antes dos workers */
		pp_copyPartitioning(list0[i], __list0Partitions[i], NUM_WORKERS);
	}


    for (i = 0; i < list1Size; i++) {
		pp_copyPartitioning(list1[i], __list1Partitions[i], NUM_WORKERS);
	}

    /* If Worker proxy known list's size during setup, _this become unnecessary. BEGIN */
    for (i = 0; i < NUM_WORKERS; i++) {
    	__flatPMC_Size_Workers[i] = pictureMotionEstimatorWorkerProxy_setWorkerToPerformMatch(
            __workers[i],
            list0Size,
            list1Size,
            0);
    }
    /* If Worker proxy known list's size during setup, _this become unnecessary. END */
    
	pictureMotionEstimatorWorkerProxy_signalMatchStart(); // because the SPE contexts are in ...woker_proxy.c This also indicates worker to continue their work.
    pictureMotionEstimatorWorkerProxy_signalCoordinatorDataReady(); // because the SPE contexts are in ...woker_proxy.c

    pictureMotionEstimatorWorkerProxy_waitWorkerResultsDone(); // because the SPE contexts are in ...woker_proxy.c


    PictureMotionCounterpart * pmc;
    pmc = &(__pmc);

    pmc_set(pmc, list0, list0Size, list1, list1Size);

    for (i = 0; i < NUM_WORKERS; i++) {
    	__fillPMC_WithFlatPMC(pmc, __flatPMC_Workers[i], __flatPMC_Size_Workers[i], i);
    }

    return pmc;
}


void PictureMotionEstimator::__fillPMC_WithFlatPMC(PictureMotionCounterpart * pmc, FlatH264PMC_Node * flatPMC, uint flatPMC_Size, int workerID)
{
	unsigned int im;
	for (im = 0; im < flatPMC_Size; ++ im) {
		ReferencePictureMotionCounterpart * rpmc = (ReferencePictureMotionCounterpart *) 0;
		rpmc = pmc_findReferencePictureMotionCounterpart(pmc,
			flatPMC[im].macroblockID, (BlockModeType) flatPMC[im].h264BlockMode,
			flatPMC[im].xInMacroblock, flatPMC[im].yInMacroblock,
			flatPMC[im].referencePictureListNumber, flatPMC[im].referencePictureID
		);

		//assert(rpmc);
		rpmc->cost = flatPMC[im].cost;
		rpmc->translation.mv_x = flatPMC[im].mv_x;
		rpmc->translation.mv_y = flatPMC[im].mv_y;

		rpmc->pred_mv[0] = flatPMC[im].pred_mv_x;
		rpmc->pred_mv[1] = flatPMC[im].pred_mv_y;

		rpmc->generatedByWorker = workerID;

	}

}


// "Public Methods" implementation
PictureMotionEstimator * PictureMotionEstimator::getInstance(unsigned int pictureWidth,
		unsigned int pictureHeight,
		unsigned int max_reference_pictures)
{
	if (! PictureMotionEstimator::__instance) {
		PictureMotionEstimator::__instance = new PictureMotionEstimator(pictureWidth,
				pictureHeight,
				max_reference_pictures);
	}

	return PictureMotionEstimator::__instance;
}


PictureMotionEstimator::PictureMotionEstimator() {}


PictureMotionEstimator::PictureMotionEstimator(
		unsigned int pictureWidth,
		unsigned int pictureHeight,
		unsigned int max_reference_pictures)
{
    unsigned int i;

    for (i = 0; i < NUM_WORKERS; i++) {
        __picName__[i] = 0;
    }


/* Pre-Conditions */
#if COORDINATOR_PRECONDITIONS
    Assert(pictureWidth >= 48, "Coordinator", "pictureWidth >= 48");
    Assert(pictureHeight >= 32, "Coordinator", "pictureHeight >= 32");
    Assert(pictureWidth % 16 == 0, "Coordinator", "pictureWidth % 16 == 0");
    Assert(pictureHeight % 16 == 0, "Coordinator", "pictureHeight % 16 == 0");

#endif
/* -------------- */


    picture_motion_estimator_worker_proxy_totalPictureWidth = pictureWidth;
    picture_motion_estimator_worker_proxy_totalPictureHeight = pictureHeight;

    setUpSynchronizationManager(NUM_WORKERS); // 6 == number of workers

    __pictureWidth = pictureWidth;
    __pictureHeight = pictureHeight;
    BlockMode ** blockModes = (BlockMode **) blockModeFactory_createH264BlockModes();
    __modesAtLevelZero = blockModes;
    __numberOfModesAtLevelZero = 1; /* Skip mode */
    setUpPictureMotionCounterpart(&(__pmc),
    		pictureWidth,
    		pictureHeight,
    		max_reference_pictures);

    for (i = 0; i < NUM_WORKERS; i++) {
    	setUpEAs_DMEC_Picture(&(__currentPicturePartitions[i]),
    			&(__info_dmec_currentPicturePartitions[i]));
    }


    unsigned long eas_info_dmec_list0Partitions[NUM_WORKERS][MAX_REFERENCE_PICTURE_LIST0_SIZE];
	unsigned long eas_info_dmec_list1Partitions[NUM_WORKERS][MAX_REFERENCE_PICTURE_LIST1_SIZE];

    /* We don't know here the sizes of list0 and list1. But since the maximum
     * size of these lists is fixed, we can set up their eas structures here.
     */
	// trace: printf("\nCoordinator: lists (0/1) picture partitions EAs\n");
    int j;
	for (i = 0; i < MAX_REFERENCE_PICTURE_LIST0_SIZE; ++ i) {
		for (j = 0; j < NUM_WORKERS; j++) {
			setUpEAs_DMEC_Picture(&(__list0Partitions[i][j]), &(__info_dmec_list0Partitions[j][i]));
			eas_info_dmec_list0Partitions[j][i] = (unsigned long) &(__info_dmec_list0Partitions[j][i]);
		}
	}

	for (i = 0; i < MAX_REFERENCE_PICTURE_LIST1_SIZE; ++ i) {
		for (j = 0; j < NUM_WORKERS; j++) {
			setUpEAs_DMEC_Picture(&(__list1Partitions[i][j]), &(__info_dmec_list1Partitions[j][i]));
			eas_info_dmec_list1Partitions[j][i] = (unsigned long) &(__info_dmec_list1Partitions[j][i]);
		}
	}


	int partitionModel = -1;
	switch (NUM_WORKERS) {
		case 1:
			partitionModel = (int) SINGLE_PARTITION;
			break;
		case 2:
			partitionModel = (int) ONExTWO_PARTITION;
			break;
		case 3:
			partitionModel = (int) THREExONE_PARTITION;
			break;
		case 4:
			partitionModel = (int) TWOxTWO_PARTITION;
			break;
		case 5:
			partitionModel = (int) THREE_TWOxTWO_PARTITION;
			break;
		case 6:
			partitionModel = (int) THREExTWO_PARTITION;
			break;
		default:
			Assert(false, "Coordinator", "Unknown partition type");
			break;
	}

	unsigned int numPartitions[1];
	PicturePartitionInfo ** ppInfo = pp_createPartitions(numPartitions,
			(PartitionModel) partitionModel,
			__pictureWidth,
			__pictureHeight);
	cout << "numPartitions: " << numPartitions[0] << "\n";
	cout << "partitionModel: " << partitionModel << "\n";
	Assert(numPartitions[0] == NUM_WORKERS, "Coordinator", "numPartitions == NUM_WORKERS");

	for (i = 0; i < numPartitions[0]; i++) {
		__workers[i] = (PictureMotionEstimatorWorkerProxy) i;
		setUp_PictureMotionEstimatorWorkerProxy(__workers[i],
				ppInfo[i]->picturePartitionX,
				ppInfo[i]->picturePartitionY,
				ppInfo[i]->picturePartitionWidth,
				ppInfo[i]->picturePartitionHeight,
		        computeMacroblockID(ppInfo[i]->picturePartitionX, ppInfo[i]->picturePartitionY, __pictureWidth, __pictureHeight),
				(unsigned long) &(__info_dmec_currentPicturePartitions[i]),
				eas_info_dmec_list0Partitions[i], MAX_REFERENCE_PICTURE_LIST0_SIZE,
				eas_info_dmec_list1Partitions[i], MAX_REFERENCE_PICTURE_LIST1_SIZE,
		        (unsigned long) __flatPMC_Workers[i]);

		delete ppInfo[i];
	}

	delete ppInfo;

    pictureMotionEstimatorWorkerProxy_waitWorkersAlive();
}


PictureMotionEstimator::~PictureMotionEstimator()
{
	unsigned int i;

	for (i = 0; i < NUM_WORKERS; i++) {
		tearDown_PictureMotionEstimatorWorkerProxy(__workers[i]);
	}

	for(i = 0; i < __numberOfModesAtLevelZero; ++ i) {
		//cout << "delete_PictureMotionEstimator() -> delete_BlockMode()\n";
		delete_BlockMode(__modesAtLevelZero[i]);
	}

	//cout << "delete_PictureMotionEstimator() -> free() 1\n";
	free(__modesAtLevelZero);

	//cout << "delete_PictureMotionEstimator() -> tearDownSynchronizationManager()\n";
	tearDownSynchronizationManager(NUM_WORKERS);
}


void PictureMotionEstimator::deleteInstance() {
	if (PictureMotionEstimator::__instance) {
		delete PictureMotionEstimator::__instance;
		PictureMotionEstimator::__instance = 0;
	}
}


PictureMotionCounterpart* PictureMotionEstimator::match(MEC_Picture* currentPicture, MEC_Picture* referencePicture)
{
    MEC_Picture** list0 = (MEC_Picture **) calloc(1, sizeof(MEC_Picture *));
    list0[0] = referencePicture;   

    PictureMotionCounterpart* pmc = __commonMatch(currentPicture,
    		list0, 1,
    		(MEC_Picture **) 0, 0);
    		
    free(list0);		
    		
    return pmc;		
}

