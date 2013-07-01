
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

#include "../include/mec_picture.h"
#include "../include/traits.h"

#if LINUX
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#else
#include <utility/malloc.h>
#endif

MEC_Picture::MEC_Picture(unsigned int width, unsigned int height, MEC_Sample** samples)
{
	Y = new_Plane(width, height, samples);
}


MEC_Picture::~MEC_Picture()
{
    delete_Plane(Y);
}
