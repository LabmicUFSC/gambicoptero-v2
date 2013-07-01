/*
 * reference_bma.h
 *
 *  Created on: Apr 9, 2011
 *      Author: mateus
 */

#ifndef REFERENCE_BMA_H_
#define REFERENCE_BMA_H_

#include "flat_h264_pmc_node.h"
#include "mec_search_window.h"
#include "mec_picture.h"

/* The results are put in the list out_flatPMC */
void referenceBMA_match(FlatH264PMC_Node * out_flatPMC,
		unsigned int * out_flatPMC_Size,
		unsigned int flatPMC_MaxSize,
		MEC_Picture * currentPicture,
		MEC_Picture ** list0,
		unsigned int list0_Size,
        unsigned int totalPictureWidth,
        unsigned int totalPictureHeight,
        MEC_SearchWindow * searchWindow,
		unsigned int workerID,
        int picturePartitionX, // x in Picture
        int picturePartitionY, // y in Picture
        unsigned int picturePartitionWidth,
        unsigned int picturePartitionHeight);


int referenceBMA_informMatchAlgorithm();


#endif /* REFERENCE_BMA_H_ */
