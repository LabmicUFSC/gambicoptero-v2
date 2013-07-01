/*
 * dmec_full_search_match_algorithm.c
 *
 * - No Rate
 * - Raster Order
 *
 *  Created on: Jun 16, 2011
 *      Author: mateus
 */

#include "../include/block_mode_type.h"
#include "../include/traits.h"
#include "../include/lazy_flat_h264_pmc.h"
#include "../include/match_algorithm.h"
#include "../include/full_search_match_algorithm.h"
#include "../include/abst_assert.h"
#include "../include/error.h"
#include "../include/master_tracedefs.h"


#if LINUX
#include <iostream>
#include <stdlib.h>
#include <limits.h>

using namespace std;
#else
#include <utility/malloc.h>
#include <utility/ostream.h>
typedef System::OStream OStream;
extern OStream cout;

#endif


#if ENABLE_CHRONOMETER
#include <abst/chronometer.h>

#endif


#define __FULLSEARCH_MATCH_ALGORITHM_RV01_DUMP 0 && ENABLE_DUMP


#define __FULLSEARCH_MATCH_ALGORITHM_RV01_POSTCONDITIONS 1


#define LAMBDA_ACCURACY_BITS_CME 16

#define MACROBLOCK_SIZE 16
#define maxfs(a,b) (((a) > (b)) ? (a) : (b))
#define minfs(a,b) (((a) < (b)) ? (a) : (b))


#define abs(a) ((a < 0) ? (-a) : (a))



// "private" functions --------------------------------------------------------
void FullSearchMatchAlgorithm::predicted_motion_vector(int pred[],
        unsigned int workerID,
        unsigned int picture_width)
{
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
    if (auxFlatPCM_Node.yInMacroblock == 0) {
        if (auxFlatPCM_Node.yInPicture > 0) {
            pmc_getMotionVector(PMC,
                    &mv,
                    auxFlatPCM_Node.xInPicture,
                    auxFlatPCM_Node.yInPicture - MACROBLOCK_SIZE,
                    auxFlatPCM_Node.xInMacroblock,
                    MACROBLOCK_SIZE - block_height);
            mv_top[0] = mv.mv_x;
            mv_top[1] = mv.mv_y;
        }
    } else {
        pmc_getMotionVector(PMC,
                &mv,
                auxFlatPCM_Node.xInPicture,
                auxFlatPCM_Node.yInPicture,
                auxFlatPCM_Node.xInMacroblock,
                auxFlatPCM_Node.yInMacroblock - block_height);
        mv_top[0] = mv.mv_x;
        mv_top[1] = mv.mv_y;
    }

    // Top-Right
    if (auxFlatPCM_Node.yInMacroblock == 0) {
        if (auxFlatPCM_Node.yInPicture > 0) {
            if (auxFlatPCM_Node.xInMacroblock + block_width == MACROBLOCK_SIZE) {
                if (auxFlatPCM_Node.xInPicture + MACROBLOCK_SIZE < ((signed int) picture_width)) {
                    pmc_getMotionVector(PMC,
                            &mv,
                            auxFlatPCM_Node.xInPicture + MACROBLOCK_SIZE,
                            auxFlatPCM_Node.yInPicture - MACROBLOCK_SIZE,
                            0,
                            MACROBLOCK_SIZE - block_height);
                    mv_top_right[0] = mv.mv_x;
                    mv_top_right[1] = mv.mv_y;
                }
            } else {
                pmc_getMotionVector(PMC,
                        &mv,
                        auxFlatPCM_Node.xInPicture,
                        auxFlatPCM_Node.yInPicture - MACROBLOCK_SIZE,
                        auxFlatPCM_Node.xInMacroblock + block_width,
                        MACROBLOCK_SIZE - block_height);
                mv_top_right[0] = mv.mv_x;
                mv_top_right[1] = mv.mv_y;
            }
        }
    } else {
        if (auxFlatPCM_Node.xInMacroblock + block_width < MACROBLOCK_SIZE) {
            pmc_getMotionVector(PMC,
                    &mv,
                    auxFlatPCM_Node.xInPicture,
                    auxFlatPCM_Node.yInPicture,
                    auxFlatPCM_Node.xInMacroblock + block_width,
                    auxFlatPCM_Node.yInMacroblock - block_height);
            mv_top_right[0] = mv.mv_x;
            mv_top_right[1] = mv.mv_y;
        }
    }

    // Left
    if (auxFlatPCM_Node.xInMacroblock == 0) {
        if (auxFlatPCM_Node.xInPicture > 0) {
            pmc_getMotionVector(PMC,
                    &mv,
                    auxFlatPCM_Node.xInPicture - MACROBLOCK_SIZE,
                    auxFlatPCM_Node.yInPicture,
                    MACROBLOCK_SIZE - block_width,
                    auxFlatPCM_Node.yInMacroblock);
            mv_left[0] = mv.mv_x;
            mv_left[1] = mv.mv_y;
        }
    } else {
        pmc_getMotionVector(PMC,
                &mv,
                auxFlatPCM_Node.xInPicture,
                auxFlatPCM_Node.yInPicture,
                auxFlatPCM_Node.xInMacroblock - block_width,
                auxFlatPCM_Node.yInMacroblock);
        mv_left[0] = mv.mv_x;
        mv_left[1] = mv.mv_y;
    }

    pred[0] = mv_top[0] + mv_top_right[0] + mv_left[0] - minfs(mv_top[0], minfs(mv_top_right[0], mv_left[0])) - maxfs(mv_top[0], maxfs(mv_top_right[0], mv_left[0]));
    pred[1] = mv_top[1] + mv_top_right[1] + mv_left[1] - minfs(mv_top[1], minfs(mv_top_right[1], mv_left[1])) - maxfs(mv_top[1], maxfs(mv_top_right[1], mv_left[1]));
}


