/*
 * vectorial_pmc.c
 *
 *  Created on: Jan 19, 2011
 *      Author: mateus
 */

#include "../include/vectorial_pmc.h"
#include "../include/error.h"

#if LINUX
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#endif

#define VECTORIAL_PMC_PRECONDITIONS 1
#define VECTORIAL_PMC_POSTCONDITIONS 1

#define VECTORIAL_PMC_TRACE 0

#define RELAXED_PMC 0 /* When enabled pmc_findReferencePictureMotionCounterpart
 can return RPMCs which reference picture IDs are not present in the PMC. */



#if RELAXED_PMC
	static ReferencePictureMotionCounterpart nullRpmc;
#endif

/* Block Coordinates related *********************************************** */
typedef
struct {
	int xInMacroblock;
	int yInMacroblock;
} BlockCoordinates;

/*
 * Dimensions
 * 1- h264BlockMode [8] which is the number of macroblock modes plus
 *    submodes for H.264, including the SKIP mode.
 * 2- [16] Because the maximum blockID will be 15 (when dealing with mode 4x4)
 */
static BlockCoordinates __blockCoordinatesMap[8][16];

static inline void __setupBlockCoordinatesMap()
{
	int i, j;
	for (i = 0; i < 8; ++i) {
		for (j = 0; j < 16; ++j) {
			__blockCoordinatesMap[i][j].xInMacroblock = -1;
			__blockCoordinatesMap[i][j].yInMacroblock = -1;
		}
	}

	// --------------------------------------------------------------
	/* MODE_SKIP */
	__blockCoordinatesMap[MODE_SKIP][0].xInMacroblock = 0;
	__blockCoordinatesMap[MODE_SKIP][0].yInMacroblock = 0;

	// --------------------------------------------------------------
	/* MODE16x16 */
	__blockCoordinatesMap[MODE16x16][0].xInMacroblock = 0;
	__blockCoordinatesMap[MODE16x16][0].yInMacroblock = 0;

	// --------------------------------------------------------------
	/* MODE16x8 */
	__blockCoordinatesMap[MODE16x8][0].xInMacroblock = 0;
	__blockCoordinatesMap[MODE16x8][0].yInMacroblock = 0;

	__blockCoordinatesMap[MODE16x8][1].xInMacroblock = 0;
	__blockCoordinatesMap[MODE16x8][1].yInMacroblock = 8;

	// --------------------------------------------------------------
	/* MODE8x16 */
	__blockCoordinatesMap[MODE8x16][0].xInMacroblock = 0;
	__blockCoordinatesMap[MODE8x16][0].yInMacroblock = 0;

	__blockCoordinatesMap[MODE8x16][1].xInMacroblock = 8;
	__blockCoordinatesMap[MODE8x16][1].yInMacroblock = 0;

	// --------------------------------------------------------------
	/* MODE8x8 */
	__blockCoordinatesMap[MODE8x8][0].xInMacroblock = 0;
	__blockCoordinatesMap[MODE8x8][0].yInMacroblock = 0;

	__blockCoordinatesMap[MODE8x8][1].xInMacroblock = 8;
	__blockCoordinatesMap[MODE8x8][1].yInMacroblock = 0;

	__blockCoordinatesMap[MODE8x8][2].xInMacroblock = 0;
	__blockCoordinatesMap[MODE8x8][2].yInMacroblock = 8;

	__blockCoordinatesMap[MODE8x8][3].xInMacroblock = 8;
	__blockCoordinatesMap[MODE8x8][3].yInMacroblock = 8;


	// --------------------------------------------------------------
	/* MODE8x4 */
	__blockCoordinatesMap[MODE8x4][0].xInMacroblock = 0;
	__blockCoordinatesMap[MODE8x4][0].yInMacroblock = 0;

	__blockCoordinatesMap[MODE8x4][1].xInMacroblock = 8;
	__blockCoordinatesMap[MODE8x4][1].yInMacroblock = 0;

	__blockCoordinatesMap[MODE8x4][2].xInMacroblock = 0;
	__blockCoordinatesMap[MODE8x4][2].yInMacroblock = 4;

	__blockCoordinatesMap[MODE8x4][3].xInMacroblock = 8;
	__blockCoordinatesMap[MODE8x4][3].yInMacroblock = 4;

	__blockCoordinatesMap[MODE8x4][4].xInMacroblock = 0;
	__blockCoordinatesMap[MODE8x4][4].yInMacroblock = 8;

	__blockCoordinatesMap[MODE8x4][5].xInMacroblock = 8;
	__blockCoordinatesMap[MODE8x4][5].yInMacroblock = 8;

	__blockCoordinatesMap[MODE8x4][6].xInMacroblock = 0;
	__blockCoordinatesMap[MODE8x4][6].yInMacroblock = 12;

	__blockCoordinatesMap[MODE8x4][7].xInMacroblock = 8;
	__blockCoordinatesMap[MODE8x4][7].yInMacroblock = 12;


	// --------------------------------------------------------------
	/* MODE4x8 */
	__blockCoordinatesMap[MODE4x8][0].xInMacroblock = 0;
	__blockCoordinatesMap[MODE4x8][0].yInMacroblock = 0;

	__blockCoordinatesMap[MODE4x8][1].xInMacroblock = 4;
	__blockCoordinatesMap[MODE4x8][1].yInMacroblock = 0;

	__blockCoordinatesMap[MODE4x8][2].xInMacroblock = 8;
	__blockCoordinatesMap[MODE4x8][2].yInMacroblock = 0;

	__blockCoordinatesMap[MODE4x8][3].xInMacroblock = 12;
	__blockCoordinatesMap[MODE4x8][3].yInMacroblock = 0;

	__blockCoordinatesMap[MODE4x8][4].xInMacroblock = 0;
	__blockCoordinatesMap[MODE4x8][4].yInMacroblock = 8;

	__blockCoordinatesMap[MODE4x8][5].xInMacroblock = 4;
	__blockCoordinatesMap[MODE4x8][5].yInMacroblock = 8;

	__blockCoordinatesMap[MODE4x8][6].xInMacroblock = 8;
	__blockCoordinatesMap[MODE4x8][6].yInMacroblock = 8;

	__blockCoordinatesMap[MODE4x8][7].xInMacroblock = 12;
	__blockCoordinatesMap[MODE4x8][7].yInMacroblock = 8;


	// --------------------------------------------------------------
	/* MODE4x4 */
	__blockCoordinatesMap[MODE4x4][0].xInMacroblock = 0;
	__blockCoordinatesMap[MODE4x4][0].yInMacroblock = 0;

	__blockCoordinatesMap[MODE4x4][1].xInMacroblock = 4;
	__blockCoordinatesMap[MODE4x4][1].yInMacroblock = 0;

	__blockCoordinatesMap[MODE4x4][2].xInMacroblock = 8;
	__blockCoordinatesMap[MODE4x4][2].yInMacroblock = 0;

	__blockCoordinatesMap[MODE4x4][3].xInMacroblock = 12;
	__blockCoordinatesMap[MODE4x4][3].yInMacroblock = 0;

	__blockCoordinatesMap[MODE4x4][4].xInMacroblock = 0;
	__blockCoordinatesMap[MODE4x4][4].yInMacroblock = 4;

	__blockCoordinatesMap[MODE4x4][5].xInMacroblock = 4;
	__blockCoordinatesMap[MODE4x4][5].yInMacroblock = 4;

	__blockCoordinatesMap[MODE4x4][6].xInMacroblock = 8;
	__blockCoordinatesMap[MODE4x4][6].yInMacroblock = 4;

	__blockCoordinatesMap[MODE4x4][7].xInMacroblock = 12;
	__blockCoordinatesMap[MODE4x4][7].yInMacroblock = 4;

	__blockCoordinatesMap[MODE4x4][8].xInMacroblock = 0;
	__blockCoordinatesMap[MODE4x4][8].yInMacroblock = 8;

	__blockCoordinatesMap[MODE4x4][9].xInMacroblock = 4;
	__blockCoordinatesMap[MODE4x4][9].yInMacroblock = 8;

	__blockCoordinatesMap[MODE4x4][10].xInMacroblock = 8;
	__blockCoordinatesMap[MODE4x4][10].yInMacroblock = 8;

	__blockCoordinatesMap[MODE4x4][11].xInMacroblock = 12;
	__blockCoordinatesMap[MODE4x4][11].yInMacroblock = 8;

    __blockCoordinatesMap[MODE4x4][12].xInMacroblock = 0;
    __blockCoordinatesMap[MODE4x4][12].yInMacroblock = 12;

    __blockCoordinatesMap[MODE4x4][13].xInMacroblock = 4;
    __blockCoordinatesMap[MODE4x4][13].yInMacroblock = 12;

	__blockCoordinatesMap[MODE4x4][14].xInMacroblock = 8;
    __blockCoordinatesMap[MODE4x4][14].yInMacroblock = 12;

	__blockCoordinatesMap[MODE4x4][15].xInMacroblock = 12;
	__blockCoordinatesMap[MODE4x4][15].yInMacroblock = 12;

	// --------------------------------------------------------------
}

