
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

#include "../include/mec_block.h"

#if LINUX
#include <stdlib.h>

#else
#include <utility/malloc.h>
#endif

MEC_Block * new_Block(int xInMacroblock, int yInMacroblock, uint width, uint height, MEC_Macroblock * macroblock) {
    MEC_Block * block;
    block = (MEC_Block *) malloc(sizeof(MEC_Block));
    
    block->xInMacroblock = xInMacroblock;
    block->yInMacroblock = yInMacroblock;
    block->width = width;
    block->height = height;
    block->macroblock = macroblock;
    
    return block;
}

void delete_Block(MEC_Block * block) {
    free(block);
}
