/*
 * lazy_picture.c
 *
 *  Created on: Jul 19, 2010
 *      Author: mateus
 */

#include "../include/master_tracedefs.h"

#include "../include/error.h"

#include "../include/abst_synchronizer.h"

#include "../include/lazy_picture.h"
#include "../include/dmec_picture_internals.h"
#include "../include/traits.h"


#if LINUX
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#include "../../../../linux/include/stringer.h"

#endif


#define __LAZY_PICTURE_GENERAL_TRACE  0 // (1 && ENABLE_TRACE)
#define __LAZY_PICTURE_DETAILED_TRACE 0 // (1 && ENABLE_TRACE)
#define __LAZY_PICTURE_OBSERVER_ENABLED 0
#define __LAZY_PICTURE_DEBUG 0

#define __LAZY_PICTURE_PRECONDITIONS 0 /* re enable me xxx */
#define __LAZY_PICTURE_POSCONDITIONS 0 /* re enable me xxx */

#define __LAZY_PICTURE_CRONO_DMA 1 && 0 /* !PC */


/* The following cronometer works only in CELL */
#if __LAZY_PICTURE_CRONO_DMA
#include <spu_mfcio.h>
#include <dmec_java/abst/cell/spu_chronometer_macros_version.h>

/* About the following variables, there is one variable for all LazyPictures
 * in a Worker. One for each plane. This works well only in CELL where each
 * worker has its private local memory. */
volatile unsigned long long _count;
volatile unsigned int _count_base;
volatile _Bool _counting;

#if !__LAZY_PICTURE_GENERATE_FULL_REPORT /* Gets the maximum TIMES measurements. */
#define TIMES 1200

static unsigned long long __dmaChronoMeasurements[TIMES];
static unsigned long long __dmaChronoMaxTime;
static unsigned int __dmaChronoMeasurementsIndex;
static unsigned int __dmaChronoMaxTimeIndex;
// static unsigned int __numOcurrencesDmaChronoMaxTime;

static unsigned long long __algorithmWaitingChronoMeasurements[TIMES];
static unsigned long long __algorithmWaitingChronoMaxTime;
static unsigned int __algorithmWaitingChronoMeasurementsIndex;
static unsigned int __algorithmWaitingChronoMaxTimeIndex;
// static unsigned int __numOcurrencesAlgorithmWaitingChronoMaxTime;
#endif

static unsigned long long __timesCalled_waitCurrentBufferIsFull[NUM_PLANES];

static unsigned int __numExecutedDMAs;

#endif /* __LAZY_PICTURE_CRONO_DMA */


/* Private Functions Declaration ------------------------------------------- */
void __generateContinuousTimeReport(unsigned long long workerID, unsigned long long data, char * nameOfData, bool lastTimeCalled);

/* ------------------------------------------------------------------------- */


#if __LAZY_PICTURE_OBSERVER_ENABLED
#include <dmec_java/distributed/mec_observer.h>
#endif

#if __LAZY_PICTURE_GENERAL_TRACE
static inline void trace(char * msg) {
	printf("%s\n", msg);
}
#endif

#if __LAZY_PICTURE_DETAILED_TRACE
static inline void traceSI(char * s, int i) {
	printf("...%s: %i\n", s, i);
}
#endif


#if __LAZY_PICTURE_DETAILED_TRACE
static inline void traceSLL(char * s, unsigned long long l) {
	printf("...%s: %llx\n", s, l);
}
#endif


#if __LAZY_PICTURE_DETAILED_TRACE
static inline void traceMEC_Sample(char * s, MEC_Sample l) {
	printf("...%s: %i\n", s, l);
}
#endif



