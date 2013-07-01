
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

#include "../include/h264_modes_partitioner.h"
#include "../include/block_mode_type.h"
#include "../include/error.h"

#if LINUX
#include <stdlib.h>
#include <assert.h>
// #include <simple_tracer.h>

#else

#include <utility/malloc.h>
#include <machine.h>

#endif

MEC_Block ** h264ModesPartitioner_partition(BlockMode * blockMode, MEC_Macroblock * macroblock, uint * p_numberOfBlocks)
{
    // simple_tracer_trace("h264ModesPartitioner_partition");
    
    MEC_Block ** blocks = (MEC_Block **) 0;
    int numberOfBlocks = 0;

    if (blockMode->type == MODE16x16) {
        //simple_tracer_trace("partition: 16x16");
        numberOfBlocks = 1;
        blocks = (MEC_Block **) calloc(1, sizeof(MEC_Block *));
        blocks[0] = new_Block(0, 0, 16, 16, macroblock);
    }
    else if (blockMode->type == MODE16x8) {
        //simple_tracer_trace("partition: 16x8");
        numberOfBlocks = 2;
        blocks = (MEC_Block **) calloc(2, sizeof(MEC_Block *));
        blocks[0] = new_Block(0, 0, 16, 8, macroblock);
        blocks[1] = new_Block(0, 8, 16, 8, macroblock);
    }
    else if (blockMode->type == MODE8x16) {
        //simple_tracer_trace("partition: 8x16");
        numberOfBlocks = 2;
        blocks = (MEC_Block **) calloc(2, sizeof(MEC_Block *));
        blocks[0] = new_Block(0, 0, 8, 16, macroblock);
        blocks[1] = new_Block(8, 0, 8, 16, macroblock);
    }
    else if (blockMode->type == MODE8x8) {
        //simple_tracer_trace("partition: 8x8");
        numberOfBlocks = 4;
        blocks = (MEC_Block **) calloc(4, sizeof(MEC_Block *));
        blocks[0] = new_Block(0, 0, 8, 8, macroblock);
        blocks[1] = new_Block(8, 0, 8, 8, macroblock);
        blocks[2] = new_Block(0, 8, 8, 8, macroblock);
        blocks[3] = new_Block(8, 8, 8, 8, macroblock);
    }
    else if (blockMode->type == MODE8x4) {
        //simple_tracer_trace("partition: 8x4");
        
        numberOfBlocks = 8;
        blocks = (MEC_Block **) calloc(8, sizeof(MEC_Block *));
        blocks[0] = new_Block(0, 0,  8, 4, macroblock);
        blocks[1] = new_Block(8, 0,  8, 4, macroblock);
        blocks[2] = new_Block(0, 4,  8, 4, macroblock);
        blocks[3] = new_Block(8, 4,  8, 4, macroblock);
        blocks[4] = new_Block(0, 8,  8, 4, macroblock);
        blocks[5] = new_Block(8, 8,  8, 4, macroblock);
        blocks[6] = new_Block(0, 12, 8, 4, macroblock);
        blocks[7] = new_Block(8, 12, 8, 4, macroblock);        
    }
    else if (blockMode->type == MODE4x8) {
        //simple_tracer_trace("partition: 4x8");
        
        numberOfBlocks = 8;
        blocks = (MEC_Block **) calloc(8, sizeof(MEC_Block *));
        blocks[0] = new_Block(0,  0, 4, 8, macroblock);
        blocks[1] = new_Block(4,  0, 4, 8, macroblock);
        blocks[2] = new_Block(8,  0, 4, 8, macroblock);
        blocks[3] = new_Block(12, 0, 4, 8, macroblock);
        blocks[4] = new_Block(0,  8, 4, 8, macroblock);
        blocks[5] = new_Block(4,  8, 4, 8, macroblock);
        blocks[6] = new_Block(8,  8, 4, 8, macroblock);
        blocks[7] = new_Block(12, 8, 4, 8, macroblock);
    }
    else if (blockMode->type == MODE4x4) {
        //simple_tracer_trace("partition: 4x4");
        
        numberOfBlocks = 16;
        blocks = (MEC_Block **) calloc(16, sizeof(MEC_Block *));
        blocks[0]  = new_Block(0,  0,  4, 4, macroblock);
        blocks[1]  = new_Block(4,  0,  4, 4, macroblock);
        blocks[2]  = new_Block(8,  0,  4, 4, macroblock);
        blocks[3]  = new_Block(12, 0,  4, 4, macroblock);
        blocks[4]  = new_Block(0,  4,  4, 4, macroblock);
        blocks[5]  = new_Block(4,  4,  4, 4, macroblock);
        blocks[6]  = new_Block(8,  4,  4, 4, macroblock);
        blocks[7]  = new_Block(12, 4,  4, 4, macroblock);
        blocks[8]  = new_Block(0,  8,  4, 4, macroblock);
        blocks[9]  = new_Block(4,  8,  4, 4, macroblock);
        blocks[10] = new_Block(8,  8,  4, 4, macroblock);
        blocks[11] = new_Block(12, 8,  4, 4, macroblock);
        blocks[12] = new_Block(0,  12, 4, 4, macroblock);
        blocks[13] = new_Block(4,  12, 4, 4, macroblock);
        blocks[14] = new_Block(8,  12, 4, 4, macroblock);
        blocks[15] = new_Block(12, 12, 4, 4, macroblock);
    }
    else if (blockMode->type == MODE_SKIP) {
        //simple_tracer_trace("partition: skip");
        numberOfBlocks = 1;
        blocks = (MEC_Block **) calloc(1, sizeof(MEC_Block *));
        blocks[0] = new_Block(0, 0, 16, 16, macroblock);
    }
    else {
        //simple_tracer_trace("partition: error");
        // error: Unknow mode!
        //abort();
        Abort();
    }

    //simple_tracer_trace("setting *p_numberOfBlocks...");
    *p_numberOfBlocks = numberOfBlocks;
    //simple_tracer_trace("returning from h264ModesPartitioner_partition...");
    return blocks;
}


