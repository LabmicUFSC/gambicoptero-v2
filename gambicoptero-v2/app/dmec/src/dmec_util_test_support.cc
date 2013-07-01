/*
 * dmec_util_test_support.cc
 *
 *  Created on: Sep 9, 2011
 *      Author: mateus
 */

#include "../include/test_support.h"
#include "../include/error.h"


#if !LINUX
extern System::OStream cout;
#endif


#define REF_PIC_ID_ONE 43

#define MAX_PEL_SIZE 10
/* #define MAX_PEL_SIZE SHRT_MAX/2 */



// Globals ---------------------------------------------------------------------
static MEC_Sample __samples[2][32][48] = {
		{{2,2,3,7,1,6,2,6,8,6,8,1,3,3,8,8,2,7,2,1,4,7,8,9,6,8,1,8,1,3,1,5,1,1,7,7,4,7,3,3,3,2,4,7,6,3,5,8},
		{8,5,5,9,4,1,7,5,9,9,1,5,3,2,7,7,1,3,2,5,9,9,6,7,9,4,7,4,6,9,2,4,4,2,3,3,3,9,4,6,1,1,4,1,2,5,5,5},
		{1,6,4,2,1,7,9,4,8,5,4,8,2,4,2,6,3,6,3,4,8,7,5,2,1,5,4,7,9,5,5,8,9,6,5,3,6,6,2,9,5,5,3,2,8,8,6,4},
		{1,9,5,6,3,2,6,8,2,7,4,7,3,7,3,7,3,9,3,9,6,9,3,9,8,1,4,6,1,4,1,8,7,4,4,6,7,1,1,8,8,5,5,1,2,9,1,3},
		{2,1,1,4,9,6,9,4,3,9,4,9,7,8,3,3,6,4,5,4,3,9,5,2,9,6,6,1,7,6,3,2,5,3,9,5,8,2,6,7,7,5,4,6,8,8,2,3},
		{4,6,6,4,6,1,7,5,7,3,6,5,8,5,4,7,1,3,7,1,7,2,6,3,5,5,3,9,1,3,7,1,3,9,4,5,4,3,5,1,1,9,1,8,8,7,1,1},
		{2,6,5,2,8,5,6,6,7,6,9,7,1,9,2,5,6,5,5,7,6,5,8,5,6,6,5,3,8,7,6,4,2,9,3,8,5,9,5,2,8,2,3,8,5,8,8,2},
		{4,4,9,8,5,6,5,1,9,3,8,6,8,5,1,9,1,4,2,7,5,8,3,4,7,5,8,2,5,9,2,3,5,8,8,6,8,2,5,4,6,9,2,5,3,3,3,7},
		{9,1,3,9,6,2,1,1,1,3,5,7,2,3,4,6,9,5,6,6,5,8,1,5,2,1,8,5,9,8,7,1,7,6,4,8,8,6,2,3,3,4,6,7,5,3,7,9},
		{3,5,8,5,1,1,7,7,6,8,4,6,2,3,4,1,3,4,4,8,1,1,3,4,9,4,4,2,2,7,1,3,4,1,1,9,5,8,7,1,4,8,2,5,2,7,8,2},
		{4,2,1,8,6,3,8,9,1,3,6,1,4,8,1,6,2,8,7,7,6,2,8,7,3,9,5,5,3,8,6,2,4,1,4,1,1,7,8,2,1,6,4,1,2,6,9,3},
		{6,6,5,8,9,4,3,4,8,1,1,6,1,1,2,2,8,1,5,7,1,7,1,1,8,1,7,3,9,7,8,3,2,6,5,3,7,1,4,9,2,7,2,8,7,6,8,2},
		{7,5,4,7,6,3,9,4,6,9,2,5,2,5,2,7,9,9,9,3,9,2,1,1,2,8,3,6,7,6,3,3,1,3,1,5,7,6,3,3,8,2,8,5,7,2,7,8},
		{5,7,7,8,4,4,5,8,8,6,8,9,1,3,2,7,5,3,5,6,5,4,6,1,2,9,9,4,7,8,3,2,3,8,1,4,1,9,4,3,2,5,5,3,2,7,7,7},
		{3,2,3,4,4,2,4,5,6,1,3,3,6,8,4,8,2,5,7,6,8,9,1,4,3,4,4,2,1,3,4,5,8,8,8,2,4,5,4,7,5,8,4,3,2,2,6,8},
		{3,7,5,1,3,9,6,3,1,8,6,5,4,5,4,2,7,8,7,2,6,3,1,6,3,4,2,1,4,6,9,5,1,1,8,7,1,1,6,8,6,3,1,2,1,3,7,3},
		{4,8,9,7,2,2,9,7,8,8,2,9,1,7,3,1,3,1,8,3,5,6,7,6,1,7,9,5,6,9,2,9,2,1,6,1,9,1,9,1,6,3,3,5,1,3,1,2},
		{9,8,2,6,9,5,4,6,5,7,5,7,8,2,9,9,1,8,2,2,5,6,5,6,2,7,9,3,9,9,2,2,5,2,2,1,7,8,7,4,4,5,3,6,4,2,8,5},
		{1,7,7,7,3,3,9,3,2,2,5,2,8,7,5,4,4,7,9,8,3,2,5,7,5,3,3,6,4,3,1,3,6,4,9,3,4,2,9,7,1,6,2,1,1,6,9,1},
		{5,2,3,9,3,3,5,8,4,9,9,8,6,2,6,4,7,8,3,4,9,7,2,1,4,1,2,9,4,6,9,5,5,3,9,1,1,3,1,1,1,8,1,3,4,6,3,9},
		{5,3,9,2,5,6,7,3,6,2,9,6,7,5,9,6,7,1,9,3,1,7,5,5,2,1,6,5,1,9,9,4,4,1,2,3,7,4,3,3,1,9,3,8,7,5,8,6},
		{8,9,4,3,5,6,4,1,1,4,2,4,7,5,4,8,7,7,6,1,1,3,4,1,8,9,5,7,9,9,1,5,5,5,6,7,9,8,4,6,4,9,5,1,7,2,3,8},
		{2,7,9,6,8,7,6,9,3,3,9,2,5,4,8,6,1,9,4,6,6,8,5,1,2,4,3,1,6,1,3,9,9,9,9,2,3,8,4,8,2,3,8,2,9,2,3,6},
		{8,2,2,2,4,4,6,6,7,7,4,7,7,3,6,9,6,4,4,4,7,7,3,9,7,3,2,1,3,2,6,7,5,3,3,6,1,1,8,8,3,5,8,9,2,4,7,7},
		{7,7,1,9,8,1,2,4,1,8,1,6,2,4,2,3,6,3,2,5,6,7,3,6,8,5,1,6,6,8,4,3,4,2,2,4,4,6,5,2,7,2,5,2,5,8,8,8},
		{3,4,8,7,1,6,3,2,3,9,1,7,3,1,1,5,3,5,9,9,1,2,7,3,7,9,8,5,8,6,5,2,1,4,9,3,2,5,3,2,6,1,5,8,2,2,8,9},
		{2,2,9,3,9,9,2,5,7,9,1,9,1,5,9,4,6,2,9,4,6,1,9,5,4,9,7,2,9,9,2,1,9,6,1,8,9,6,5,8,7,5,9,2,4,3,9,6},
		{3,9,1,9,5,4,2,7,2,2,1,7,3,9,6,5,4,7,3,5,7,9,2,9,3,9,3,5,4,4,7,2,4,2,6,1,9,6,5,7,6,4,8,1,1,8,4,2},
		{7,7,2,7,1,8,1,5,6,3,6,6,7,7,6,7,8,2,7,3,3,5,7,4,1,9,2,6,1,3,4,7,4,2,9,9,3,1,5,9,8,3,6,5,1,9,2,5},
		{2,8,5,2,7,9,4,3,1,5,6,5,2,1,8,1,2,8,4,3,3,4,2,6,1,7,8,3,2,8,8,2,1,6,2,5,9,3,6,9,3,8,7,4,5,9,4,9},
		{4,6,6,5,4,4,5,5,7,8,3,6,4,2,3,2,6,1,8,5,5,6,3,7,9,9,2,8,3,9,5,9,3,9,4,8,7,2,2,3,3,7,6,6,7,8,3,9},
		{7,8,4,4,2,4,1,1,4,4,9,8,3,4,7,6,4,2,5,1,4,7,5,8,4,2,5,2,8,2,8,7,7,2,9,2,4,6,8,4,1,5,2,8,9,4,3,1}},

		{{3,3,6,7,7,9,6,1,2,4,1,5,1,1,1,9,3,1,5,9,8,5,1,7,7,3,3,4,3,7,1,5,9,7,5,9,2,9,5,3,1,7,3,5,9,9,5,8},
		{5,6,5,3,7,9,7,1,2,4,1,4,9,1,1,5,2,1,6,4,1,2,8,2,2,8,9,1,3,7,7,3,7,2,7,9,5,2,1,2,2,1,7,4,3,3,9,4},
		{6,7,6,6,9,5,5,1,8,3,9,8,7,6,7,5,5,6,6,2,9,7,9,4,3,3,5,9,1,1,5,1,7,1,3,5,4,2,3,2,2,2,5,7,2,4,2,2},
		{1,2,6,2,6,7,6,2,9,2,3,6,3,7,2,8,9,6,2,6,2,3,2,4,5,4,6,7,6,9,4,9,6,5,7,2,8,6,4,2,3,8,9,5,7,9,1,7},
		{2,8,5,1,9,5,6,6,3,2,6,8,2,2,3,9,5,1,6,9,4,9,8,3,5,5,2,7,3,4,5,4,5,5,1,1,6,5,4,3,3,6,5,7,5,9,1,3},
		{1,9,6,6,4,9,3,8,4,9,2,4,5,7,4,6,9,1,2,3,4,5,9,2,5,1,1,6,2,4,8,1,1,5,6,8,4,4,8,6,9,3,4,4,3,5,2,6},
		{7,6,1,1,1,8,4,6,6,5,8,1,1,9,6,4,8,3,3,1,7,8,2,1,3,4,7,4,1,3,4,2,5,6,3,3,9,6,8,1,8,8,2,4,2,7,5,2},
		{5,3,4,6,7,9,4,1,1,7,6,3,3,3,6,6,4,7,6,6,7,8,4,3,6,1,2,3,6,6,8,2,4,7,6,6,8,4,2,2,7,6,8,4,6,1,3,8},
		{8,8,3,4,9,4,7,3,4,2,3,8,4,9,5,7,1,2,3,6,4,6,7,5,9,5,5,8,4,9,2,4,3,7,4,4,9,7,2,9,8,7,8,3,4,9,6,7},
		{9,3,2,6,7,6,6,3,4,1,2,7,5,6,7,2,1,2,2,9,5,2,7,2,1,6,6,4,6,6,4,3,7,8,7,8,1,5,3,7,2,6,8,5,9,7,1,5},
		{2,2,5,9,3,2,3,3,2,3,5,4,3,1,9,4,6,3,2,9,9,1,5,9,8,5,4,8,3,9,7,6,2,1,5,1,1,9,6,5,7,9,4,4,2,7,3,2},
		{8,9,1,7,5,2,1,3,5,1,1,1,4,2,2,9,4,3,9,1,8,9,1,5,9,6,9,2,3,3,4,2,3,6,9,8,8,1,3,1,2,5,5,5,8,5,1,4},
		{2,4,3,5,4,5,6,4,9,1,6,2,8,5,6,5,2,7,5,6,2,4,8,1,5,2,3,5,7,7,1,1,7,7,8,1,6,8,9,8,3,9,4,9,1,5,6,6},
		{1,9,7,6,7,8,2,2,7,7,9,9,9,6,6,8,8,3,8,7,2,7,2,2,4,7,7,9,6,8,5,3,7,7,4,9,5,2,8,5,2,5,1,4,5,9,1,8},
		{6,2,8,3,4,5,5,5,6,4,1,8,5,1,4,7,9,9,5,1,9,6,8,2,4,5,5,5,3,8,9,9,8,5,4,5,9,5,6,1,4,1,2,8,8,2,7,7},
		{8,9,7,8,5,6,1,1,6,6,5,5,6,4,4,1,9,9,4,6,1,9,7,4,7,6,6,5,4,4,4,1,3,8,5,2,6,5,7,2,1,4,9,1,5,4,7,7},
		{3,5,1,1,2,7,6,6,2,8,6,6,1,1,6,3,8,2,1,1,2,2,5,2,4,6,2,7,8,8,1,6,8,6,8,8,4,4,9,1,6,1,8,9,1,1,3,4},
		{6,6,8,6,3,1,4,3,9,3,5,9,9,3,5,8,2,3,2,1,9,3,9,3,4,4,4,1,3,7,4,4,2,7,4,5,3,3,4,6,9,2,8,9,6,1,1,5},
		{4,1,5,7,2,5,2,7,3,8,4,2,9,2,3,8,3,6,2,5,5,9,5,1,7,7,4,8,2,2,1,8,4,8,9,7,6,3,8,9,2,1,9,5,9,3,2,2},
		{3,7,8,2,3,2,7,6,9,9,8,4,8,7,2,5,1,1,4,3,2,4,9,2,7,1,5,1,9,3,3,2,3,5,3,1,2,2,1,2,3,9,6,2,7,8,7,9},
		{2,3,2,7,2,5,4,4,5,3,3,9,6,7,4,8,7,3,6,6,8,8,5,5,7,5,8,8,5,3,3,7,4,6,1,8,5,8,5,8,4,8,2,5,8,8,5,5},
		{6,3,2,6,1,4,6,4,8,7,1,2,3,3,8,1,6,9,1,7,2,6,6,1,1,7,4,1,9,5,8,6,3,7,1,4,1,4,8,4,5,6,6,1,6,6,2,4},
		{9,8,1,7,5,2,7,1,8,6,4,6,6,5,8,8,3,5,5,9,6,2,6,9,6,6,8,6,6,8,7,4,3,1,4,1,5,6,7,1,7,5,1,6,8,7,2,7},
		{3,4,4,3,1,7,6,4,4,5,2,8,6,3,9,2,6,4,7,1,6,4,9,3,6,7,7,1,1,2,1,2,9,8,2,4,9,7,9,1,5,5,5,4,8,5,1,2},
		{3,2,8,7,2,1,3,9,9,5,9,7,2,2,6,5,9,2,1,8,5,6,3,3,1,4,6,5,6,4,2,8,6,6,8,5,1,9,5,9,1,8,5,2,3,6,1,5},
		{6,3,4,2,9,7,6,1,2,2,6,9,7,9,8,4,5,6,7,5,6,6,7,4,2,9,7,9,3,6,3,7,8,3,5,5,5,7,8,1,6,6,4,6,1,1,3,6},
		{7,3,2,2,3,9,2,4,3,8,7,1,6,1,6,1,6,4,8,1,6,9,5,7,5,6,8,9,3,6,7,8,3,7,5,4,1,3,2,2,9,4,8,1,3,2,9,4},
		{8,8,5,4,7,4,4,4,8,8,7,9,5,2,4,7,5,6,9,5,1,8,7,1,1,1,5,8,9,5,4,3,2,8,7,3,8,1,1,7,9,1,3,2,5,8,2,9},
		{4,3,7,1,5,8,2,6,4,5,5,9,3,4,6,6,2,8,8,7,9,3,9,5,3,7,7,2,1,3,5,9,6,2,3,6,7,3,2,6,7,6,3,3,9,1,2,9},
		{9,2,2,9,7,5,7,5,8,9,6,8,3,1,7,9,3,6,1,4,8,8,2,4,1,6,4,2,8,3,2,1,6,1,9,1,8,5,7,1,8,1,3,5,6,6,6,8},
		{1,4,6,4,9,8,4,1,9,7,3,4,5,5,3,5,6,1,6,4,2,9,3,6,7,9,5,9,4,1,3,4,3,8,1,2,5,4,3,9,3,5,3,9,2,5,5,6},
		{8,5,7,1,1,4,1,6,3,5,8,6,9,8,8,9,2,4,4,5,4,4,9,8,8,2,4,3,8,2,9,5,4,6,7,4,1,6,1,1,5,1,9,5,4,3,9,9}}
};



