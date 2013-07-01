/*
 * lazy_flat_h264_pmc.c
 *
 *  Created on: Jul 16, 2010
 *      Author: mateus
 */
//#include <assert.h>
//#include <stdio.h>
//#include <stdbool.h>
//#include <limits.h>

#include "../include/traits.h"
#include "../include/lazy_flat_h264_pmc.h"

#include "../include/master_tracedefs.h"

#if ENABLE_CHRONOMETER
#include "../include/abst/chronometer.h"
#endif


#if ENABLE_CHRONOMETER
#if ENABLE_CHRONO_RECORDING_RESULTS_TIME
Chronometer chrono_recording_results_time;
#endif

#if ENABLE_CHRONO_GET_MV_TIME
Chronometer chrono_get_mv_time;
#endif

#endif


#define __LAZY_FLAT_H264_PMC_DUMP 1 && ENABLE_DUMP

#define SHRT_MAX 32767

/* Functions Implementation ------------------------------------------------ */
void setUpLazyFlatH264PMC(LazyFlatH264PMC * flatPMC,
		unsigned long long base_ea_flatH264PMC,
		unsigned flatH264PMC_Size)
{
#if ENABLE_CHRONOMETER
#if ENABLE_CHRONO_RECORDING_RESULTS_TIME
	setUpChronometer(&chrono_recording_results_time);
#endif

#if ENABLE_CHRONO_GET_MV_TIME
    setUpChronometer(&chrono_get_mv_time);
#endif
#endif

	flatPMC->base_ea_flatH264PMC = base_ea_flatH264PMC;
	flatPMC->flatH264PMC_Size = flatH264PMC_Size;
	flatPMC->indexFlatPMC = 0;
}

void lfpmc_putFlatH264PMC_Node(LazyFlatH264PMC * flatPMC,
		FlatH264PMC_Node * pmcNode)
{
	/* Pre-conditions */
	//--- assert(__flatPMC.indexFlatPMC < MAX_FLAT_PMC_SIZE_FOR_COORDINATOR);

	/* -------------- */
#if ENABLE_CHRONOMETER
#if ENABLE_CHRONO_RECORDING_RESULTS_TIME
    chrono_start(&chrono_recording_results_time);
#endif
#endif

#if __LAZY_FLAT_H264_PMC_DUMP
	// printf("Worker, pmcNode:");
	// flatH264PMC_Node_print(pmcNode);
	d_dumpflatH264PMC_Node("w_flatpmc.log", pmcNode);
#endif

	unsigned long long ea_flatH264PMC;
	ea_flatH264PMC = flatPMC->base_ea_flatH264PMC +
			flatPMC->indexFlatPMC * sizeof(FlatH264PMC_Node);

	FlatH264PMC_Node * target = (FlatH264PMC_Node *) ((unsigned long) ea_flatH264PMC);
	*target = *pmcNode;

	flatPMC->indexFlatPMC ++;


#if ENABLE_CHRONOMETER
#if ENABLE_CHRONO_RECORDING_RESULTS_TIME
	chrono_pause(&chrono_recording_results_time);
#endif
#endif

}

// maybe move checkFlatPMC to LazyFlatH264PMC
/*
void checkFlatPMC(FlatH264PMC_Node * flatPMC, uint flatPMC_Size,
		uint picturePartitionWidth, uint picturePartitionHeight,
		int useBiPred)
{
#if __RANDOM_MATCH_ALGORITHM_GENERAL_TRACE
	printf("checkFlatPMC (random_match_algorithm)\n");
#endif

	int i;
	for (i = 0; i < flatPMC_Size; ++i) {
		assert(flatPMC[i].mv_x != 0); // not always true, but anyway...
		assert(flatPMC[i].mv_y != 0); // not always true, but anyway...
		assert(flatPMC[i].cost != 0); // not always true, but anyway...
	}
}
*/

static inline bool __matchFlatH264PMC(
		int xInPicture,
		int yInPicture,
		BlockModeType h264BlockMode,
		int xInMacroblock,
		int yInMacroblock,
		unsigned int referencePictureListNumber,
		int referencePictureID,
		FlatH264PMC_Node * flatPMC)
{
	return xInPicture == flatPMC->xInPicture &&
			yInPicture == flatPMC->yInPicture &&
			h264BlockMode == flatPMC->h264BlockMode &&
			xInMacroblock == flatPMC->xInMacroblock &&
			yInMacroblock == flatPMC->yInMacroblock &&
			referencePictureListNumber == flatPMC->referencePictureListNumber &&
			referencePictureID == flatPMC->referencePictureID;
}


void pmc_getMotionVector(LazyFlatH264PMC * flatPMC,
		MotionVector * mv,
		int xInPartition,
		int yInPartition,
		int xInMacroblock,
		int yInMacroblock)
{
#if ENABLE_CHRONOMETER && ENABLE_CHRONO_GET_MV_TIME
    chrono_start(&chrono_get_mv_time);
#endif

    int x, y;
    x = (xInPartition >> 2) + (xInMacroblock >> 2);
    y = (yInPartition >> 2) + (yInMacroblock >> 2);

	mv->mv_x = flatPMC->mvs[x][y].mv_x;
	mv->mv_y = flatPMC->mvs[x][y].mv_y;

#if ENABLE_CHRONOMETER && ENABLE_CHRONO_GET_MV_TIME
    chrono_pause(&chrono_get_mv_time);
#endif
}

void pmc_putMotionVector(LazyFlatH264PMC * flatPMC,
		short mv_x, short mv_y,
		int xInPartition,
		int yInPartition,
		int xInMacroblock,
		int yInMacroblock)
{
#if ENABLE_CHRONOMETER && ENABLE_CHRONO_GET_MV_TIME
    chrono_start(&chrono_get_mv_time);
#endif

    int x, y;
    x = (xInPartition >> 2) + (xInMacroblock >> 2);
    y = (yInPartition >> 2) + (yInMacroblock >> 2);

    flatPMC->mvs[x][y].mv_x = mv_x;
    flatPMC->mvs[x][y].mv_y = mv_y;

#if ENABLE_CHRONOMETER && ENABLE_CHRONO_GET_MV_TIME
    chrono_pause(&chrono_get_mv_time);
#endif
}

/* ------------------------------------------------------------------------- */