void setUpLazyPicture(unsigned long long workerID,
		LazyPicture * picture,
		enum PictureType type,
		int absolute_x_in_picture,
		int absolute_y_in_picture,
		unsigned int picturePartitionWidth,
		unsigned int picturePartitionHeight,
		unsigned int pictureTotalWidth,
		unsigned int pictureTotalHeight,
		unsigned long long ea_info_dmec_picturePartition)
{
	//~ trace("setUpLazyPicture");

	picture->__LID = -1;
	picture->__type = type;

	EAs_DMEC_Picture * p_eas = (EAs_DMEC_Picture *) ((unsigned long) ea_info_dmec_picturePartition);
	EAs_DMEC_Picture eas = *p_eas;

	picture->width = picturePartitionWidth;
	picture->height = picturePartitionHeight;
	picture->numLumaSamples = picture->width * picture->height;

	picture->newStuff_ea_Y_samplesPartition = eas.ea_Y_samples;

#if __LAZY_PICTURE_OBSERVER_ENABLED
	mec_observer_check_ea_s_picture("setUpLazyPicture", picture->ea_ID,
			picture->ea_ID, picture->ea_Y_samples,
			picture->ea_Cb_samples, picture->ea_Cr_samples);
#endif

#if __LAZY_PICTURE_DETAILED_TRACE
	printf("setUpLazyPicture\n");
	// printf("...picture ID: %i\n", picture->ID);
	// printf("...ea_ID: %llx\n", picture->ea_ID);
	/*
	printf("...Reference EAs\n");
	printf("...ea_Y_samples: %llx\n", picture->ea_Y_samples);
	printf("...ea_Cb_samples: %llx\n", picture->ea_Cb_samples);
	printf("...ea_Cr_samples: %llx\n", picture->ea_Cr_samples);
	*/
	printf("...New EAs\n");
	printf("...worker: %lli\n", workerID);
	printf("...ea_Y_samples: %llx\n", picture->newStuff_ea_Y_samplesPartition);
	printf("...ea_Cb_samples: %llx\n", picture->newStuff_ea_Cb_samplesPartition);
	printf("...ea_Cr_samples: %llx\n", picture->newStuff_ea_Cr_samplesPartition);

	/*
	assert(picture->ea_Y_samples == picture->newStuff_ea_Y_samplesPartition);
	assert(picture->ea_Cb_samples == picture->newStuff_ea_Cb_samplesPartition);
	assert(picture->ea_Cr_samples == picture->newStuff_ea_Cr_samplesPartition);
	assert(2 == 9);
	*/

#endif

	picture->__workerID = workerID;

} /* end of function: setUpLazyPicture */


void tearDownLazyPicture(LazyPicture * picture)
{
} /* tearDownLazyPicture */


#if __LAZY_PICTURE_CRONO_DMA && !__LAZY_PICTURE_GENERATE_FULL_REPORT
static void __write_file_report(char * name, unsigned long long * data, unsigned long long maxTime, unsigned long long workerID)
{
	// TODO, REDO
	FILE *fp = fopen(name, "w");

	fprintf(fp, "time report, worker %lli:\n", workerID);
	fprintf(fp, "using SPU decrementer\n");
#if ENABLE_ASYNCH_TRANSF
	fprintf(fp, "using asynchronous DMA\n");
#else
	fprintf(fp, "using synchronous DMA\n");
#endif
	fprintf(fp, "number of workers: %i\n", NUM_WORKERS);
	//fprintf(fp, "buffer size (bytes): %li\n", BUFFER_SIZE * sizeof(unsigned short));
	fprintf(fp, "times: %i\n", TIMES);


	unsigned int plane;
	for (plane = 0; plane < NUM_PLANES; plane++) {
		fprintf(fp, "times called __waitCurrentBufferIsFull: %llu (plane: %u)\n", __timesCalled_waitCurrentBufferIsFull[plane], plane);
	}

	fprintf(fp, "Number of DMAs: %i", __numExecutedDMAs);

	bool containsMaxTime = false;
	unsigned int i;
	for (i = 0; i < TIMES; i ++) {
		fprintf(fp, "%llu\n", data[i]);
		if (data[i] == maxTime) {
			containsMaxTime = true;
		}
	}

	if (containsMaxTime) {
		fprintf(fp, "OK data contains maxTime\n");
	}
	else {
		fprintf(fp, "Data does not contain maxTime, non-representative data?\n");
	}

	fclose(fp);

}
#endif


