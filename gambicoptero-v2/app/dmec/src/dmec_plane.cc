
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

#include "../include/mec_plane.h"
#include "../include/traits.h"

#if LINUX
#include <stdlib.h>

#else
#include <utility/malloc.h>
#endif


MEC_Plane * new_Plane(unsigned int width, unsigned int height, MEC_Sample ** samples) {
    MEC_Plane * plane;
    plane = (MEC_Plane *) malloc(sizeof(MEC_Plane));

    plane->width = width;
    plane->height = height;

    plane->samples = samples;

    return plane;
}

void delete_Plane(MEC_Plane * _this) {
    free(_this);
}

