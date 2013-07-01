/*! \interface LazyPicture
 ******************************************************************************
 * \brief
 * 		LazyPicture represents a proxy for a picture's partition on Worker side.
 *
 ******************************************************************************
 *
 *      Author: mateus
 */

#ifndef LAZY_PICTURE_H_
#define LAZY_PICTURE_H_

#include "motion_vector.h"
#include "block_mode_type.h"
#include "traits.h"

#define USE_GOOD_AND_OLD_LAZY_PICTURE 1

#define __LAZY_PICTURE_GENERATE_FULL_REPORT 0

#if USE_GOOD_AND_OLD_LAZY_PICTURE
#include "lazy_picture_internals.h"
#else
#if LAZY_PICTURE_ENABLE_DOUBLE_BUFFER && ENABLE_CHROMA_PLANES
#include "lazy_picture_internals.h"
#elif !LAZY_PICTURE_ENABLE_DOUBLE_BUFFER  && !ENABLE_CHROMA_PLANES
#include "lazy_picture_internals_sb_sp.h"
#elif LAZY_PICTURE_ENABLE_DOUBLE_BUFFER && !ENABLE_CHROMA_PLANES
#include "lazy_picture_internals_db_sp.h"
#endif
#endif



/*!
 ******************************************************************************
 * \brief
 *    	Valid values for picture's planes.
 *
 ******************************************************************************
 */
enum PlaneID {
	Y = 0,
	Cb,
	Cr
};


/*!
 ******************************************************************************
 * \brief
 *    	Sets up the LazyPicture. Should be called before all other methods. (Alexandre: You don't need to use this method.)
 *
 *    You need to use this method only if you are working on picture_motion_estimator_worker.c.
 *
 ******************************************************************************
 */
void setUpLazyPicture(unsigned long long workerID,
		LazyPicture * picture,
		enum PictureType type,
		int absolute_x_in_picture,
		int absolute_y_in_picture,
		unsigned int picturePartitionWidth,
		unsigned int picturePartitionHeight,
		unsigned int pictureTotalWidth,
		unsigned int pictureTotalHeight,
		unsigned long long ea_info_dmec_picturePartition);


void tearDownLazyPicture(LazyPicture * picture);

#if USE_OBSOLETE
/* xxx OBSOLETE xxx */
void lp_signalCanWriteInBuffers(LazyPicture * picture);
#endif

/*!
 ******************************************************************************
 * \brief
 *    	Gets picture ID.
 *
 ******************************************************************************
 */
#if 0
inline unsigned int lp_getID(LazyPicture * picture);
#endif

/*!
 ******************************************************************************
 * \brief
 *    	Gets one sample of specified row and col.
 *    row and column are relative to picture's partition (i.e. they always starts on (0,0))
 *
 ******************************************************************************
 */
MEC_Sample lp_getSample(LazyPicture * picture, int row, int column);

/*!
 ******************************************************************************
 * \brief
 *    	Gets picture's partition width.
 *
 ******************************************************************************
 */
unsigned int lp_getWidth(LazyPicture * picture);

/*!
 ******************************************************************************
 * \brief
 *    	Gets picture's partition height.
 *
 ******************************************************************************
 */
unsigned int lp_getHeight(LazyPicture * picture);


void lp_generateTimeReport(unsigned long long workerID);


#endif /* LAZY_PICTURE_H_ */
