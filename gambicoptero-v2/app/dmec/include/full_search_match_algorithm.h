/*

 Mateus Krepsky Ludwich.
*/

#ifndef FULL_SEARCH_MATCH_ALGORITHM_H_
#define FULL_SEARCH_MATCH_ALGORITHM_H_

#include "match_algorithm.h"

class FullSearchMatchAlgorithm : public MatchAlgorithm
{

public:
    void match_algorithm_biPredictionMatch(unsigned int workerID,
            LazyPicture * currentPicture,
            LazyPicture list0[], unsigned int list0_Size,
            LazyPicture list1[], unsigned int list1_Size,
            int picturePartitionX, // x in Picture
            int picturePartitionY, // y in Picture
            unsigned int picturePartitionWidth,
            unsigned int picturePartitionHeight,
            unsigned int totalPictureWidth,
            unsigned int totalPictureHeight,
            MEC_SearchWindow * searchWindow,
            int lambda_factor,
            LazyFlatH264PMC * flatPMC);


    virtual ~FullSearchMatchAlgorithm();


private:
    // Private methods
    inline int ___computeMacroblockID(int xInPicture, int yInPicture, unsigned int pictureWidth, unsigned int pictureHeight);

protected:
    inline void predicted_motion_vector(int pred[],
            unsigned int workerID,
            unsigned int picture_width);

    virtual int SAD(LazyPicture* currentPicture,
            LazyPicture* referencePicture,
            int blocksize_x, int blocksize_y,
            int center_x, int center_y,
            int candidate_x, int candidate_y,
            int min_cost,
            unsigned int workerID);

    inline void __computeMVsAndCost(LazyPicture * currentPicture,
            LazyPicture * referencePicture,
            unsigned int workerID,
            MEC_SearchWindow * searchWindow,
            BlockModeType h264BlockMode,
            int pred_x, int pred_y);

    void __matchModeNxM(BlockModeType h264BlockMode,
            unsigned int * xsInMacroblock,
            unsigned int * ysInMacroblock,
            unsigned int numberOfBlocks,
            LazyPicture * currentPicture,
            LazyPicture * referencePicture,
            unsigned int workerID,
            MEC_SearchWindow * searchWindow);

    void __matchModes(LazyPicture * currentPicture,
            LazyPicture * referencePicture,
            unsigned int workerID,
            MEC_SearchWindow * searchWindow);

    void __matchReferencePictureMotionCounterpartForListN(
            LazyPicture * currentPicture,
            unsigned int listNumber,
            LazyPicture list[], unsigned int listSize,
            unsigned int workerID,
            MEC_SearchWindow * searchWindow);

    void __matchLists(LazyPicture * currentPicture,
            int xInPicture, int yInPicture,
            int id,
            LazyPicture list0[], unsigned int list0_Size,
            LazyPicture list1[], unsigned int list1_Size,
            int usingN_ReferenceLists,
            unsigned int workerID,
            MEC_SearchWindow * searchWindow);

private:
    // Attributes
    FlatH264PMC_Node auxFlatPCM_Node;
    int block_width;
    int block_height;

    LazyFlatH264PMC * PMC;

    int __partition_x;
    int __partition_y;

};

#endif /* FULL_SEARCH_MATCH_ALGORITHM_H_ */
