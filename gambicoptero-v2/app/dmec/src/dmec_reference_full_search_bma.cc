/*
 * ReferenceFullSearchV01
 * Reference Full Search Block-Match Algorithm - version 01
 *
 * - No Rate
 * - Raster Order
 *
 *
 *  Created on: Jul 30, 2010
 *      Author: mateus
 */

#include "../include/reference_bma.h"
#include "../include/error.h"

#include "../include/block_mode_type.h"
#include "../include/traits.h"
#include "../include/abst_assert.h"
#include "../include/lazy_flat_h264_pmc.h"

#include "../include/master_tracedefs.h"

#if LINUX
#include <iostream>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>

using namespace std;

#else
#include <utility/ostream.h>
typedef System::OStream OStream;
extern OStream cout;

#endif


#define __REFERENCE_FULL_SEARCH_BMA_V01_DUMP 1 && ENABLE_DUMP

#define MACROBLOCK_SIZE 16
#define maxfs(a,b) (((a) > (b)) ? (a) : (b))
#define minfs(a,b) (((a) < (b)) ? (a) : (b))


// Private Type
typedef struct FullSearchMatchAlgorithm {
    FlatH264PMC_Node auxFlatPCM_Node;
    int block_width, block_height;
    int partition_x;
    int partition_y;
    unsigned int partition_width;
    unsigned int partition_height;


    unsigned int flatH264PMC_Size;
    unsigned int indexFlatPMC;
    MotionVector mvs[160][160];

    FlatH264PMC_Node * out_flatPMC;
    unsigned int out_flatPMC_Size;

} MatchAlgorithm;

// globals
static MatchAlgorithm __algorithm[NUM_WORKERS];



// "private" functions --------------------------------------------------------
void __getMotionVector(MotionVector * mv,
        int xInPartition,
        int yInPartition,
        int xInMacroblock,
        int yInMacroblock,
        unsigned int workerID)
{
    int x, y;
    x = (xInPartition >> 2) + (xInMacroblock >> 2);
    y = (yInPartition >> 2) + (yInMacroblock >> 2);

    mv->mv_x = __algorithm[workerID].mvs[x][y].mv_x;
    mv->mv_y = __algorithm[workerID].mvs[x][y].mv_y;

}

