
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
#include "../include/block_mode.h"

#if LINUX
#include <stdlib.h>
#include <iostream>

using namespace std;

#else
#include <utility/malloc.h>
#include <utility/ostream.h>
typedef System::OStream OStream;
extern OStream cout;
#endif

BlockMode * new_BlockMode(BlockModeType type, uint level, uint width, uint height, BlockMode ** subModes, uint numberOfSubModes, boolean isLeaf) {
    BlockMode * _this;
    _this = (BlockMode *) malloc(sizeof(BlockMode));

    _this->type = type;
    _this->level = level;
    _this->width = width;
    _this->height = height;

    _this->subModes = subModes;
    _this->numberOfSubModes = numberOfSubModes;
    _this->isLeaf = isLeaf; // numberOfSubModes == 0 and subModes == null
    
    return _this;
}

void delete_BlockMode(BlockMode * _this) {
    // delete subModes? Yes!
    unsigned int i;
    for(i = 0; i < _this->numberOfSubModes; ++ i) {
        //cout << "delete_BlockMode() i = " << i << "\n";
        delete_BlockMode(_this->subModes[i]);
    }

    if (_this->numberOfSubModes > 0) {
    	//cout << "delete_BlockMode() free() 1, type: " << _this->type << " level: " << _this->level << "\n";
    	free(_this->subModes);
    	//cout << "delete_BlockMode() free() 2\n";
    }

    free(_this);
}

void blockMode_print(BlockMode * _this) {
}