int FullSearchMatchAlgorithm::SAD(LazyPicture* currentPicture,
        LazyPicture* referencePicture,
        int blocksize_x, int blocksize_y,
        int center_x, int center_y,
        int candidate_x, int candidate_y,
        int min_cost,
        unsigned int workerID)
{
    MEC_Sample currentSample;
    MEC_Sample referenceSample;
    int mcost = 0;
    int x, y;
    for (y = 0; y < blocksize_y; y++) {
        for (x = 0; x < blocksize_x; x++) {
            currentSample = lp_getSample(currentPicture,
                    y + center_y,
                    x + center_x);

            referenceSample = lp_getSample(referencePicture,
                    y + candidate_y,
                    x + candidate_x);

            mcost += abs((currentSample - referenceSample));

#if 0 /* Early termination mechanism */
            if (mcost >= min_cost) {
                return mcost;
            }
#endif

        }
    }


#if __FULLSEARCH_MATCH_ALGORITHM_RV01_POSTCONDITIONS
	Assert(mcost >= 0, "FullSearch", "SAD cannot be negative");
#endif

    return mcost;
}


/* Para cada match de picture esta função é chamada para cada bloco
 * (partição de macrobloco) da current picture, que terá seu movimento estimado
 * nas reference pictures.
 * Por exemplo, para uma picture 48x32 existem 6 macroblocos, que multiplicados
 * por 41 (o número total de blocos considerando-se todos os modos do H.264) é
 * igual a 246 blocos. Ou seja, esta função será chamada 246 em um match de
 * picture.
 *
 * Como o bloco muda a cada vez que esta função é chamada, conseguentemente
 * também muda a posição da search window, uma vez que a search window se
 * desloca sobre a picture de referência a cada mudança de bloco na current
 * picture.
 *
 * */
