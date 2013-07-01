#include "../include/master_tracedefs.h"
#include "../include/mec_macroblock.h"
#include "../include/traits.h"

#if LINUX
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#else
#include <utility/malloc.h>
#endif

#define __MEC_MACROBLOCK_DEBUG (0 && ENABLE_TRACE)
#define __MEC_MACROBLOCK_TRACE (0 && ENABLE_TRACE)

MEC_Macroblock * new_Macroblock(int ID, int xInPicture, int yInPicture, uint width, uint height, MEC_Picture * picture) {
    MEC_Macroblock * macroblock;
    macroblock = (MEC_Macroblock *) malloc(sizeof(MEC_Macroblock));

    macroblock->ID = ID;
    macroblock->xInPicture = xInPicture;
    macroblock->yInPicture = yInPicture;
    macroblock->width = width;
    macroblock->height = height;
    macroblock->picture = picture;

    return macroblock;
}

void delete_Macroblock(MEC_Macroblock * macroblock) {
    free(macroblock->blocks[MODE_SKIP][0]);
    free(macroblock->blocks[MODE_SKIP]);

    free(macroblock->blocks[MODE16x16][0]);
    free(macroblock->blocks[MODE16x16]);

    free(macroblock->blocks[MODE16x8][0]);
    free(macroblock->blocks[MODE16x8]);

    free(macroblock->blocks[MODE8x16][0]);
    free(macroblock->blocks[MODE8x16][1]);
    free(macroblock->blocks[MODE8x16]);

    free(macroblock->blocks[MODE8x8][0]);
    free(macroblock->blocks[MODE8x8][1]);
    free(macroblock->blocks[MODE8x8]);

    free(macroblock->blocks[MODE8x4][0]);
    free(macroblock->blocks[MODE8x4][1]);
    free(macroblock->blocks[MODE8x4]);

    free(macroblock->blocks[MODE4x8][0]);
    free(macroblock->blocks[MODE4x8][1]);
    free(macroblock->blocks[MODE4x8][2]);
    free(macroblock->blocks[MODE4x8][3]);
    free(macroblock->blocks[MODE4x8]);

    free(macroblock->blocks[MODE4x4][0]);
    free(macroblock->blocks[MODE4x4][1]);
    free(macroblock->blocks[MODE4x4][2]);
    free(macroblock->blocks[MODE4x4][3]);
    free(macroblock->blocks[MODE4x4]);

    free(macroblock);
}

void macroblock_print(MEC_Macroblock * _this) {
    //printf("mb(ID:%i,x:%i,y:%i,w:%i,h:%i)\n", _this->ID, _this->xInPicture, _this->yInPicture, _this->width, _this->height);
}


int computeMacroblockID(int xInPicture, int yInPicture, uint pictureWidth, uint pictureHeight) {
#if __MEC_MACROBLOCK_TRACE
	printf("computeMacroblockID\n");
	printf("...xInPicture:%i, yInPicture:%i, pictureWidth:%i, pictureHeight:%i\n",
			xInPicture, yInPicture, pictureWidth, pictureHeight);
#endif

	unsigned int MBH = pictureWidth / MACROBLOCK_SIZE;

	int id = (xInPicture / MACROBLOCK_SIZE) +
			((((MBH * yInPicture) / MACROBLOCK_SIZE)/MBH) * MBH);

#if 0
	int oid = __computeMacroblockID(xInPicture, yInPicture, pictureWidth, pictureHeight);
	printf("...id:%i, oid:%i\n", id, oid);
	assert(id == oid);
#endif

#if __MEC_MACROBLOCK_TRACE
	printf("...id:%i\n", id);
#endif

	return id;
}
