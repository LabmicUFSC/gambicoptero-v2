
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

#include "../include/master_tracedefs.h"

#include "../include/dmec_picture.h"

void copy_MEC_Picture_to_DMEC_Picture(MEC_Picture * source, DMEC_Picture * target) {
	//assert(source);
	//assert(target);

	/* target->ID = source->ID; */
	target->width = source->width();
	target->height = source->height();


	unsigned int row, col;
	for (row = 0; row < source->height(); ++ row) {
		for (col = 0; col < source->width(); ++ col) {
			/* traceMIMII("target->Y_samples[%i] = ", row * target->width + col,
						 "source->Y->samples[%i][%i]", row, col); */

			target->Y_samples[row * target->width + col] = source->sample(row, col);
		}
	}

}


void setUpEAs_DMEC_Picture(DMEC_Picture * picture, EAs_DMEC_Picture * eas) {
	/* unsigned long ea_ID; */
	unsigned long ea_Y_samples;

	/* ea_ID = (unsigned long) &picture->ID; */
	ea_Y_samples = (unsigned long) &picture->Y_samples;

	/* eas->ea_ID = ea_ID; */
	eas->ea_Y_samples = ea_Y_samples;
}
