
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

#ifndef DMEC_PICTURE_H_
#define DMEC_PICTURE_H_

#include "mec_picture.h"

#include "dmec_picture_internals.h"

void copy_MEC_Picture_to_DMEC_Picture(MEC_Picture * source, DMEC_Picture * target);

void setUpEAs_DMEC_Picture(DMEC_Picture * picture, EAs_DMEC_Picture * eas);

#endif // DMEC_PICTURE_H_