// ----------------------------------------------------------------------------



MEC_Sample ** TestSupport::__createSampleMatrix(int width, int height)
{
	MEC_Sample ** samples;
	samples = (MEC_Sample **) calloc(height, sizeof(MEC_Sample *));

	int i;
	for (i = 0; i < height; ++ i) {
		samples[i] = (MEC_Sample *) calloc(width, sizeof(MEC_Sample));
	}

	int row, col;
	for (row = 0; row < height; ++ row) {
		for (col = 0; col < width; ++ col) {
			samples[row][col] = random_give_non_zero_int_smaller_than(MAX_PEL_SIZE);
		}
	}

	return samples;
}


MEC_Sample ** TestSupport::__createSpecificSampleMatrix(int width, int height, unsigned int dataSet) {
	MEC_Sample ** samples = (MEC_Sample **) calloc(height, sizeof(MEC_Sample *));

	int i;
	for (i = 0; i < height; ++ i) {
		samples[i] = (MEC_Sample *) calloc(width, sizeof(MEC_Sample));
	}

	int row, col;
	for (row = 0; row < height; ++ row) {
		for (col = 0; col < width; ++ col) {
			samples[row][col] = __samples[dataSet][row%32][col%48]; /* These
			modules (%) guarantee that row and columns of __samples are
			respected, although it is not the best solution of the world.  */
		}
	}

	return samples;
}