static inline void predicted_motion_vector(int pred[], unsigned int workerID) {
    //BlockModeType type = MODE4x4;
    MotionVector mv;
    int mv_top[2];
    int mv_top_right[2];
    int mv_left[2];
    mv_top[0] = 0;
    mv_top[1] = 0;
    mv_top_right[0] = 0;
    mv_top_right[1] = 0;
    mv_left[0] = 0;
    mv_left[1] = 0;

    // Top
    if (__algorithm[workerID].auxFlatPCM_Node.yInMacroblock == 0) {
        if (__algorithm[workerID].auxFlatPCM_Node.yInPicture > __algorithm[workerID].partition_y) {
            __getMotionVector(&mv,
                    __algorithm[workerID].auxFlatPCM_Node.xInPicture - __algorithm[workerID].partition_x,
                    __algorithm[workerID].auxFlatPCM_Node.yInPicture - MACROBLOCK_SIZE - __algorithm[workerID].partition_y,
                    __algorithm[workerID].auxFlatPCM_Node.xInMacroblock,
                    MACROBLOCK_SIZE - __algorithm[workerID].block_height,
                    workerID);
            mv_top[0] = mv.mv_x;
            mv_top[1] = mv.mv_y;
        }
    } else {
        __getMotionVector(&mv,
                __algorithm[workerID].auxFlatPCM_Node.xInPicture - __algorithm[workerID].partition_x,
                __algorithm[workerID].auxFlatPCM_Node.yInPicture - __algorithm[workerID].partition_y,
                __algorithm[workerID].auxFlatPCM_Node.xInMacroblock,
                __algorithm[workerID].auxFlatPCM_Node.yInMacroblock - __algorithm[workerID].block_height,
                workerID);
        mv_top[0] = mv.mv_x;
        mv_top[1] = mv.mv_y;
    }

    // Top-Right
    if (__algorithm[workerID].auxFlatPCM_Node.yInMacroblock == 0) {
        if (__algorithm[workerID].auxFlatPCM_Node.yInPicture > __algorithm[workerID].partition_y) {
            if (__algorithm[workerID].auxFlatPCM_Node.xInMacroblock + __algorithm[workerID].block_width == MACROBLOCK_SIZE) {
                if (__algorithm[workerID].auxFlatPCM_Node.xInPicture + MACROBLOCK_SIZE < (__algorithm[workerID].partition_x + (int) __algorithm[workerID].partition_width)) {
                    __getMotionVector(&mv,
                            __algorithm[workerID].auxFlatPCM_Node.xInPicture + MACROBLOCK_SIZE - __algorithm[workerID].partition_x,
                            __algorithm[workerID].auxFlatPCM_Node.yInPicture - MACROBLOCK_SIZE - __algorithm[workerID].partition_y,
                            0,
                            MACROBLOCK_SIZE - __algorithm[workerID].block_height,
                            workerID);
                    mv_top_right[0] = mv.mv_x;
                    mv_top_right[1] = mv.mv_y;
                }
            } else {
                __getMotionVector(&mv,
                        __algorithm[workerID].auxFlatPCM_Node.xInPicture - __algorithm[workerID].partition_x,
                        __algorithm[workerID].auxFlatPCM_Node.yInPicture - MACROBLOCK_SIZE - __algorithm[workerID].partition_y,
                        __algorithm[workerID].auxFlatPCM_Node.xInMacroblock + __algorithm[workerID].block_width,
                        MACROBLOCK_SIZE - __algorithm[workerID].block_height,
                        workerID);
                mv_top_right[0] = mv.mv_x;
                mv_top_right[1] = mv.mv_y;
            }
        }
    } else {
        if (__algorithm[workerID].auxFlatPCM_Node.xInMacroblock + __algorithm[workerID].block_width < MACROBLOCK_SIZE) {
            __getMotionVector(&mv,
                    __algorithm[workerID].auxFlatPCM_Node.xInPicture - __algorithm[workerID].partition_x,
                    __algorithm[workerID].auxFlatPCM_Node.yInPicture - __algorithm[workerID].partition_y,
                    __algorithm[workerID].auxFlatPCM_Node.xInMacroblock + __algorithm[workerID].block_width,
                    __algorithm[workerID].auxFlatPCM_Node.yInMacroblock - __algorithm[workerID].block_height,
                    workerID);
            mv_top_right[0] = mv.mv_x;
            mv_top_right[1] = mv.mv_y;
        }
    }

    // Left
    if (__algorithm[workerID].auxFlatPCM_Node.xInMacroblock == 0) {
        if (__algorithm[workerID].auxFlatPCM_Node.xInPicture > __algorithm[workerID].partition_x) {
            __getMotionVector(&mv,
                    __algorithm[workerID].auxFlatPCM_Node.xInPicture - MACROBLOCK_SIZE - __algorithm[workerID].partition_x,
                    __algorithm[workerID].auxFlatPCM_Node.yInPicture - __algorithm[workerID].partition_y,
                    MACROBLOCK_SIZE - __algorithm[workerID].block_width,
                    __algorithm[workerID].auxFlatPCM_Node.yInMacroblock,
                    workerID);
            mv_left[0] = mv.mv_x;
            mv_left[1] = mv.mv_y;
        }
    } else {
        __getMotionVector(&mv,
                __algorithm[workerID].auxFlatPCM_Node.xInPicture - __algorithm[workerID].partition_x,
                __algorithm[workerID].auxFlatPCM_Node.yInPicture - __algorithm[workerID].partition_y,
                __algorithm[workerID].auxFlatPCM_Node.xInMacroblock - __algorithm[workerID].block_width,
                __algorithm[workerID].auxFlatPCM_Node.yInMacroblock,
                workerID);
        mv_left[0] = mv.mv_x;
        mv_left[1] = mv.mv_y;
    }

    pred[0] = mv_top[0] + mv_top_right[0] + mv_left[0] - minfs(mv_top[0], minfs(mv_top_right[0], mv_left[0])) - maxfs(mv_top[0], maxfs(mv_top_right[0], mv_left[0]));
    pred[1] = mv_top[1] + mv_top_right[1] + mv_left[1] - minfs(mv_top[1], minfs(mv_top_right[1], mv_left[1])) - maxfs(mv_top[1], maxfs(mv_top_right[1], mv_left[1]));
}


