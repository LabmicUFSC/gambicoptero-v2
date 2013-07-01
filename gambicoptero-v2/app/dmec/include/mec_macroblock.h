
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

#ifndef MACROBLOCK_H_
#define MACROBLOCK_H_

#include "me_component_typedefs.h"
#include "mec_picture.h"
#include "block_mode_type.h"

typedef struct {
    int ID;

    uint xInPicture;
    uint yInPicture;

    uint width;
    uint height;

    MEC_Picture * picture;

    // MEC_Block
    void *** blocks[8];
} MEC_Macroblock;

MEC_Macroblock * new_Macroblock(int ID, int xInPicture, int yInPicture, uint width, uint height, MEC_Picture * picture);
void delete_Macroblock(MEC_Macroblock * macroblock);
void macroblock_print(MEC_Macroblock * _this);

int computeMacroblockID(int xInPicture, int yInPicture, uint pictureWidth, uint pictureHeight);

#endif // MACROBLOCK_H_

