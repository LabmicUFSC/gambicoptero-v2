
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

#ifndef BLOCK_H_
#define BLOCK_H_

#include "me_component_typedefs.h"
#include "mec_macroblock.h"

typedef struct {
    int xInMacroblock;
    int yInMacroblock;

    uint width;
    uint height;

    MEC_Macroblock * macroblock;

    // (BlockMotionCounterpart *)
    void * blockMotionCounterpart;
} MEC_Block;

MEC_Block * new_Block(int xInMacroblock, int yInMacroblock, uint width, uint height, MEC_Macroblock * macroblock);
void delete_Block(MEC_Block * block);


#endif /* BLOCK_H_ */