MEC_Picture * TestSupport::createRandomPicture(uint width, uint height)
{
    MEC_Picture * picture;
    MEC_Sample ** Y_samples = __createSampleMatrix(width, height);

    picture = new MEC_Picture(width, height, Y_samples);

    return picture;
}


MEC_Picture * TestSupport::createPicture(unsigned int width, unsigned int height, unsigned int dataSet)
{
	MEC_Picture * picture;
    MEC_Sample ** Y_samples = __createSpecificSampleMatrix(width, height, dataSet);

    picture = new MEC_Picture(width, height, Y_samples);

    return picture;
}


void TestSupport::testPictureMotionCounterpart(PictureMotionCounterpart* pmc,
			unsigned int pictureWidth, unsigned int pictureHeight,
			MEC_Picture* currentPicture,
			MEC_Picture* referencePicture)
{
    MEC_Picture** list0 = (MEC_Picture **) calloc(1, sizeof(MEC_Picture *));
    list0[0] = referencePicture;   

    __testPictureMotionCounterpart(pmc,
		pictureWidth, pictureHeight,
		currentPicture,
		list0, 1,
		1);
		
    free(list0);		
}


/* Works with Vectorial PictureMotionCounterpart */
void TestSupport::__testPictureMotionCounterpart(PictureMotionCounterpart * pmc,
		unsigned int pictureWidth, unsigned int pictureHeight,
		MEC_Picture * currentPicture,
		MEC_Picture ** list0, unsigned int list0Size,
		unsigned int maxReferencePictures)
{
	unsigned int numPartitions[1];

	enum PartitionModel partitionModel;
	switch (NUM_WORKERS) {
		case 1:
			partitionModel = SINGLE_PARTITION;
			break;
		case 2:
			partitionModel = ONExTWO_PARTITION;
			break;
		case 3:
			partitionModel = THREExONE_PARTITION;
			break;
		case 4:
			partitionModel = TWOxTWO_PARTITION;
			break;
		case 5:
			partitionModel = THREE_TWOxTWO_PARTITION;
			break;
		case 6:
			partitionModel = THREExTWO_PARTITION;
			break;
		default:
			Assert(false, "Coordinator", "Unknown partition type");
			break;
	}

	// trace("testPMC_and_beyond_v2, creating picture partitions");
	PicturePartitionInfo ** ppi = 0;
	ppi = pp_createPartitions(numPartitions, partitionModel, pictureWidth, pictureHeight);
	// assert(ppi != 0x0);

	// trace("testPMC_and_beyond_v2, creating picture partitions DONE");

	Assert(numPartitions[0] == NUM_WORKERS, "", "");

	unsigned int flatPMC_MaxSize = MAX_FLAT_PMC_SIZE_FOR_COORDINATOR;

	MEC_SearchWindow searchWindow;
	searchWindow.centerX = 0;
	searchWindow.centerY = 0;
	searchWindow.searchRange = SEARCH_RANGE;

	PictureMotionCounterpart * referencePMC = (PictureMotionCounterpart *) malloc(sizeof(PictureMotionCounterpart));
	setUpPictureMotionCounterpart(referencePMC,
				pictureWidth,
				pictureHeight,
				maxReferencePictures);

	pmc_set(referencePMC, list0, list0Size, 0, 0);

    unsigned int i;

    // trace("testPMC_and_beyond_v2, will iterate over partitions");
    for (i = 0; i < numPartitions[0]; i++) {
    	// match
    	FlatH264PMC_Node * flatPMC = (FlatH264PMC_Node *) calloc(flatPMC_MaxSize, sizeof(FlatH264PMC_Node));
    	unsigned int out_flatPMC_Size[1];

    	referenceBMA_match(flatPMC,
    			out_flatPMC_Size,
    			flatPMC_MaxSize,
				currentPicture,
				list0,
				list0Size,
				pictureWidth,
				pictureHeight,
				&searchWindow,
				ppi[i]->workerID,
				ppi[i]->picturePartitionX,
				ppi[i]->picturePartitionY,
				ppi[i]->picturePartitionWidth,
				ppi[i]->picturePartitionHeight);

    	// gathering results
    	// assert(out_flatPMC_Size[0] > 0);
    	// assert(out_flatPMC_Size[0] < flatPMC_MaxSize);
    	// assert(ppi[i]->workerID >=0 && ppi[i]->workerID <= numPartitions[0]);
		pmc_fillPMC_WithFlatPMC(referencePMC, flatPMC, out_flatPMC_Size[0], ppi[i]->workerID);
		free(flatPMC);
    }

    //trace("testPMC_and_beyond_v2, iterate over partitions DONE");
    // test
    Assert(pmc_equals(pmc, referencePMC), "DMEC_App", "ME not performed correctly");

    // deletions
    for (i = 0; i < numPartitions[0]; i++) {
    	free(ppi[i]);
    }

    free(ppi);
    free(referencePMC);
}


// ----------------------------------------------------------------------------

