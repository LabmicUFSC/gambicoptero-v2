/*
 * dmec_picture_partitioner.cc
 *
 *  Created on: May 26, 2011
 *      Author: mateus
 */

#include "../include/picture_partitioner.h"
#include "../include/abst_assert.h"

#if LINUX
#include <stdlib.h>

#else
#include <utility/malloc.h>
#endif


PicturePartitionInfo ** __allocPicturePartitionInfo(unsigned numPartitionsInfo)
{
	PicturePartitionInfo ** ppi = (PicturePartitionInfo **) calloc(numPartitionsInfo, sizeof(PicturePartitionInfo *));

	unsigned int i;
	for (i = 0; i < numPartitionsInfo; i++) {
		ppi[i] = (PicturePartitionInfo *) malloc(sizeof(PicturePartitionInfo));
		ppi[i]->workerID = i;
	}

	return ppi;
}


static PicturePartitionInfo ** __singlePartitioning(unsigned int * out_numPartitionsInfo,
		unsigned int pictureWidth,
		unsigned int pictureHeight)
{
	PicturePartitionInfo ** ppi = __allocPicturePartitionInfo(1);
	out_numPartitionsInfo[0] = 1;

	ppi[0]->picturePartitionX = 0;
	ppi[0]->picturePartitionY = 0;
	ppi[0]->picturePartitionWidth = pictureWidth;
	ppi[0]->picturePartitionHeight = pictureHeight;

	Assert(ppi[0]->picturePartitionWidth % MACROBLOCK_SIZE == 0, "", "");
	Assert(ppi[0]->picturePartitionHeight % MACROBLOCK_SIZE == 0, "", "");

	return ppi;
}


static PicturePartitionInfo ** __oneByTwoPartitioning(unsigned int * out_numPartitionsInfo,
		unsigned int pictureWidth,
		unsigned int pictureHeight)
{
	/* Picture is partitioned in 1 in width and is partitioned in 2 in height,
	 * in the following way:
	 *
	 * |0|
	 * |1|
	 *
	 */

	PicturePartitionInfo ** ppi = __allocPicturePartitionInfo(2);
	out_numPartitionsInfo[0] = 2;

	unsigned int topHeight = ((pictureHeight / 32) * 32) / 2;
	unsigned int bottomHeight = pictureHeight - topHeight;

	ppi[0]->picturePartitionX = 0;
	ppi[0]->picturePartitionY = 0;
	ppi[0]->picturePartitionWidth = pictureWidth;
	ppi[0]->picturePartitionHeight = topHeight;

	ppi[1]->picturePartitionX = 0;
	ppi[1]->picturePartitionY = topHeight;
	ppi[1]->picturePartitionWidth = pictureWidth;
	ppi[1]->picturePartitionHeight = bottomHeight;


	Assert(ppi[0]->picturePartitionWidth % MACROBLOCK_SIZE == 0, "PicturePartitioner", "1x2");
	Assert(ppi[0]->picturePartitionHeight % MACROBLOCK_SIZE == 0, "PicturePartitioner", "1x2");
	Assert(ppi[1]->picturePartitionWidth % MACROBLOCK_SIZE == 0, "PicturePartitioner", "1x2");
	Assert(ppi[1]->picturePartitionHeight % MACROBLOCK_SIZE == 0, "PicturePartitioner", "1x2");

	return ppi;
}


static PicturePartitionInfo ** __threeByOnePartitioning(unsigned int * out_numPartitionsInfo,
		unsigned int pictureWidth,
		unsigned int pictureHeight)
{
	/* Picture is partitioned in 3 in width and is partitioned in 1 in height,
	 * in the following way:
	 *
	 * |0|1|2|
	 *
	 */

	PicturePartitionInfo ** ppi = __allocPicturePartitionInfo(3);
	out_numPartitionsInfo[0] = 3;

	unsigned int leftWidth = ((pictureWidth / 48) * 48) / 3;
	unsigned int centerWidth = leftWidth;
	unsigned int rightWidth = pictureWidth - (leftWidth + centerWidth);

	ppi[0]->picturePartitionX = 0;
	ppi[0]->picturePartitionY = 0;
	ppi[0]->picturePartitionWidth = leftWidth;
	ppi[0]->picturePartitionHeight = pictureHeight;

	ppi[1]->picturePartitionX = leftWidth;
	ppi[1]->picturePartitionY = 0;
	ppi[1]->picturePartitionWidth = centerWidth;
	ppi[1]->picturePartitionHeight = pictureHeight;

	ppi[2]->picturePartitionX = leftWidth + centerWidth;
	ppi[2]->picturePartitionY = 0;
	ppi[2]->picturePartitionWidth = rightWidth;
	ppi[2]->picturePartitionHeight = pictureHeight;

	for (unsigned int i = 0; i < 3; i++) {
		Assert(ppi[i]->picturePartitionWidth % MACROBLOCK_SIZE == 0, "PicturePartitioner", "3x1:W");
		Assert(ppi[i]->picturePartitionHeight % MACROBLOCK_SIZE == 0, "PicturePartitioner", "3x1:H");
	}

	return ppi;
}


