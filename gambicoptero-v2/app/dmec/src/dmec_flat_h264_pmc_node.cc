
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

#include "../include/flat_h264_pmc_node.h"

bool flatH264PMC_Node_equals(FlatH264PMC_Node * thiz, FlatH264PMC_Node * other)
{
	if (thiz->cost != other->cost) {
		return false;
	}

	if (thiz->h264BlockMode != other->h264BlockMode) {
		return false;
	}

	if (thiz->macroblockID != other->macroblockID) {
		return false;
	}

	if (thiz->mv_x != other->mv_x) {
		return false;
	}

	if (thiz->mv_y != other->mv_y) {
		return false;
	}

	if (thiz->pred_mv_x != other->pred_mv_x) {
		return false;
	}

	if (thiz->pred_mv_y != other->pred_mv_y) {
		return false;
	}

	if (thiz->referencePictureID != other->referencePictureID) {
		return false;
	}

	if (thiz->referencePictureListNumber != other->referencePictureListNumber) {
		return false;
	}

	if (thiz->xInMacroblock != other->xInMacroblock) {
		return false;
	}

	if (thiz->xInPicture != other->xInPicture) {
		return false;
	}

	if (thiz->yInMacroblock != other->yInMacroblock) {
		return false;
	}

	if (thiz->yInPicture != other->yInPicture) {
		return false;
	}


	return true;
}