/* ************************************************************************* */


/* Block IDs related ******************************************************* */
/*
 * Dimensions
 * 1- h264BlockMode [8] which is the number of macroblock modes plus
 *    submodes for H.264, including the SKIP mode.
 * 2- [13] Because the maximum index for xInMabroblock will be 12
 *    (when dealing with mode 4x4)
 * 3- [13] Because the maximum index for yInMabroblock will be 12
 *    (when dealing with mode 4x4)
 */
static int __blockIDMap[8][13][13];


#define UNREGISTERED_BLOCK_ID -1


static inline void __setupBlockIDMap()
{
	int i, j, k;
	for (i = 0; i < 8; ++i) {
		for (j = 0; j < 13; ++j) {
			for (k = 0; k < 13; ++k) {
				__blockIDMap[i][j][k] = UNREGISTERED_BLOCK_ID;
			}
		}

	}

	__blockIDMap[MODE_SKIP][0][0] = 0;

	__blockIDMap[MODE16x16][0][0] = 0;

	__blockIDMap[MODE16x8][0][0] = 0;
	__blockIDMap[MODE16x8][0][8] = 1;

	__blockIDMap[MODE8x16][0][0] = 0;
	__blockIDMap[MODE8x16][8][0] = 1;

	__blockIDMap[MODE8x8][0][0] = 0;
	__blockIDMap[MODE8x8][8][0] = 1;
	__blockIDMap[MODE8x8][0][8] = 2;
	__blockIDMap[MODE8x8][8][8] = 3;

	__blockIDMap[MODE8x4][0][0] = 0;
	__blockIDMap[MODE8x4][8][0] = 1;
	__blockIDMap[MODE8x4][0][4] = 2;
	__blockIDMap[MODE8x4][8][4] = 3;
	__blockIDMap[MODE8x4][0][8] = 4;
	__blockIDMap[MODE8x4][8][8] = 5;
	__blockIDMap[MODE8x4][0][12] = 6;
	__blockIDMap[MODE8x4][8][12] = 7;

	__blockIDMap[MODE4x8][0] [0] = 0;
	__blockIDMap[MODE4x8][4] [0] = 1;
	__blockIDMap[MODE4x8][8] [0] = 2;
	__blockIDMap[MODE4x8][12][0] = 3;
	__blockIDMap[MODE4x8][0] [8] = 4;
	__blockIDMap[MODE4x8][4] [8] = 5;
	__blockIDMap[MODE4x8][8] [8] = 6;
	__blockIDMap[MODE4x8][12][8] = 7;

	__blockIDMap[MODE4x4][0] [0] = 0;
	__blockIDMap[MODE4x4][4] [0] = 1;
	__blockIDMap[MODE4x4][8] [0] = 2;
	__blockIDMap[MODE4x4][12][0] = 3;
	__blockIDMap[MODE4x4][0] [4] = 4;
	__blockIDMap[MODE4x4][4] [4] = 5;
	__blockIDMap[MODE4x4][8] [4] = 6;
	__blockIDMap[MODE4x4][12][4] = 7;
	__blockIDMap[MODE4x4][0] [8] = 8;
	__blockIDMap[MODE4x4][4] [8] = 9;
	__blockIDMap[MODE4x4][8] [8] = 10;
	__blockIDMap[MODE4x4][12][8] = 11;
	__blockIDMap[MODE4x4][0] [12] = 12;
	__blockIDMap[MODE4x4][4] [12] = 13;
	__blockIDMap[MODE4x4][8] [12] = 14;
	__blockIDMap[MODE4x4][12][12] = 15;
}

