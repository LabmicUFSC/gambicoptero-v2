
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

#include "../include/reference_picture_motion_counterpart.h"

#if LINUX
#include <stdlib.h>
#include <iostream>

using namespace std;

#else
#include <utility/malloc.h>

#endif

ReferencePictureMotionCounterpart * new_ReferencePictureMotionCounterpart(MEC_Picture * referencePicture) {
    ReferencePictureMotionCounterpart * _this;
    _this = (ReferencePictureMotionCounterpart *) malloc(sizeof(ReferencePictureMotionCounterpart));

    _this->referencePicture = referencePicture;

    _this->cost = 0;

    _this->translation.mv_x = 0;
    _this->translation.mv_y = 0;

    _this->generatedByWorker = -97;

    return _this;
}

void delete_ReferencePictureMotionCounterpart(ReferencePictureMotionCounterpart * _this) {
    free(_this);
}

void printReferencePictureMotionCounterpart(ReferencePictureMotionCounterpart * _this) {
	/*printf("\t\t\t\t\tRPMC={\n");
	printf("\t\t\t\t\t\tmv_x:%i, mv_y:%i, cost:%i\n",
			_this->translation.mv_x,
			_this->translation.mv_y,
			_this->cost);
	printf("\t\t\t\t\t}\n");*/
}


bool rpmc_equals(ReferencePictureMotionCounterpart * thiz, ReferencePictureMotionCounterpart * other)
{
	if (thiz->avalible != other->avalible) {
		return false;
	}

	if (thiz->cost != other->cost) {
		return false;
	}

	if(thiz->generatedByWorker != other->generatedByWorker) {
		return false;
	}

	if (thiz->translation.mv_x != other->translation.mv_x) {
		return false;
	}

	if (thiz->translation.mv_y != other->translation.mv_y) {
		return false;
	}

	return true;
}