static PicturePartitionInfo ** __twoByTwoPartitioning(unsigned int * out_numPartitionsInfo,
		unsigned int pictureWidth,
		unsigned int pictureHeight)
{
	/* Picture is partitioned in 2 in width and is partitioned in 2 in height,
	 * in the following way:
	 *
	 * |0|1|
	 * |2|3|
	 *
	 */

	PicturePartitionInfo ** ppi = __allocPicturePartitionInfo(4);
	out_numPartitionsInfo[0] = 4;

	unsigned int topHeight = ((pictureHeight / 32) * 32) / 2;
	unsigned int bottomHeight = pictureHeight - topHeight;
	unsigned int leftWidth = ((pictureWidth / 32) * 32) / 2;
	unsigned int rightWidth = pictureWidth - leftWidth;


	ppi[0]->picturePartitionX = 0;
	ppi[0]->picturePartitionY = 0;
	ppi[0]->picturePartitionWidth = leftWidth;
	ppi[0]->picturePartitionHeight = topHeight;

	ppi[1]->picturePartitionX = (int) leftWidth;
	ppi[1]->picturePartitionY = 0;
	ppi[1]->picturePartitionWidth = rightWidth;
	ppi[1]->picturePartitionHeight = topHeight;

	ppi[2]->picturePartitionX = 0;
	ppi[2]->picturePartitionY = (int) topHeight;
	ppi[2]->picturePartitionWidth = leftWidth;
	ppi[2]->picturePartitionHeight = bottomHeight;

	ppi[3]->picturePartitionX = (int) leftWidth;
	ppi[3]->picturePartitionY = (int) topHeight;
	ppi[3]->picturePartitionWidth = rightWidth;
	ppi[3]->picturePartitionHeight = bottomHeight;


	for (unsigned int i = 0; i < 4; i++) {
		Assert(ppi[i]->picturePartitionWidth % MACROBLOCK_SIZE == 0, "PicturePartitioner", "2x2:W");
		Assert(ppi[i]->picturePartitionHeight % MACROBLOCK_SIZE == 0, "PicturePartitioner", "2x2:H");
	}

	return ppi;
}


static PicturePartitionInfo ** __three_twoByTwoPartitioning(unsigned int * out_numPartitionsInfo,
		unsigned int pictureWidth,
		unsigned int pictureHeight)
{
	/* Picture is partitioned in 3 in width and is partitioned in 2 in height,
	 * in the following way:
	 *
	 * |0|1|2|
	 * |3 | 4|
	 *
	 */

	PicturePartitionInfo ** ppi = __allocPicturePartitionInfo(5);
	out_numPartitionsInfo[0] = 5;

	unsigned int topHeight = ((pictureHeight / 32) * 32) / 2;
	unsigned int bottomHeight = pictureHeight - topHeight;

	unsigned int leftWidthF = ((pictureWidth / 48) * 48) / 3;
	unsigned int centerWidthF = leftWidthF;
	unsigned int rightWidthF = pictureWidth - (leftWidthF + centerWidthF);

	unsigned int leftWidthS = ((pictureWidth / 32) * 32) / 2;
	unsigned int rightWidthS = pictureWidth - leftWidthS;


	ppi[0]->picturePartitionX = 0;
	ppi[0]->picturePartitionY = 0;
	ppi[0]->picturePartitionWidth = leftWidthF;
	ppi[0]->picturePartitionHeight = topHeight;

	ppi[1]->picturePartitionX = leftWidthF;
	ppi[1]->picturePartitionY = 0;
	ppi[1]->picturePartitionWidth = centerWidthF;
	ppi[1]->picturePartitionHeight = topHeight;

	ppi[2]->picturePartitionX = leftWidthF + centerWidthF;
	ppi[2]->picturePartitionY = 0;
	ppi[2]->picturePartitionWidth = rightWidthF;
	ppi[2]->picturePartitionHeight = topHeight;

	ppi[3]->picturePartitionX = 0;
	ppi[3]->picturePartitionY = topHeight;
	ppi[3]->picturePartitionWidth = leftWidthS;
	ppi[3]->picturePartitionHeight = bottomHeight;

	ppi[4]->picturePartitionX = leftWidthS;
	ppi[4]->picturePartitionY = topHeight;
	ppi[4]->picturePartitionWidth = rightWidthS;
	ppi[4]->picturePartitionHeight = bottomHeight;

	for (unsigned int i = 0; i < 5; i++) {
		Assert(ppi[i]->picturePartitionWidth % MACROBLOCK_SIZE == 0, "PicturePartitioner", "3.2x2:W");
		Assert(ppi[i]->picturePartitionHeight % MACROBLOCK_SIZE == 0, "PicturePartitioner", "3.2x2:H");
	}

	return ppi;
}


