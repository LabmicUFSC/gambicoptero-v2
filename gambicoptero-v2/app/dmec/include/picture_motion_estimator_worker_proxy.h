
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

#ifndef PICTURE_MOTION_ESTIMATOR_WORKER_PROXY_H_
#define PICTURE_MOTION_ESTIMATOR_WORKER_PROXY_H_

#include "traits.h"
#include "worker_parameters.h"
#include "mec_picture.h"

typedef enum {
    WORKER_PROXY00 = 0,
    WORKER_PROXY01 = 1,
    WORKER_PROXY02 = 2,
    WORKER_PROXY03 = 3,
    WORKER_PROXY04 = 4,
    WORKER_PROXY05 = 5
} PictureMotionEstimatorWorkerProxy;


void setUp_PictureMotionEstimatorWorkerProxy(PictureMotionEstimatorWorkerProxy workerProxy,
		int picturePartitionX, int picturePartitionY,
		unsigned int picturePartitionWidth, unsigned int picturePartitionHeight,
		int picturePartitionFirstMacroblockID,
		unsigned long ea_info_dmec_currentPicturePartition,
		unsigned long * eas_info_dmec_list0Partition, unsigned int list0MaxSize,
		unsigned long * eas_info_dmec_list1Partition, unsigned int list1MaxSize,
		unsigned long ea_flatPMC);


void tearDown_PictureMotionEstimatorWorkerProxy(PictureMotionEstimatorWorkerProxy _this);

/* pictureMotionEstimatorWorkerProxy_setWorkerToPerformMatch
 *
 * Copies reference picture lists's sizes and lambda_factor to worker
 * parameters, computes size of flat PMC and copy it to worker parameters to.
 *
 * Returns size of flat PMC
 *
 */
unsigned int pictureMotionEstimatorWorkerProxy_setWorkerToPerformMatch(PictureMotionEstimatorWorkerProxy _this,
		unsigned int eas_dmec_list0Size,
	    unsigned int eas_dmec_list1Size,
	    int lambda_factor
	);


void pictureMotionEstimatorWorkerProxy_waitWorkersAlive();


void pictureMotionEstimatorWorkerProxy_signalMatchStart();


void pictureMotionEstimatorWorkerProxy_signalCoordinatorDataReady();


void pictureMotionEstimatorWorkerProxy_waitWorkerResultsDone();


#endif /* PICTURE_MOTION_ESTIMATOR_WORKER_PROXY_H_ */