static inline int SAD(MEC_Picture* currentPicture,
        MEC_Picture* referencePicture,
        int blocksize_x, int blocksize_y,
        int center_x, int center_y,
        int candidate_x, int candidate_y,
        int min_cost,
        unsigned int workerID) {
    MEC_Sample currentSample;
    MEC_Sample referenceSample;
    int mcost = 0;
    int x, y;
    for (y = 0; y < blocksize_y; y++) {
        for (x = 0; x < blocksize_x; x++) {
            currentSample = currentPicture->sample(
            		y + center_y,
            		x + center_x);

            referenceSample = referencePicture->sample(
            		y + candidate_y,
            		x + candidate_x);


            int absDiff = currentSample - referenceSample < 0? referenceSample - currentSample : currentSample - referenceSample;
            mcost += absDiff;

#if 0 /* Early termination mechanism */
            if (mcost >= min_cost) {
                return mcost;
            }
#endif
        }
    }

    return mcost;
}

static inline void __computeMVsAndCost(MEC_Picture * currentPicture,
        MEC_Picture * referencePicture,
        unsigned int workerID,
        MEC_SearchWindow * searchWindow,
        BlockModeType h264BlockMode,
        int pred_x, int pred_y)
{
    int candidate_x = 0, candidate_y = 0;
    int min_cost = INT_MAX;
    int center_x = __algorithm[workerID].auxFlatPCM_Node.xInMacroblock + __algorithm[workerID].auxFlatPCM_Node.xInPicture;
    int center_y = __algorithm[workerID].auxFlatPCM_Node.yInMacroblock + __algorithm[workerID].auxFlatPCM_Node.yInPicture;
    int x = 0, y = 0, block_cost;
    int _x, _y;

    for (candidate_y = center_y - searchWindow->searchRange;
            (int) candidate_y <= (int) (center_y + searchWindow->searchRange - __algorithm[workerID].block_height + 1);
            candidate_y++)
    {
        for (candidate_x = center_x - searchWindow->searchRange;
                (int) candidate_x <= (int) (center_x + searchWindow->searchRange - __algorithm[workerID].block_width + 1);
                candidate_x++)
        {

            if ((candidate_x >= __algorithm[workerID].partition_x)
                    && (candidate_y >= __algorithm[workerID].partition_y)
                    && (candidate_x <= __algorithm[workerID].partition_x + (int) __algorithm[workerID].partition_width - (int) __algorithm[workerID].block_width)
                    && (candidate_y <= __algorithm[workerID].partition_y + (int) __algorithm[workerID].partition_height - (int) __algorithm[workerID].block_height)) {


            	block_cost = SAD(currentPicture,
                        referencePicture,
                        __algorithm[workerID].block_width, __algorithm[workerID].block_height,
                        center_x, center_y,
                        candidate_x, candidate_y,
                        min_cost,
                        workerID);

                if (min_cost > block_cost) {
                    min_cost = block_cost;
                    x = candidate_x;
                    y = candidate_y;
                }
                else {
#if __REFERENCE_FULL_SEARCH_BMA_V01_DUMP
					d_dumpS_SI2("ts_compute_mvs_and_cost.log",
							__occurenceComputeMVsAndCost, __maxOccurrencesComputeMVsAndCostToBeRecorded,
							"if#2:false,min_cos remains the same",
							"min_cos", min_cost,
							"block_cost", block_cost);
#endif
				}
            }
            else {
#if __REFERENCE_FULL_SEARCH_BMA_V01_DUMP
				d_dumpS("ts_compute_mvs_and_cost.log", __occurenceComputeMVsAndCost, __maxOccurrencesComputeMVsAndCostToBeRecorded, "if#1:false");
#endif
            }
        }
    }

    __algorithm[workerID].auxFlatPCM_Node.mv_x = x - center_x;
    __algorithm[workerID].auxFlatPCM_Node.mv_y = y - center_y;
    __algorithm[workerID].auxFlatPCM_Node.cost = min_cost;
    __algorithm[workerID].auxFlatPCM_Node.pred_mv_x = pred_x;
    __algorithm[workerID].auxFlatPCM_Node.pred_mv_y = pred_y;

    int xInPartition = __algorithm[workerID].auxFlatPCM_Node.xInPicture - __algorithm[workerID].partition_x;
    int yInPartition = __algorithm[workerID].auxFlatPCM_Node.yInPicture - __algorithm[workerID].partition_y;
    int xInMacroblock = __algorithm[workerID].auxFlatPCM_Node.xInMacroblock;
    int yInMacroblock = __algorithm[workerID].auxFlatPCM_Node.yInMacroblock;
    _x = (xInPartition >> 2) + (xInMacroblock >> 2);
    _y = (yInPartition >> 2) + (yInMacroblock >> 2);

    __algorithm[workerID].mvs[_x][_y].mv_x = __algorithm[workerID].auxFlatPCM_Node.mv_x;
    __algorithm[workerID].mvs[_x][_y].mv_y = __algorithm[workerID].auxFlatPCM_Node.mv_y;

    FlatH264PMC_Node * dest = &__algorithm[workerID].out_flatPMC[__algorithm[workerID].indexFlatPMC];
    FlatH264PMC_Node * source =  &__algorithm[workerID].auxFlatPCM_Node;
    //memcpy(dest, source, sizeof(FlatH264PMC_Node));
    *dest = *source;

#if 0
    if (! flatH264PMC_Node_equals(source, dest)) {
    	//printf("source\n");
    	flatH264PMC_Node_print(source);
    	//printf("dest\n");
    	flatH264PMC_Node_print(dest);
    }
#endif
    Assert(flatH264PMC_Node_equals(source, dest), "", "");

    __algorithm[workerID].indexFlatPMC ++;

    __algorithm[workerID].out_flatPMC_Size++;


#if __REFERENCE_FULL_SEARCH_BMA_V01_DUMP
    d_dump_computeMVsAndCost_testSupportVersion("ts_compute_mvs_and_cost.log",
    		__occurenceComputeMVsAndCost, __maxOccurrencesComputeMVsAndCostToBeRecorded,
    		currentPicture,
    		referencePicture,
    		searchWindow,
    		h264BlockMode,
    		pred_x,
    		pred_y,
    		&__algorithm[workerID].auxFlatPCM_Node);
    __occurenceComputeMVsAndCost++;
#endif
}


