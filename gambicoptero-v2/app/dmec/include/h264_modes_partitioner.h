
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

#ifndef H264_MODES_PARTITIONER_H_
#define H264_MODES_PARTITIONER_H_

#include "me_component_typedefs.h"
#include "mec_block.h"
#include "block_mode.h"
#include "mec_macroblock.h"

MEC_Block ** h264ModesPartitioner_partition(BlockMode * blockMode, MEC_Macroblock * macroblock, uint * p_numberOfBlocks);

#endif /* H264_MODES_PARTITIONER_H_ */

