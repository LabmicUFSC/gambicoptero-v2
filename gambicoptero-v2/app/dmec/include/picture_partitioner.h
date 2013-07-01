/*
 * picture_partitioner.h
 *
 *  Created on: May 26, 2011
 *      Author: mateus
 */

#ifndef PICTURE_PARTITIONER_H_
#define PICTURE_PARTITIONER_H_

#include "vectorial_pmc.h"
#include "flat_h264_pmc_node.h"

#if LINUX
#include <assert.h>
#endif


enum PartitionModel {
	SINGLE_PARTITION = 1,
	ONExTWO_PARTITION = 2,
	THREExONE_PARTITION = 3,
	TWOxTWO_PARTITION = 4,
	THREE_TWOxTWO_PARTITION = 5,
	THREExTWO_PARTITION = 6
};


typedef struct {
	unsigned int workerID;
	int picturePartitionX;
	int picturePartitionY;
	unsigned int picturePartitionWidth;
	unsigned int picturePartitionHeight;

} PicturePartitionInfo;


/* Returns a list of PicturePartitionInfo.
 * The size of this list is returned in out_numPartitionsInfo.
 *
 * You should delete the returned list by your own. */
PicturePartitionInfo ** pp_createPartitions(unsigned int * out_numPartitionsInfo,
		enum PartitionModel model,
		unsigned int totalPictureWidth,
		unsigned int totalPictureHeight);


#endif /* PICTURE_PARTITIONER_H_ */
