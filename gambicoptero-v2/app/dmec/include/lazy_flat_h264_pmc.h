/*! \interface LazyFlatH264PMC
 ******************************************************************************
 * \brief
 * 		LazyFlatH264PMC represents the Picture Motion Counterpart from the
 * point of view of one worker.
 *
 ******************************************************************************
 *
 *      Author: mateus
 */
#ifndef LAZY_FLAT_H264_PMC_H_
#define LAZY_FLAT_H264_PMC_H_

#include "motion_vector.h"
#include "block_mode_type.h"
#include "flat_h264_pmc_node.h"
#include "lazy_flat_h264_pmc_internals.h"

/*!
 ******************************************************************************
 * \brief
 *    	Sets up the LazyFlatH264PMC. Should be called before all other methods. (Alexandre: You don't need to use this method.)
 *
 *    You need to use this method only if you are working on picture_motion_estimator_worker.c.
 *
 ******************************************************************************
 */
void setUpLazyFlatH264PMC(LazyFlatH264PMC * flatPMC,
		unsigned long long base_ea_flatH264PMC,
		unsigned flatH264PMC_Size);


/*!
 ******************************************************************************
 * \brief
 *    	Inserts a node of Flat Picture Motion Counterpart in the
 *    "Flat Picture Motion Counterpart Tree". A worker must call this method
 *    always when motion vectors and cost are generated.
 *
 ******************************************************************************
 */
void lfpmc_putFlatH264PMC_Node(LazyFlatH264PMC * flatPMC,
		FlatH264PMC_Node * pmcNode);


/*!
 ******************************************************************************
 * \brief
 *    	Gets a previously generated motion vector (X and Y values) for the
 *    specified position.
 *
 *    A previously generated motion vector must be already inserted in the Flat
 *    Picture Motion Counterpart, in other words, you must be called
 *    lfpmc_putFlatH264PMC_Node before, for that motion vector.
 *
 *    If asked for a motion vector not yet generated, this function will return
 *    the motion vector (SHRT_MAX,SHRT_MAX), where SHRT_MAX is defined in
 *    limits.h.
 *
 * \param mv: The returned motion vector (X and Y values).
 *
 * \param workerID: The ID of the worker responsible for the picture partition.
 *
 * \param xInPicture: The X position of one macroblock in the whole picture.
 *
 * \param yInPicture: The Y position of one macroblock in the whole picture.
 *
 * \param h264BlockMode: The H.264 macroblock mode from where the motion vector
 *  was generated.
 *
 * \param xInMacroblock: The X position of one block in a macroblock. It is
 * relative to the macroblock (i.e. always starts in 0).
 *
 * \param yInMacroblock: The Y position of one block in a macroblock. It is
 * relative to the macroblock (i.e. always starts in 0).
 *
 * \param referencePictureListNumber: The number of the reference picture list
 * from where the motion vector was generated. Valid values are 0 or 1.
 *
 * \param referencePictureID: The ID of the reference picture from where the
 * motion vector was generated.
 *
 ******************************************************************************
 */
void pmc_getMotionVector(LazyFlatH264PMC * flatPMC,
		MotionVector * mv,
		int xInPicture,
		int yInPicture,
		int xInMacroblock,
		int yInMacroblock);

void pmc_putMotionVector(LazyFlatH264PMC * flatPMC,
		short mv_x, short mv_y,
		int xInPicture,
		int yInPicture,
		int xInMacroblock,
		int yInMacroblock);

#endif /* LAZY_FLAT_H264_PMC_H_ */