/* ************************************************************************* */


/* Vectorial PMC Methods *************************************************** */
static inline void __clearRpmc(PictureMotionCounterpart * _this)
{
	ReferencePictureMotionCounterpart * rpmc;

	int macroblockID, h264BlockMode, blockNumber, listNumber;
	unsigned int referencePictures;
	for (macroblockID = 0; macroblockID < _this->__numMacroblocks; ++macroblockID) {
		for (h264BlockMode = 0; h264BlockMode < NUM_H264_MODES; ++h264BlockMode) {
			for (blockNumber = 0; blockNumber < MAX_NUM_BLOCKS_PER_MODE; ++blockNumber) {
				for (listNumber = 0; listNumber < 1; ++listNumber) {
					for (referencePictures = 0; referencePictures < _this->__max_reference_pictures; ++referencePictures) {
						rpmc = &(_this->__rpmc[macroblockID][h264BlockMode][blockNumber][listNumber][referencePictures]);
						rpmc->avalible = false;
						rpmc->cost = (unsigned int) -1;
						rpmc->generatedByWorker = -1;
						rpmc->referencePicture = 0;
					}
				}
			}
		}
	}

}

void setUpPictureMotionCounterpart(PictureMotionCounterpart * _this,
		unsigned int pictureWidth,
		unsigned int pictureHeight,
		unsigned int max_reference_pictures)
{
	_this->__numMacroblocks = (pictureHeight / MACROBLOCK_SIZE) * (pictureWidth / MACROBLOCK_SIZE);
	_this->__max_reference_pictures = max_reference_pictures;

	__setupBlockIDMap();
	__setupBlockCoordinatesMap();
	__clearRpmc(_this);

#if RELAXED_PMC
	nullRpmc.avalible = true;
	nullRpmc.cost = 0;
	nullRpmc.generatedByWorker = -1;
	nullRpmc.pred_mv[0] = 0;
	nullRpmc.pred_mv[1] = 0;
	nullRpmc.rate = 0;
	nullRpmc.referencePicture = 0;
	nullRpmc.translation.mv_x = 0;
	nullRpmc.translation.mv_y = 0;
#endif

}

