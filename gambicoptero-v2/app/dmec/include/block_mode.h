
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

#ifndef BLOCK_MODE_H_
#define BLOCK_MODE_H_

#include "me_component_typedefs.h"
#include "block_mode_type.h"

typedef struct BlockModeStruct {
    uint level;
    uint width;
    uint height;

    struct BlockModeStruct ** subModes;
    uint numberOfSubModes;

    boolean isLeaf;

    BlockModeType type;
        
} BlockMode;

BlockMode * new_BlockMode(BlockModeType type, uint level, uint width, uint height, BlockMode ** subModes, uint numberOfSubModes, boolean isLeaf);
void delete_BlockMode(BlockMode * _this);
void blockMode_print(BlockMode * _this);

#endif /* BLOCK_MODE_H_ */

