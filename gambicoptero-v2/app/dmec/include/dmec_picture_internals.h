/*
 * dmec_picture_internals.h
 *
 *  Created on: Jul 29, 2010
 *      Author: mateus
 */

#ifndef DMEC_PICTURE_INTERNALS_H_
#define DMEC_PICTURE_INTERNALS_H_

#include "traits.h"

struct __DMEC_Picture {
	/* All planes (Y,Cb,Cr) must be 16 byte aligned. Besides that, the size of
	 * these planes must be multiple of 16 bytes. */
    MEC_Sample Y_samples[MAX_PICTURE_WIDTH * MAX_PICTURE_HEIGHT] __attribute__((aligned(ALIGNMENT)));

#if ENABLE_CHROMA_PLANES
    MEC_Sample Cb_samples[(MAX_PICTURE_WIDTH * MAX_PICTURE_HEIGHT) / 2] __attribute__((aligned(ALIGNMENT)));
    MEC_Sample Cr_samples[(MAX_PICTURE_WIDTH * MAX_PICTURE_HEIGHT) / 2] __attribute__((aligned(ALIGNMENT)));
#endif

	unsigned int width;
	unsigned int height;

#if ENABLE_CHROMA_PLANES
	unsigned int chroma_width;
	unsigned int chroma_height;
#endif

} __attribute__((aligned(ALIGNMENT)));

typedef struct __DMEC_Picture DMEC_Picture;

struct __EAs_DMEC_Picture {
	unsigned long long ea_Y_samples;

#if ENABLE_CHROMA_PLANES
	unsigned long long ea_Cb_samples;
	unsigned long long ea_Cr_samples;
#endif

} __attribute__((aligned(ALIGNMENT)));

typedef struct __EAs_DMEC_Picture EAs_DMEC_Picture;

#endif /* DMEC_PICTURE_INTERNALS_H_ */
