/*
 * picture.h
 *
 *  Created on: Sep 28, 2011
 *      Author: mateus
 */

#ifndef PICTURE_H_
#define PICTURE_H_

#include "mec_sample.h"

// <interface>
class Picture {
	/* All methods in this interface are declared as private instead of pure
	 * virtual (= 0), for performance reasons.
	*/
private:
	/* \brief
	 * 		Gets one sample of specified row and column.
	 */
	MEC_Sample sample(int row, int column);


	/* \brief
	 * 		Gets picture's width in pixels.
	 */
	unsigned int width();


	/* \brief
	 *    	Gets picture's height in pixels.
	 */
	unsigned int height();

};

#endif /* PICTURE_H_ */
