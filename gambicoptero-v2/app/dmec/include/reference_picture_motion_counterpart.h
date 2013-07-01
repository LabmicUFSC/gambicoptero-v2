
/*!
 *******************************************************************************
 * \file
 *
 * \brief
 *    -
 *
 * \author
 *      - Alexandre Massayuki Okazaki   (alexandre@lisha.ufsc.br)
 *      - Mateus Krepsky Ludwich        (mateus@lisha.ufsc.br)
 *      - Tiago de Albuqueque Reis      (reis@lisha.ufsc.br)
 * 
 *******************************************************************************
 */

#ifndef REFERENCE_PICTURE_MOTION_COUNTERPART_H_
#define REFERENCE_PICTURE_MOTION_COUNTERPART_H_

#include "me_component_typedefs.h"
#include "mec_picture.h"
#include "motion_vector.h"


typedef struct {
    MEC_Picture * referencePicture;

    uint cost;
    uint rate;
    short pred_mv[2];

    MotionVector translation;

    int generatedByWorker; /* just for debug */
    bool avalible;

} ReferencePictureMotionCounterpart;

ReferencePictureMotionCounterpart * new_ReferencePictureMotionCounterpart(MEC_Picture * referencePicture);
void delete_ReferencePictureMotionCounterpart(ReferencePictureMotionCounterpart * _this);

void printReferencePictureMotionCounterpart(ReferencePictureMotionCounterpart * _this);

bool rpmc_equals(ReferencePictureMotionCounterpart * thiz, ReferencePictureMotionCounterpart * other);

#endif /* REFERENCE_PICTURE_MOTION_COUNTERPART_H_ */