void FullSearchMatchAlgorithm::__computeMVsAndCost(LazyPicture * currentPicture,
        LazyPicture * referencePicture,
        unsigned int workerID,
        MEC_SearchWindow * searchWindow,
        BlockModeType h264BlockMode,
        int pred_x, int pred_y)
{
    int candidate_x = 0;
    int candidate_y = 0;
    int min_cost = INT_MAX;
    /* center represents the topleft position of the block been processed */
    int center_x = auxFlatPCM_Node.xInMacroblock + auxFlatPCM_Node.xInPicture;
    int center_y = auxFlatPCM_Node.yInMacroblock + auxFlatPCM_Node.yInPicture;
    int x = 0;
    int y = 0;
    int block_cost;

    for (candidate_y = center_y - searchWindow->searchRange;
            (int) candidate_y <= (int) (center_y + searchWindow->searchRange - block_height + 1);
            candidate_y++)
    {

        for (candidate_x = center_x - searchWindow->searchRange;
                (int) candidate_x <= (int) (center_x + searchWindow->searchRange - block_width + 1);
                candidate_x++)
        {
            if ((candidate_x >= 0)
                    && (candidate_y >= 0)
                    && (candidate_x <= ((int) lp_getWidth(currentPicture)) - ((int) block_width))
                    && (candidate_y <= ((int) lp_getHeight(currentPicture)) - ((int) block_height)))
            {

            	block_cost = SAD(currentPicture,
                        referencePicture,
                        block_width, block_height,
                        center_x, center_y,
                        candidate_x, candidate_y,
                        min_cost,
                        workerID);

                if (min_cost > block_cost) {
                    min_cost = block_cost;
                    x = candidate_x;
                    y = candidate_y;
                }
            }
        }
    }

    auxFlatPCM_Node.mv_x = x - center_x;
    auxFlatPCM_Node.mv_y = y - center_y;
    auxFlatPCM_Node.cost = min_cost;
    auxFlatPCM_Node.pred_mv_x = pred_x;
    auxFlatPCM_Node.pred_mv_y = pred_y;

    pmc_putMotionVector(PMC,
            auxFlatPCM_Node.mv_x, auxFlatPCM_Node.mv_y,
            auxFlatPCM_Node.xInPicture,
            auxFlatPCM_Node.yInPicture,
            auxFlatPCM_Node.xInMacroblock,
            auxFlatPCM_Node.yInMacroblock);

    /* Until now auxFlatPCM_Node it is relative to picture partition (i.e. starts on 0).
     * As lfpmc_putFlatH264PMC_Node asks for whole picture coordinates it it necessary to correct x and y in picture. */
    auxFlatPCM_Node.xInPicture += __partition_x;
    auxFlatPCM_Node.yInPicture += __partition_y;
    lfpmc_putFlatH264PMC_Node(PMC, & auxFlatPCM_Node);


#if __FULLSEARCH_MATCH_ALGORITHM_RV01_POSTCONDITIONS
	Assert(auxFlatPCM_Node.cost >= 0, "FullSearch", "cost must be positive");
#endif
}


void FullSearchMatchAlgorithm::__matchModeNxM(BlockModeType h264BlockMode,
        unsigned int * xsInMacroblock,
        unsigned int * ysInMacroblock,
        unsigned int numberOfBlocks,
        LazyPicture * currentPicture,
        LazyPicture * referencePicture,
        unsigned int workerID,
        MEC_SearchWindow * searchWindow)
{
    unsigned int i = 0;
    int pred[2];

    auxFlatPCM_Node.h264BlockMode = h264BlockMode;

    for (i = 0; i < numberOfBlocks; ++ i) {
        auxFlatPCM_Node.xInMacroblock = xsInMacroblock[i];
        auxFlatPCM_Node.yInMacroblock = ysInMacroblock[i];

        predicted_motion_vector(pred, workerID, lp_getWidth(currentPicture));
        __computeMVsAndCost(currentPicture,
                referencePicture,
                workerID,
                searchWindow,
                h264BlockMode,
                pred[0], pred[1]);

    }
}