static inline int __computeBlockID(BlockModeType h264BlockMode,
		int xInMacroblock,
		int yInMacroblock)
{
	int id = __blockIDMap[h264BlockMode][xInMacroblock][yInMacroblock];


#if	VECTORIAL_PMC_TRACE
	if(id == UNREGISTERED_BLOCK_ID) {
		printf("vectorial_pmc.c::__computeBlockID: (h264BlockMode: %i, xInMacroblock: %i, yInMacroblock: %i)\n", h264BlockMode, xInMacroblock, yInMacroblock);
	}

#endif


#if	VECTORIAL_PMC_POSTCONDITIONS
	//assert(id != UNREGISTERED_BLOCK_ID);
#endif

	return id;
}


void pmc_getBlockCoordinates(int blockID,
		BlockModeType h264BlockMode,
		int * xInMacroblock, int * yInMacroblock)
{
	xInMacroblock[0] = __blockCoordinatesMap[h264BlockMode][blockID].xInMacroblock;
	yInMacroblock[0] = __blockCoordinatesMap[h264BlockMode][blockID].yInMacroblock;
#if VECTORIAL_PMC_TRACE
	printf("pmc_getBlockCoordinates, blockID: %i, h264BlockMode: %i, xInMacroblock: %i, yInMacroblock: %i\n",
			blockID, h264BlockMode, xInMacroblock[0], yInMacroblock[0]);
#endif
}


ReferencePictureMotionCounterpart * pmc_findReferencePictureMotionCounterpart(
		PictureMotionCounterpart * _this,
		int macroblockID,
		BlockModeType h264BlockMode,
		int xInMacroblock,
		int yInMacroblock,
		unsigned int referencePictureListNumber,
		int referencePictureID)
{
#if VECTORIAL_PMC_TRACE
	printf("vectorial_pmc.c::pmc_findReferencePictureMotionCounterpart\n");
#endif

	int blockNumber = __computeBlockID(h264BlockMode, xInMacroblock, yInMacroblock);

	ReferencePictureMotionCounterpart * all_rpmcs = _this->__rpmc[macroblockID][h264BlockMode][blockNumber][referencePictureListNumber];
	ReferencePictureMotionCounterpart * rpmc = 0;

	rpmc = &(all_rpmcs[referencePictureID]); /* "sequential" IDs */

#if RELAXED_PMC
	if (! rpmc) {
		rpmc = &nullRpmc;
	}
#else
	if (! rpmc) { /* The polemic question xxx */
		/*printf("Error at pmc_findReferencePictureMotionCounterparts: referencePictureID unknown by PMC!\n");
		printf("You have asked for: (macroblockID: %i, h264BlockMode: %i, xInMacroblock: %i, yInMacroblock: %i, referencePictureListNumber: %i, referencePictureID: %i)\n",
				macroblockID, h264BlockMode, xInMacroblock, yInMacroblock, referencePictureListNumber, referencePictureID);
		abort();*/
		Abort();
	}
#endif

#if VECTORIAL_PMC_POSTCONDITIONS
	//assert(rpmc->avalible);
#endif

	return rpmc;
}