static PicturePartitionInfo ** __threeByTwoPartitioning(unsigned int * out_numPartitionsInfo,
		unsigned int pictureWidth,
		unsigned int pictureHeight)
{
	/* Picture is partitioned in 3 in width and is partitioned in 2 in height,
	 * in the following way:
	 *
	 * |0|1|2|
	 * |3|4|5|
	 *
	 */
	PicturePartitionInfo ** ppi = __allocPicturePartitionInfo(6);
	out_numPartitionsInfo[0] = 6;

	unsigned int leftWidth = ((pictureWidth / 48) * 48) / 3;
	unsigned int centerWidth = leftWidth;
	unsigned int rightWidth = pictureWidth - (leftWidth + centerWidth);
	unsigned int topHeight = ((pictureHeight / 32) * 32) / 2;
	unsigned int bottomHeight = pictureHeight - topHeight;


	ppi[0]->picturePartitionX = 0;
	ppi[0]->picturePartitionY = 0;
	ppi[0]->picturePartitionWidth = leftWidth;
	ppi[0]->picturePartitionHeight = topHeight;

	ppi[1]->picturePartitionX = (int) leftWidth;
	ppi[1]->picturePartitionY = 0;
	ppi[1]->picturePartitionWidth = centerWidth;
	ppi[1]->picturePartitionHeight = topHeight;

	ppi[2]->picturePartitionX = (int) leftWidth + centerWidth;
	ppi[2]->picturePartitionY = 0;
	ppi[2]->picturePartitionWidth = rightWidth;
	ppi[2]->picturePartitionHeight = topHeight;

	ppi[3]->picturePartitionX = 0;
	ppi[3]->picturePartitionY = (int) topHeight;
	ppi[3]->picturePartitionWidth = leftWidth;
	ppi[3]->picturePartitionHeight = bottomHeight;

	ppi[4]->picturePartitionX = (int) leftWidth;
	ppi[4]->picturePartitionY = (int) topHeight;
	ppi[4]->picturePartitionWidth = centerWidth;
	ppi[4]->picturePartitionHeight = bottomHeight;

	ppi[5]->picturePartitionX = (int) leftWidth + centerWidth;
	ppi[5]->picturePartitionY = (int) topHeight;
	ppi[5]->picturePartitionWidth = rightWidth;
	ppi[5]->picturePartitionHeight = bottomHeight;

	for (unsigned int i = 0; i < 6; i++) {
		Assert(ppi[i]->picturePartitionWidth % MACROBLOCK_SIZE == 0, "PicturePartitioner", "3x2:W");
		Assert(ppi[i]->picturePartitionHeight % MACROBLOCK_SIZE == 0, "PicturePartitioner", "3x2:H");
	}

	return ppi;
}


PicturePartitionInfo ** pp_createPartitions(unsigned int * out_numPartitionsInfo,
		enum PartitionModel model,
		unsigned int totalPictureWidth,
		unsigned int totalPictureHeight)
{
	Assert(totalPictureWidth >= 48, "", "");
	Assert(totalPictureHeight >= 32, "", "");
	Assert(totalPictureWidth % 16 == 0, "", "");
	Assert(totalPictureHeight % 16 == 0, "", "");

	switch (model) {
		case SINGLE_PARTITION:
			return __singlePartitioning(out_numPartitionsInfo, totalPictureWidth, totalPictureHeight);

		case ONExTWO_PARTITION:
			return __oneByTwoPartitioning(out_numPartitionsInfo, totalPictureWidth, totalPictureHeight);

		case THREExONE_PARTITION:
			return __threeByOnePartitioning(out_numPartitionsInfo, totalPictureWidth, totalPictureHeight);

		case TWOxTWO_PARTITION:
			return __twoByTwoPartitioning(out_numPartitionsInfo, totalPictureWidth, totalPictureHeight);

		case THREE_TWOxTWO_PARTITION:
			return __three_twoByTwoPartitioning(out_numPartitionsInfo, totalPictureWidth, totalPictureHeight);

		case THREExTWO_PARTITION:
			return __threeByTwoPartitioning(out_numPartitionsInfo, totalPictureWidth, totalPictureHeight);

		default:
			Assert(false, "PicturePartition", "Unknown partition model");
			return 0;
	}
}