static void __matchModeNxM(BlockModeType h264BlockMode,
        unsigned int * xsInMacroblock,
        unsigned int * ysInMacroblock,
        unsigned int numberOfBlocks,
        MEC_Picture * currentPicture,
        MEC_Picture * referencePicture,
        unsigned int workerID,
        MEC_SearchWindow * searchWindow)
{
    unsigned int i = 0;
    int pred[2];

    __algorithm[workerID].auxFlatPCM_Node.h264BlockMode = h264BlockMode;

    for (i = 0; i < numberOfBlocks; ++ i) {
        __algorithm[workerID].auxFlatPCM_Node.xInMacroblock = xsInMacroblock[i];
        __algorithm[workerID].auxFlatPCM_Node.yInMacroblock = ysInMacroblock[i];

        predicted_motion_vector(pred, workerID);
        __computeMVsAndCost(currentPicture,
                referencePicture,
                workerID,
                searchWindow,
                h264BlockMode,
                pred[0], pred[1]);

    }
}

static void __matchModes(MEC_Picture * currentPicture,
        MEC_Picture * referencePicture,
        unsigned int workerID,
        MEC_SearchWindow * searchWindow)
{
    unsigned int x4x4[16] = {0, 4, 8, 12, 0, 4, 8, 12, 0, 4, 8, 12, 0, 4, 8, 12};
    unsigned int y4x4[16] = {0, 0, 0, 0, 4, 4, 4, 4, 8, 8, 8, 8, 12, 12, 12, 12};
    __algorithm[workerID].block_width = 4;
    __algorithm[workerID].block_height = 4;
    __matchModeNxM(MODE4x4, x4x4, y4x4, 16,
            currentPicture,
            referencePicture,
            workerID,
            searchWindow);

    unsigned int x4x8[8] = {0, 4, 8, 12, 0, 4, 8, 12};
    unsigned int y4x8[8] = {0, 0, 0, 0, 8, 8, 8, 8};
    __algorithm[workerID].block_width = 4;
    __algorithm[workerID].block_height = 8;
    __matchModeNxM(MODE4x8, x4x8, y4x8, 8,
            currentPicture,
            referencePicture,
            workerID,
            searchWindow);

    unsigned int x8x4[8] = {0, 8, 0, 8, 0, 8, 0, 8};
    unsigned int y8x4[8] = {0, 0, 4, 4, 8, 8, 12, 12};
    __algorithm[workerID].block_width = 8;
    __algorithm[workerID].block_height = 4;
    __matchModeNxM(MODE8x4, x8x4, y8x4, 8,
            currentPicture,
            referencePicture,
            workerID,
            searchWindow);

    unsigned int x8x8[4] = {0, 8, 0, 8};
    unsigned int y8x8[4] = {0, 0, 8, 8};
    __algorithm[workerID].block_width = 8;
    __algorithm[workerID].block_height = 8;
    __matchModeNxM(MODE8x8, x8x8, y8x8, 4,
            currentPicture,
            referencePicture,
            workerID,
            searchWindow);

    unsigned int x8x16[2] = {0, 8};
    unsigned int y8x16[2] = {0, 0};
    __algorithm[workerID].block_width = 8;
    __algorithm[workerID].block_height = 16;
    __matchModeNxM(MODE8x16, x8x16, y8x16, 2,
            currentPicture,
            referencePicture,
            workerID,
            searchWindow);

    unsigned int x16x8[2] = {0, 0};
    unsigned int y16x8[2] = {0, 8};
    __algorithm[workerID].block_width = 16;
    __algorithm[workerID].block_height = 8;
    __matchModeNxM(MODE16x8, x16x8, y16x8, 2,
            currentPicture,
            referencePicture,
            workerID,
            searchWindow);

    unsigned int xSkip[1] = {0};
    unsigned int ySkip[1] = {0};
    __algorithm[workerID].block_width = 16;
    __algorithm[workerID].block_height = 16;
    __matchModeNxM(MODE16x16, xSkip, ySkip, 1,
            currentPicture,
            referencePicture,
            workerID,
            searchWindow);

}