void printPictureMotionCounterpart(PictureMotionCounterpart * _this)
{
	// TODO
}


void pmc_set(PictureMotionCounterpart * _this,
		MEC_Picture ** list0,
		unsigned int list0Size,
		MEC_Picture ** list1,
		unsigned int list1Size)
{
#if VECTORIAL_PMC_TRACE
	printf("vectorial_pmc.c::pmc_set\n");
#endif

	__clearRpmc(_this);

	ReferencePictureMotionCounterpart * rpmc;

	int macroblockID, h264BlockMode, blockID;
	unsigned int referencePictures;
	for (macroblockID = 0; macroblockID < _this->__numMacroblocks; ++macroblockID) {
		for (h264BlockMode = 0; h264BlockMode < NUM_H264_MODES; ++h264BlockMode) {
			for (blockID = 0; blockID < bmt_getNumBlocks((BlockModeType) h264BlockMode); ++blockID) {

				for (referencePictures = 0; referencePictures < list0Size; ++referencePictures) {
					rpmc = &(_this->__rpmc[macroblockID][h264BlockMode][blockID][0][referencePictures]);
					rpmc->referencePicture = list0[referencePictures];
					rpmc->avalible = true;
				}
				for (referencePictures = 0; referencePictures < list1Size; ++referencePictures) {
					rpmc = &(_this->__rpmc[macroblockID][h264BlockMode][blockID][1][referencePictures]);
					rpmc->referencePicture = list1[referencePictures];
					rpmc->avalible = true;
				}

			}
		}
	}

}


void pmc_fillPMC_WithFlatPMC(PictureMotionCounterpart * pmc,
		FlatH264PMC_Node * flatPMC,
		unsigned int flatPMC_Size,
		int workerID)
{
	unsigned int im;
	for (im = 0; im < flatPMC_Size; ++ im) {
		ReferencePictureMotionCounterpart * rpmc = (ReferencePictureMotionCounterpart *) 0;
		rpmc = pmc_findReferencePictureMotionCounterpart(pmc,
			flatPMC[im].macroblockID, (BlockModeType) flatPMC[im].h264BlockMode,
			flatPMC[im].xInMacroblock, flatPMC[im].yInMacroblock,
			flatPMC[im].referencePictureListNumber, flatPMC[im].referencePictureID);

		//assert(rpmc);
		rpmc->cost = flatPMC[im].cost;
		rpmc->translation.mv_x = flatPMC[im].mv_x;
		rpmc->translation.mv_y = flatPMC[im].mv_y;

		rpmc->pred_mv[0] = flatPMC[im].pred_mv_x;
		rpmc->pred_mv[1] = flatPMC[im].pred_mv_y;

		rpmc->generatedByWorker = workerID;
	}

}


bool pmc_equals(PictureMotionCounterpart * thiz,
		PictureMotionCounterpart * other)
{
	if (thiz->__max_reference_pictures != other->__max_reference_pictures) {
		return false;
	}

	if (thiz->__numMacroblocks != other->__numMacroblocks) {
		return false;
	}

	ReferencePictureMotionCounterpart * rpmcFromThiz;
	ReferencePictureMotionCounterpart * rpmcFromOther;

	int macroblockID, h264BlockMode, blockNumber, listNumber, referencePictures;
	for (macroblockID = 0; macroblockID < thiz->__numMacroblocks; ++macroblockID) {
		for (h264BlockMode = 0; h264BlockMode < NUM_H264_MODES; ++h264BlockMode) {
			for (blockNumber = 0; blockNumber < MAX_NUM_BLOCKS_PER_MODE; ++blockNumber) {
				for (listNumber = 0; listNumber < 1; ++listNumber) {
					for (referencePictures = 0; referencePictures < (int) thiz->__max_reference_pictures; ++referencePictures) {
						rpmcFromThiz = &(thiz->__rpmc[macroblockID][h264BlockMode][blockNumber][listNumber][referencePictures]);
						rpmcFromOther = &(other->__rpmc[macroblockID][h264BlockMode][blockNumber][listNumber][referencePictures]);
						if (! rpmc_equals(rpmcFromThiz, rpmcFromOther)) {
							return false;
						}
					}
				}
			}
		}
	}

	return true;
}
