/*! \interface MatchAlgorithm
 ******************************************************************************
 * \brief
 * 		MatchAlgorithm is the point where workers perform motion estimation of
 * theirs picture's partitions.
 *
 ******************************************************************************
 *
 *      Author: mateus
 */

#include "mec_search_window.h"

#include "flat_h264_pmc_node.h"
#include "lazy_picture.h"
#include "lazy_flat_h264_pmc.h"
#include "algorithm_ids.h"

#ifndef MATCH_ALGORITHM_H_
#define MATCH_ALGORITHM_H_

/*!
 ******************************************************************************
 * \brief
 *    	Performs motion estimation in a picture partition.
 *
 *    This method must call lfpmc_putFlatH264PMC_Node method, for each computed
 *    motion vectors and cost. See xor_match_algorithm.c for an example.
 *
 * \param workerID: The ID of the worker responsible for the picture partition.
 *
 * \param currentPicture: The picture partition which will be searched in
 * the pictures of reference (i.e. the partition that will be motion-estimated).
 *
 * \param list0: The list of pictures of reference number 0.
 *
 * \param list0_Size: The number of reference pictures in list0. This value
 * cannot be zero (i.e. list0 cannot be empty).
 *
 * \param list1: The list of pictures of reference number 1.
 *
 * \param list1_Size: The number of reference pictures in list1. This value
 * can be zero (i.e. list1 can be empty).
 *
 * \param picturePartitionX: The position X in the whole Picture, where the
 * picture partition starts (i.e. its left corner).
 *
 * \param picturePartitionY: The position Y in the whole Picture, where the
 * picture partition starts (i.e. its top corner).
 *
 * \param picturePartitionWidth: The with in pixels of the picture partition.
 *
 * \param picturePartitionHeight: The height in pixels of the picture partition.
 *
 * \param totalPictureWidth: The width in pixels of the whole picture.
 *
 * \param totalPictureHeight: The height in pixels of the whole picture.
 *
 * \param searchWindow: The search window to be used in the search for the motion vectors.
 *
 * \param lambda_factor: Used to compute the motion cost.
 *
 ******************************************************************************
 */
class MatchAlgorithm
{

public:
    virtual void match_algorithm_biPredictionMatch(unsigned int workerID,
            LazyPicture * currentPicture,
            LazyPicture list0[], unsigned int list0_Size,
            LazyPicture list1[], unsigned int list1_Size,
            int picturePartitionX, // x in Picture
            int picturePartitionY, // y in Picture
            unsigned int picturePartitionWidth,
            unsigned int picturePartitionHeight,
            unsigned int totalPictureWidth,
            unsigned int totalPictureHeight,
            MEC_SearchWindow * searchWindow,
            int lambda_factor,
            LazyFlatH264PMC * flatPMC) = 0;


    virtual ~MatchAlgorithm()
    {}


};

#endif /* MATCH_ALGORITHM_H_ */