void lp_generateTimeReport(unsigned long long workerID)
{
#if __LAZY_PICTURE_CRONO_DMA
#if __LAZY_PICTURE_GENERATE_FULL_REPORT
	__generateContinuousTimeReport(workerID, 0, "ctr_dma_transf", true);
	__generateContinuousTimeReport(workerID, 0, "ctr_algorithm_waiting", true);
#else
	const unsigned int name_size = 256;
	char name[name_size];

	snprintf(name, name_size, "dma_transf_w%lli.out", workerID);
	__write_file_report(name, __dmaChronoMeasurements, __dmaChronoMaxTime, workerID);

	snprintf(name, name_size, "algorithm_waiting_w%lli.out", workerID);
	__write_file_report(name, __algorithmWaitingChronoMeasurements, __algorithmWaitingChronoMaxTime, workerID);
#endif
#endif
}


void __generateContinuousTimeReport(unsigned long long workerID, unsigned long long data, char * nameOfData, bool lastTimeCalled)
{
#if __LAZY_PICTURE_CRONO_DMA && __LAZY_PICTURE_GENERATE_FULL_REPORT
	const unsigned int name_size = 256;
	char name[name_size];

	snprintf(name, name_size, "%s_w%lli.out", nameOfData, workerID);

	FILE *fp = fopen(name, "a");

	if  (lastTimeCalled) {
		fprintf(fp, "time report, worker %lli:\n", workerID);
		fprintf(fp, "using SPU decrementer\n");
	#if ENABLE_ASYNCH_TRANSF
		fprintf(fp, "using asynchronous DMA\n");
	#else
		fprintf(fp, "using synchronous DMA\n");
	#endif
		fprintf(fp, "number of workers: %i\n", NUM_WORKERS);
		fprintf(fp, "buffer size (bytes): %li\n", MAX_SAMPLES_LUMA_FOR_WORKER * sizeof(unsigned short));

		unsigned int plane;
		for (plane = 0; plane < NUM_PLANES; plane++) {
			fprintf(fp, "times called __waitCurrentBufferIsFull: %llu (plane: %u)\n", __timesCalled_waitCurrentBufferIsFull[plane], plane);
		}

		fprintf(fp, "Number of DMAs: %i", __numExecutedDMAs);
	}
	else {
		fprintf(fp, "%llu\n", data);
	}

	fclose(fp);
#endif
}


MEC_Sample lp_getSample(LazyPicture * picture, int row, int column)
{
	unsigned long long ea_sample, ea_plane;
	unsigned int width = picture->width;

	ea_plane = picture->newStuff_ea_Y_samplesPartition;

	int position = row * width + column;
	ea_sample = ea_plane + position * sizeof(MEC_Sample);

	unsigned long aux = (unsigned long) ea_sample;

	MEC_Sample * sample = (MEC_Sample *) aux;
	return *sample;
}


unsigned int lp_getWidth(LazyPicture * picture)
{
	///trace("lp_getWidth");
	return picture->width;
}


unsigned int lp_getHeight(LazyPicture * picture)
{
	return picture->height;
}


#if LINUX
string LazyPicture::str(LazyPicture * picture)
{
    string rep = "Picture:\n";

    unsigned int row, col;

    for (row = 0; row < picture->height; ++ row) {
        if (row < 10) {
            rep += "\n0" + Stringer::uint2str(row) + "=>";
        }
        else {
            rep += "\n" + Stringer::uint2str(row) + "=>";
        }

        for(col = 0; col < picture->width; ++ col) {
            rep += Stringer::ushort2str(lp_getSample(picture, row, col));
            rep += Stringer::separator();
        }
    }

    return rep;
}

#endif

