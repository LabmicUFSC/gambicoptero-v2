/*
 * vectorial_pmc.h
 *
 * Vectorial version of Picture Motion Counterpart.
 *
 *  Created on: Jan 19, 2011
 *      Author: mateus
 */

#ifndef VECTORIAL_PMC_H_
#define VECTORIAL_PMC_H_

#include "block_mode_type.h"
#include "reference_picture_motion_counterpart.h"
#include "traits.h"
#include "flat_h264_pmc_node.h"

#define MAX_NUM_BLOCKS_PER_MODE 16

#if ENABLE_BI_PREDICTION
#define NUM_LISTS 2
#else
#define NUM_LISTS 1
#endif

typedef struct {
	/* dimensions:
	 * 1- macroblockID [MAX_NUM_MACROBLOCKS] the maximum number of macroblocks
	 *  is the number of macroblocks that a picture 1920x1088
	 *  ("rounded" full-hd) has.
	 * 2- h264BlockMode [NUM_H264_MODES] number of macroblock modes plus submodes for H.264,
	 *  including the SKIP mode.
	 * 3- [MAX_NUM_BLOCKS_PER_MODE] the maximum number of blocks that a mode can have. (4x4 sub mode).
	 * 4- referencePictureListNumber [2] list0 or list1
	 * 5- referencePictureID [MAX_REFERENCE_PICTURE_LIST_SIZE] maximum of
	 * MAX_REFERENCE_PICTURE_LIST_SIZE reference frames for each list
	 *  (list0, list1).
	 *
	 * When only one list is used, the ReferencePictureMotionCounterpart of the
	 * "unused" list will have its field "available" marked as "false".
	 */
	ReferencePictureMotionCounterpart __rpmc[MAX_NUM_MACROBLOCKS][NUM_H264_MODES][MAX_NUM_BLOCKS_PER_MODE][NUM_LISTS][MAX_REFERENCE_PICTURE_LIST0_SIZE];

	/* The number of macroblocks is the same that MacroblockMotionCounterparts */
	int __numMacroblocks;

	unsigned int __max_reference_pictures;

} PictureMotionCounterpart;



void setUpPictureMotionCounterpart(PictureMotionCounterpart * _this,
		unsigned int pictureWidth,
		unsigned int pictureHeight,
		unsigned int max_reference_pictures);


ReferencePictureMotionCounterpart * pmc_findReferencePictureMotionCounterpart(
		PictureMotionCounterpart * _this,
		int macroblockID,
		BlockModeType h264BlockMode,
		int xInMacroblock,
		int yInMacroblock,
		unsigned int referencePictureListNumber,
		int referencePictureID);


void printPictureMotionCounterpart(PictureMotionCounterpart * _this);


/* This function sets all ReferencePictureMotionCounterpart of PMC with
 * the reference pictures that come from the reference frame lists. */
void pmc_set(PictureMotionCounterpart * _this,
		MEC_Picture ** list0,
		unsigned int list0Size,
		MEC_Picture ** list1,
		unsigned int list1Size);


/* This function returns in xInMacroblock, and yInMacroblock the block
 * coordinates. The block is specified by its ID.
 *
 * Maybe _this function could lie in MEC_Block, instead of here.
 */
void pmc_getBlockCoordinates(int blockID,
		BlockModeType h264BlockMode,
		int * xInMacroblock, int * yInMacroblock);


void pmc_fillPMC_WithFlatPMC(PictureMotionCounterpart * pmc,
		FlatH264PMC_Node * flatPMC,
		unsigned int flatPMC_Size,
		int workerID);


bool pmc_equals(PictureMotionCounterpart * thiz, PictureMotionCounterpart * other);


#endif /* VECTORIAL_PMC_H_ */

