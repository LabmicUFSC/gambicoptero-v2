/*
 * lazy_picture_internals.h
 *
 *  Created on: Jul 27, 2010
 *      Author: mateus
 */

#ifndef LAZY_PICTURE_INTERNALS_H_
#define LAZY_PICTURE_INTERNALS_H_

#include "mec_sample.h"

#include "abst_synchronizer.h"

#include "traits.h"

#if LINUX
#include <string>

using namespace std;
#endif

#if ENABLE_CHROMA_PLANES
#define NUM_PLANES 3 /* Planes: Y, Cb, Cr */
#else
#define NUM_PLANES 1 /* Plane: Y */
#endif

#define NUM_BUFFERS 1


enum PictureType {
	CurrentPicture,
	ReferencePicture
};


/* Do not use this struct directly!
 * Always use it through its access methods!
 * */
struct LazyPicture
{
	enum PictureType __type;

	/* About Picture partition */
	unsigned int width;
	unsigned int height;
	unsigned int numLumaSamples;


	/* unsigned long long ea_ID; */
	unsigned long long newStuff_ea_Y_samplesPartition;

	unsigned long long __workerID;

	int __LID; /* just for debug */
	int __previousNextRow[NUM_PLANES]; /* just for debug */
	int __previousNextColumn[NUM_PLANES]; /* just for debug */
	bool __firstCallTo__db_commomGetSamples[NUM_PLANES]; /* just for debug */
	// unsigned long long __timesCalled_waitCurrentBufferIsFull[NUM_PLANES]; /* just for debug */
	// Chronometer __crono_algorithmWaitingForSamplesTransference[NUM_PLANES]; /* just for debug */

#if LINUX
	static string str(LazyPicture * picture);
#endif
};


#endif /* LAZY_PICTURE_INTERNALS_H_ */

