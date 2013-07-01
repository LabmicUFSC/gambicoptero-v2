
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

#ifndef BLOCK_MODE_TYPE_H_
#define BLOCK_MODE_TYPE_H_

typedef
enum {
	/* It is explicit here the value of each mode, to remember that this order
	 * must not change, because these order is used by BlockModeType functions.
	 */
    MODE_SKIP = 0,
    MODE16x16 = 1,
    MODE16x8 = 2,
    MODE8x16 = 3,
    MODE8x8 = 4,
    MODE8x4 = 5,
    MODE4x8 = 6,
    MODE4x4 = 7
} BlockModeType;

void block_mode_type_print(BlockModeType _this);

BlockModeType get_block_mode_type(int sizex, int sizey);

void getBlockDimensions(BlockModeType h264BlockMode, unsigned int * blockWidth, unsigned int * blockHeight);

int bmt_getNumBlocks(BlockModeType _this);

#endif /* BLOCK_MODE_TYPE_H_ */


