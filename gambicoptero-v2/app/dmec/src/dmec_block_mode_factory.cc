
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
#include "../include/traits.h"
#include "../include/block_mode_factory.h"
#include "../include/block_mode_type.h"

#if LINUX
#include <stdlib.h>

#else
#include <utility/malloc.h>
#endif


BlockMode ** blockModeFactory_createH264BlockModes() {
    BlockMode ** null = (BlockMode **) 0;

    /* "level 2": 8x4, 4x8, 4x4 */
    BlockMode ** subModes8x8;
    subModes8x8 = (BlockMode **) calloc(3, sizeof(BlockMode *));
    subModes8x8[0] = new_BlockMode(MODE8x4, 2, 8, 4, null, 0, True);
    subModes8x8[1] = new_BlockMode(MODE4x8, 2, 4, 8, null, 0, True);
    subModes8x8[2] = new_BlockMode(MODE4x4, 2, 4, 4, null, 0, True);
    
    /* "level 1": 16x16, 16x8, 8x16, 8x8 */
    BlockMode ** modesLevel1;
    modesLevel1 = (BlockMode **) calloc(4, sizeof(BlockMode *));
    modesLevel1[0] = new_BlockMode(MODE16x16, 1, 16, 16, null, 0, True);
    modesLevel1[1] = new_BlockMode(MODE16x8, 1, 16, 8, null, 0, True);
    modesLevel1[2] = new_BlockMode(MODE8x16, 1, 8, 16, null, 0, True);
    modesLevel1[3] = new_BlockMode(MODE8x8, 1, 8, 8, subModes8x8, 3, False);

    /* "level 0": skip */
    BlockMode ** modes;
    modes = (BlockMode **) calloc(1, sizeof(BlockMode *));
    modes[0] = new_BlockMode(MODE_SKIP, 0, 0, 0, modesLevel1, 4, False); // Skip Mode
    
    return modes;
}