void FullSearchMatchAlgorithm::__matchModes(LazyPicture * currentPicture,
        LazyPicture * referencePicture,
        unsigned int workerID,
        MEC_SearchWindow * searchWindow)
{
    unsigned int x4x4[16] = {0, 4, 8, 12, 0, 4, 8, 12, 0, 4, 8, 12, 0, 4, 8, 12};
    unsigned int y4x4[16] = {0, 0, 0, 0, 4, 4, 4, 4, 8, 8, 8, 8, 12, 12, 12, 12};
    block_width = 4;
    block_height = 4;
    __matchModeNxM(MODE4x4, x4x4, y4x4, 16,
            currentPicture,
            referencePicture,
            workerID,
            searchWindow);

    unsigned int x4x8[8] = {0, 4, 8, 12, 0, 4, 8, 12};
    unsigned int y4x8[8] = {0, 0, 0, 0, 8, 8, 8, 8};
    block_width = 4;
    block_height = 8;
    __matchModeNxM(MODE4x8, x4x8, y4x8, 8,
            currentPicture,
            referencePicture,
            workerID,
            searchWindow);

    unsigned int x8x4[8] = {0, 8, 0, 8, 0, 8, 0, 8};
    unsigned int y8x4[8] = {0, 0, 4, 4, 8, 8, 12, 12};
    block_width = 8;
    block_height = 4;
    __matchModeNxM(MODE8x4, x8x4, y8x4, 8,
            currentPicture,
            referencePicture,
            workerID,
            searchWindow);

    unsigned int x8x8[4] = {0, 8, 0, 8};
    unsigned int y8x8[4] = {0, 0, 8, 8};
    block_width = 8;
    block_height = 8;
    __matchModeNxM(MODE8x8, x8x8, y8x8, 4,
            currentPicture,
            referencePicture,
            workerID,
            searchWindow);

    unsigned int x8x16[2] = {0, 8};
    unsigned int y8x16[2] = {0, 0};
    block_width = 8;
    block_height = 16;
    __matchModeNxM(MODE8x16, x8x16, y8x16, 2,
            currentPicture,
            referencePicture,
            workerID,
            searchWindow);

    unsigned int x16x8[2] = {0, 0};
    unsigned int y16x8[2] = {0, 8};
    block_width = 16;
    block_height = 8;
    __matchModeNxM(MODE16x8, x16x8, y16x8, 2,
            currentPicture,
            referencePicture,
            workerID,
            searchWindow);

    unsigned int xSkip[1] = {0};
    unsigned int ySkip[1] = {0};
    block_width = 16;
    block_height = 16;
    __matchModeNxM(MODE16x16, xSkip, ySkip, 1,
            currentPicture,
            referencePicture,
            workerID,
            searchWindow);
}


void FullSearchMatchAlgorithm::__matchReferencePictureMotionCounterpartForListN(
        LazyPicture * currentPicture,
        unsigned int listNumber,
        LazyPicture list[], unsigned int listSize,
        unsigned int workerID,
        MEC_SearchWindow * searchWindow)
{
    unsigned int i;

    auxFlatPCM_Node.referencePictureListNumber = listNumber;

    for (i = 0; i < listSize; ++ i) {
        auxFlatPCM_Node.referencePictureID = (int) i;

        __matchModes(currentPicture,
                &list[i],
                workerID,
                searchWindow);
    }
}

