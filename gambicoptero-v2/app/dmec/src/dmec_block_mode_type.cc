
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

#include "../include/block_mode_type.h"
#include "../include/error.h"


void block_mode_type_print(BlockModeType _this) {
}

BlockModeType get_block_mode_type(int sizex, int sizey) {
    BlockModeType bmt = MODE_SKIP;
    if (sizex == 16) {
        if (sizey == 16) {
            bmt = MODE16x16;
            return bmt;
        } else if (sizey == 8) {
            bmt = MODE16x8;
            return bmt;
        }
    } else if (sizex == 8) {
        if (sizey == 16) {
            bmt = MODE8x16;
            return bmt;
        } else if (sizey == 8) {
            bmt = MODE8x8;
            return bmt;
        } else if (sizey == 4) {
            bmt = MODE8x4;
            return bmt;
        }
    } else if (sizex == 4) {
        if (sizey == 8) {
            bmt = MODE4x8;
            return bmt;
        } else if (sizey == 4) {
            bmt = MODE4x4;
            return bmt;
        }
    }
    return bmt;
}

void getBlockDimensions(BlockModeType h264BlockMode, unsigned int * blockWidth, unsigned int * blockHeight) {
    switch(h264BlockMode) {
    case MODE_SKIP:
        *blockWidth = 16;
        *blockHeight = 16;
        break;
    case MODE16x16:
        *blockWidth = 16;
        *blockHeight = 16;
        break;
    case MODE16x8:
        *blockWidth = 16;
        *blockHeight = 8;
        break;
    case MODE8x16:
        *blockWidth = 8;
        *blockHeight = 16;
        break;
    case MODE8x8:
        *blockWidth = 8;
        *blockHeight = 8;
        break;
    case MODE8x4:
        *blockWidth = 8;
        *blockHeight = 4;
        break;
    case MODE4x8:
        *blockWidth = 4;
        *blockHeight = 8;
        break;
    case MODE4x4:
        *blockWidth = 4;
        *blockHeight = 4;
        break;
    default:
        //printf("BlockModeType::getBlockDimensions, Error: ");
        //printf("Unknown block mode: %i. Aborting...", h264BlockMode);
        //abort();
        Abort();
        break;
    }
}


int bmt_getNumBlocks(BlockModeType _this)
{
	/* MODE_SKIP, MODE16x16, ..., MODE4x8, MODE4x4 */
	int numBlocks[8] = {1, 1, 2, 2, 4, 8, 8, 16};
	return numBlocks[_this];
}