static void __matchReferencePictureMotionCounterpartForListN(
        MEC_Picture * currentPicture,
        unsigned int listNumber,
        MEC_Picture ** list, unsigned int listSize,
        unsigned int workerID,
        MEC_SearchWindow * searchWindow)
{
    unsigned int i;

    __algorithm[workerID].auxFlatPCM_Node.referencePictureListNumber = listNumber;

    for (i = 0; i < listSize; ++ i) {

        __algorithm[workerID].auxFlatPCM_Node.referencePictureID = (int) i;

        __matchModes(currentPicture,
                list[i],
                workerID,
                searchWindow);
    }
}

static void __matchLists(MEC_Picture * currentPicture,
        int xInPicture, int yInPicture,
        int id,
        MEC_Picture ** list0, unsigned int list0_Size,
        MEC_Picture ** list1, unsigned int list1_Size,
        int usingN_ReferenceLists,
        unsigned int workerID,
        MEC_SearchWindow * searchWindow)
{
    __algorithm[workerID].auxFlatPCM_Node.xInPicture = xInPicture;
    __algorithm[workerID].auxFlatPCM_Node.yInPicture = yInPicture;
    __algorithm[workerID].auxFlatPCM_Node.macroblockID = id;

    __matchReferencePictureMotionCounterpartForListN(
            currentPicture,
            0,
            list0, list0_Size,
            workerID,
            searchWindow);
    __matchReferencePictureMotionCounterpartForListN(
            currentPicture,
            1,
            list1, list1_Size,
            workerID,
            searchWindow);
}

