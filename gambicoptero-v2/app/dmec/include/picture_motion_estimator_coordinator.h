
/*!
 ******************************************************************************
 * \file
 *
 * \brief
 *    -
 *
 * \author
 *      - Alexandre Massayuki Okazaki   (alexandre@lisha.ufsc.br)
 *      - Mateus Krepsky Ludwich        (mateus@lisha.ufsc.br)
 *      - Tiago de Albuquerque Reis     (reis@lisha.ufsc.br)
 *
 ******************************************************************************
 */

#ifndef PICTURE_MOTION_ESTIMATOR_COORDINATOR_H_
#define PICTURE_MOTION_ESTIMATOR_COORDINATOR_H_

#include "block_mode.h"
#include "vectorial_pmc.h"
#include "mec_picture.h"
#include "dmec_picture.h"
#include "search_metric.h"
#include "reference_picture_motion_counterpart.h"

#include "traits.h"
#include "picture_motion_estimator_worker_proxy.h"

/// << singleton >>
class PictureMotionEstimator {

public:
    // General methods
   /* For performance reasons, this method does not creates a
    * PictureMotionCounterpart (PMC) each time it is invoked.
    * Instead of that, it returns a PMC which is a attribute of
    * PictureMotionEstimator.
    * Consequently you should not delete this method's return.
    * It will be properly deleted on the deletion of PictureMotionEstimator.
    * */
    PictureMotionCounterpart* match(MEC_Picture* currentPicture, MEC_Picture* referencePicture);


public: 
    // Singleton access
    static PictureMotionEstimator * getInstance(unsigned int pictureWidth,
    		unsigned int pictureHeight,
    		unsigned int max_reference_pictures);


    static void deleteInstance();


private:
    // Since this class is a singleton, constructors are private.
    PictureMotionEstimator();

    ~PictureMotionEstimator();

    PictureMotionEstimator(unsigned int pictureWidth,
    		unsigned int pictureHeight,
    		unsigned int max_reference_pictures);


private:
    // Singleton
    static PictureMotionEstimator * __instance;


private:
    // Instance attributes
    unsigned int __pictureWidth;
    unsigned int __pictureHeight;

    BlockMode ** __modesAtLevelZero;
    unsigned int __numberOfModesAtLevelZero;
    PictureMotionCounterpart __pmc;

    PictureMotionEstimatorWorkerProxy __workers[NUM_WORKERS];


private:
    // Private methods
    unsigned int __computeTotalFlatPMC_Size(unsigned int list0ReferencePictureSize,
    		unsigned int list1ReferencePictureSize);


    PictureMotionCounterpart * __commonMatch(MEC_Picture * currentPicture,
    		MEC_Picture ** list0,
    		unsigned int list0Size,
    		MEC_Picture ** list1,
    		unsigned int list1Size);


    void __fillPMC_WithFlatPMC(PictureMotionCounterpart * pmc,
    		FlatH264PMC_Node * flatPMC,
    		unsigned int flatPMC_Size,
    		int workerID);


    void pp_copyPartitioning(MEC_Picture * picture,
    		DMEC_Picture * partitions,
    		unsigned int numPartitions);

};


#endif /* PICTURE_MOTION_ESTIMATOR_COORDINATOR_H_ */

