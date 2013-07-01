
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

#ifndef PLANE_H_
#define PLANE_H_

#include "mec_sample.h"

typedef struct {
    unsigned int width;
    unsigned int height;

    MEC_Sample ** samples;
} MEC_Plane;

MEC_Plane * new_Plane(unsigned int width, unsigned int height, MEC_Sample ** samples);
void delete_Plane(MEC_Plane * _this);

#endif /* PLANE_H_ */