// ----------------------------------------------------------------------------

int static ___computeMacroblockID(int xInPicture, int yInPicture, unsigned int pictureWidth, unsigned int pictureHeight) {
    unsigned int MBH = pictureWidth / MACROBLOCK_SIZE;

    int id = (xInPicture / MACROBLOCK_SIZE) +
        ((((MBH * yInPicture) / MACROBLOCK_SIZE)/MBH) * MBH);

    return id;
}


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
        unsigned int picturePartitionHeight)
{
#if ENABLE_CHRONOMETER && ENABLE_CHRONO_MATCH_ALGORITHM_TIME
    chrono_start(&chrono_match_algorithm_time);
#endif

    int usingN_ReferenceLists = 1;

    __algorithm[workerID].partition_x = picturePartitionX;
    __algorithm[workerID].partition_y = picturePartitionY;
    __algorithm[workerID].partition_width = picturePartitionWidth;
    __algorithm[workerID].partition_height = picturePartitionHeight;
    __algorithm[workerID].out_flatPMC = out_flatPMC;
    __algorithm[workerID].out_flatPMC_Size = 0;
    __algorithm[workerID].indexFlatPMC = 0;

    unsigned int numberOfMacroblocksInHorizontal = picturePartitionWidth / MACROBLOCK_SIZE;
    unsigned int numberOfMacroblocksInVertical = picturePartitionHeight /  MACROBLOCK_SIZE;

    // __matchMacroblock - begin

    unsigned int i, j, k;

    for (i = 0; i < numberOfMacroblocksInVertical; ++ i) {
        for (j = 0; j < numberOfMacroblocksInHorizontal; ++ j) {
            int xInPicture, yInPicture;

            xInPicture = j * MACROBLOCK_SIZE + picturePartitionX;
            if (j * MACROBLOCK_SIZE >= picturePartitionWidth) {
                xInPicture = picturePartitionX;
            }

            yInPicture = i * MACROBLOCK_SIZE + picturePartitionY;

            if (i * MACROBLOCK_SIZE >= picturePartitionHeight) {
                Abort();
            }

            k = ___computeMacroblockID(xInPicture, yInPicture, totalPictureWidth, totalPictureHeight);

#if 1 /* Trace */
            cout << "ReferenceBMA processing macroblock #" << k << "\n";
#endif

            __matchLists(currentPicture,
                    xInPicture, yInPicture,
                    k,
                    list0, list0_Size,
                    0, 0,
                    usingN_ReferenceLists,
                    workerID,
                    searchWindow);

        }
    }

    // __matchMacroblock - end

    out_flatPMC_Size[0] = __algorithm[workerID].out_flatPMC_Size;

#if ENABLE_CHRONOMETER && ENABLE_CHRONO_MATCH_ALGORITHM_TIME
    chrono_pause(&chrono_match_algorithm_time);
#endif

}