void FullSearchMatchAlgorithm::__matchLists(LazyPicture * currentPicture,
        int xInPicture, int yInPicture,
        int id,
        LazyPicture list0[], unsigned int list0_Size,
        LazyPicture list1[], unsigned int list1_Size,
        int usingN_ReferenceLists,
        unsigned int workerID,
        MEC_SearchWindow * searchWindow)
{
    auxFlatPCM_Node.xInPicture = xInPicture;
    auxFlatPCM_Node.yInPicture = yInPicture;
    auxFlatPCM_Node.macroblockID = id;

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

int FullSearchMatchAlgorithm::___computeMacroblockID(int xInPicture, int yInPicture, unsigned int pictureWidth, unsigned int pictureHeight) {
    unsigned int MBH = pictureWidth / MACROBLOCK_SIZE;

    int id = (xInPicture / MACROBLOCK_SIZE) +
        ((((MBH * yInPicture) / MACROBLOCK_SIZE)/MBH) * MBH);

    return id;
}

/* Note:
 *  This algorithm should does not know about picture partition existence,
 *  although it operates over a picture partition (but this is done implicitly
 *  by using LazyPicture).
 *
 */
/*
 * Note:
 *
 * Probably the parameters: picturePartitionX, picturePartitionY,
 * picturePartitionWidth, and picturePartitionHeight
 * will be removed, because they are in the actual MEC_Picture structure.
 *
 * */
void FullSearchMatchAlgorithm::match_algorithm_biPredictionMatch(unsigned int workerID,
        LazyPicture * currentPicture,
        LazyPicture list0[], unsigned int list0_Size,
        LazyPicture list1[], unsigned int list1_Size,
        int _picturePartitionX, // x in Picture      /* Should be removed xxx */
        int _picturePartitionY, // y in Picture      /* Should be removed xxx */
        unsigned int _picturePartitionWidth,         /* Should be removed xxx */
        unsigned int _picturePartitionHeight,        /* Should be removed xxx */
        unsigned int _totalPictureWidth,             /* Should be removed xxx */
        unsigned int _totalPictureHeight,            /* Should be removed xxx */
        MEC_SearchWindow * searchWindow,
        int lambda_factor,
        LazyFlatH264PMC * flatPMC)
{
#if 1 /* Preconditions */
    unsigned int ref;
    for (ref = 0; ref < list0_Size; ref ++) {
        Assert(lp_getWidth(currentPicture) == lp_getWidth(&list0[ref]), "FullSearchMatchAlgorithmRV01::match_algorithm_biPredictionMatch", "cannot operate over pictures of different dimensions");
        Assert(lp_getHeight(currentPicture) == lp_getHeight(&list0[ref]), "FullSearchMatchAlgorithmRV01::match_algorithm_biPredictionMatch", "cannot operate over pictures of different dimensions");
    }

    for (ref = 0; ref < list1_Size; ref ++) {
      Assert(lp_getWidth(currentPicture) == lp_getWidth(&list1[ref]), "FullSearchMatchAlgorithmRV01::match_algorithm_biPredictionMatch", "cannot operate over pictures of different dimensions");
      Assert(lp_getHeight(currentPicture) == lp_getHeight(&list1[ref]), "FullSearchMatchAlgorithmRV01::match_algorithm_biPredictionMatch", "cannot operate over pictures of different dimensions");
    }

#endif


#if 0 // trace
    printf("FullSeachMatchAlgorithmRV01::match_algorithm_biPredictionMatch worker:%u\n", workerID);
#endif


    // __matchMacroblock - begin

    int usingN_ReferenceLists = 1;
    if (list1_Size > 0) {
        usingN_ReferenceLists = 2;
    }

    PMC = flatPMC;


    unsigned int numberOfMacroblocksInHorizontal = lp_getWidth(currentPicture) / MACROBLOCK_SIZE;
    unsigned int numberOfMacroblocksInVertical = lp_getHeight(currentPicture) /  MACROBLOCK_SIZE;

    unsigned int i, j, k;

    for (i = 0; i < numberOfMacroblocksInVertical; ++ i) {
        for (j = 0; j < numberOfMacroblocksInHorizontal; ++ j) {
            int xInPicture, yInPicture;

            xInPicture = j * MACROBLOCK_SIZE;
            if (j * MACROBLOCK_SIZE >= lp_getWidth(currentPicture)) {
                /* Back to the beginning of the row */
                xInPicture = 0;
            }

            yInPicture = i * MACROBLOCK_SIZE;

            if (i * MACROBLOCK_SIZE >= lp_getHeight(currentPicture)) {
                Abort();
            }

            /* The macroblock ID (k) takes into consideration the whole picture. */
            k = ___computeMacroblockID(xInPicture + _picturePartitionX, yInPicture + _picturePartitionY, _totalPictureWidth, _totalPictureHeight);
#if 1 /* Trace */
            cout << "processing macroblock #" << k << "\n";
            /*
            FILE * fp = fopen("fs_rv01_mb.data", "w");
            fprintf(fp, "processing macroblock #%i of 1360\n", k);
            fclose(fp);
            */
#endif

            __matchLists(currentPicture,
                    xInPicture, yInPicture,
                    k,
                    list0, list0_Size,
                    list1, list1_Size,
                    usingN_ReferenceLists,
                    workerID,
                    searchWindow);

        }
    }

    // __matchMacroblock - end


}


FullSearchMatchAlgorithm::~FullSearchMatchAlgorithm()
{}
