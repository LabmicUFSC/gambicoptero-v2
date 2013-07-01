
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
 *      - Tiago de Albuquerque Reis      (reis@lisha.ufsc.br)
 *
 ******************************************************************************
 */

#ifndef WORKER_PARAMETERS_H_
#define WORKER_PARAMETERS_H_

#include "traits.h"

/* A instance of WorkerParameters is passed as parameter
   to spe_context_run function.
   WorkerParameters must be multiple of 16 bytes.
 */
struct __WorkerParameters {
	/* Worker ID *********************************************************** */
	unsigned long long workerID;

	/* --------------------------------------------------------------------- */


    /* Picture partition definition **************************************** */
    int                 picturePartitionX;
    int                 picturePartitionY;
    unsigned int        picturePartitionWidth;
    unsigned int        picturePartitionHeight;
    unsigned int        totalPictureWidth;
    unsigned int        totalPictureHeight;

    /* --------------------------------------------------------------------- */


    /* Addresses for Match inputs ******************************************* */
    unsigned long long ea_info_dmec_currentPicturePartition;

    unsigned long long eas_info_dmec_list0Partition[MAX_REFERENCE_PICTURE_LIST0_SIZE];
    unsigned int       list0Size;

    unsigned long long eas_info_dmec_list1Partition[MAX_REFERENCE_PICTURE_LIST1_SIZE];
    unsigned int       list1Size;

    int lambda_factor;

    /* --------------------------------------------------------------------- */


    /* Addresses for Match outputs ****************************************** */
    unsigned long long  ea_flatPMC;
    unsigned int        flatPMCSize;

    /* --------------------------------------------------------------------- */

} __attribute__((aligned(ALIGNMENT)));

typedef struct __WorkerParameters WorkerParameters;

#endif /* WORKER_PARAMETERS_H_ */

