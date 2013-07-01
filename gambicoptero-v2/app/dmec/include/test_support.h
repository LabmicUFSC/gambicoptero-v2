/*
 * test_support.h
 *
 *  Created on: Sep 9, 2011
 *      Author: mateus
 */

#ifndef TEST_SUPPORT_H_
#define TEST_SUPPORT_H_

#include "me_component.h"


#if LINUX
#include<iostream>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>

using namespace std;

#else
#include <utility/ostream.h>
#include <utility/malloc.h>

#endif


#define __TEST_SUPORT_GENERAL_TRACE 0 // (1 && ENABLE_TRACE)
#define __TEST_SUPORT_DETAILED_TRACE 0 // (1 && ENABLE_TRACE)
#define __TEST_SUPORT_DUMP 0 && ENABLE_DUMP
#define __TEST_SUPORT_ADVANCED_DUMP 0 && ENABLE_DUMP


class TestSupport {

public:
	static MEC_Picture* createRandomPicture(uint width, uint height);

	static MEC_Picture* createPicture(unsigned int width, unsigned int height, unsigned int dataSet);
		
	static void testPictureMotionCounterpart(PictureMotionCounterpart* pmc,
			unsigned int pictureWidth, unsigned int pictureHeight,
			MEC_Picture* currentPicture,
			MEC_Picture* referencePicture);

private:
	static MEC_Sample** __createSampleMatrix(int width, int height);

	static MEC_Sample** __createSpecificSampleMatrix(int width, int height, unsigned int dataSet);
	
	static void __testPictureMotionCounterpart(PictureMotionCounterpart* pmc,
		unsigned int pictureWidth, unsigned int pictureHeight,
		MEC_Picture* currentPicture,
		MEC_Picture** list0, unsigned int list0Size,
		unsigned int maxReferencePictures);

};


#endif /* TEST_SUPPORT_H_ */

