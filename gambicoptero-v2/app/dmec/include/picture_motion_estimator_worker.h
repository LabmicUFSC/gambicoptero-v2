
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

#ifndef PICTURE_MOTION_ESTIMATOR_WORKER_H_
#define PICTURE_MOTION_ESTIMATOR_WORKER_H_

#include "pme_worker_internals.h"

#include "traits.h"

/* Works for single-thread Workers, with double buffer support.
 * It uses asynchronous DMA in Cell and synchronous memory copy in PC
 *
 * Essa função aqui não tem nada a ver com double buffer ou não!  */
void pictureMotionEstimatorWorker_run_db_sth(PictureMotionEstimatorWorker * worker);


#endif

