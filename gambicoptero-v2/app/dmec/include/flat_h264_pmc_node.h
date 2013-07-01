
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

#include "traits.h"

#ifndef FLAT_H264_PMC_NODE_H_
#define FLAT_H264_PMC_NODE_H_

struct __FlatH264PMC_Node {
    /* Identification */
    /* Macroblock */
    int xInPicture;
    int yInPicture;
    int macroblockID;

    /* Block (macroblock) Mode */
    int h264BlockMode;

    /* Block */
    int xInMacroblock;
    int yInMacroblock;

    /* Reference Picture List (0 or 1) */
    unsigned int referencePictureListNumber;

    /* Reference Picture */
    int referencePictureID;

    /* "input/output" */
    /* predictors */
    short pred_mv_x;
    short pred_mv_y;

    /* output */
    int cost;
    short mv_x;
    short mv_y;

} __attribute__((aligned(ALIGNMENT)));

typedef struct __FlatH264PMC_Node FlatH264PMC_Node;

bool flatH264PMC_Node_equals(FlatH264PMC_Node * thiz, FlatH264PMC_Node * other);

#endif /* FLAT_H264_PMC_NODE_H_ */

