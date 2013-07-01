
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

#ifndef MEC_PICTURE_H_
#define MEC_PICTURE_H_

#include "mec_plane.h"
#include "traits.h"
#include "picture.h"

class MEC_Picture : public Picture {

public:
	// Constructor and destructor
	MEC_Picture(unsigned int width, unsigned int height, MEC_Sample** samples);

	~MEC_Picture();


	// General methods
	MEC_Sample sample(int row, int column)
	{
		return Y->samples[row][column];
	}


	unsigned int width()
	{
		return Y->width;
	}



	unsigned int height()
	{
		return Y->height;
	}




// Instance variables
private:
	MEC_Plane* Y;

    /* MEC_Macroblock */
	void*** macroblocks;

};


#endif /*MEC_PICTURE_H_ */

