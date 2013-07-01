/*
 * lazy_flat_h264_pmc_internals.h
 *
 *  Created on: Mar 11, 2011
 *      Author: mateus
 */

#ifndef LAZY_FLAT_H264_PMC_INTERNALS_H_
#define LAZY_FLAT_H264_PMC_INTERNALS_H_

typedef struct {
	unsigned long long base_ea_flatH264PMC;
	unsigned int flatH264PMC_Size;
	unsigned int indexFlatPMC;

	MotionVector mvs[160][160];

} LazyFlatH264PMC;



#endif /* LAZY_FLAT_H264_PMC_INTERNALS_H_ */
