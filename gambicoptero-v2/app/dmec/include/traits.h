/*
 * common_constants.h
 *
 *  Created on: Jun 15, 2010
 *      Author: mateus
 */

#ifndef COMMON_CONSTANTS_H_
#define COMMON_CONSTANTS_H_

#ifdef COMPILEFORLINUX
#define LINUX 1
#else
#define LINUX 0
#endif


#if !LINUX
#define INT_MAX 2147483647
#endif


#define MACROBLOCK_SIZE 16
#define NUMBER_OF_BLOCKS_FOR_ALL_MODES 41

#define SEARCH_RANGE 16


/* The maximum number of macroblocks for a picture 1920x1088 ("rounded" full-hd) */
#define MAX_NUM_MACROBLOCKS 8160

#define NUM_H264_MODES 8

#define ENABLE_CHROMA_PLANES 0

#define ENABLE_BI_PREDICTION 1

#define ENABLE_DEBUG_ASPECT 1


/* Configurations ----------------------------------------------------------- */

// The Block Match Algorithm which will be used.
class FullSearchMatchAlgorithm;
class AdapterFullSearchMatchAlgorithm;

//#if ENABLE_DEBUG_ASPECT
//typedef AdapterFullSearchMatchAlgorithm ActualMatchAlgorithm;

//#else
typedef FullSearchMatchAlgorithm ActualMatchAlgorithm;

//#endif



#define NUM_WORKERS 6


/* Change MAX_PICTURE_WIDTH and MAX_PICTURE_HEIGHT back to FullHD TODO */
/* Full HD: 1920x1080, 1920x1088 (cropped) */
// #define MAX_PICTURE_WIDTH 1920
// #define MAX_PICTURE_HEIGHT 1088
/* QCIF: 176x144 */
#define MAX_PICTURE_WIDTH 176
#define MAX_PICTURE_HEIGHT 144


/*
 * The number of flat PMCs for a picture is defined as
 * ((picture_width * picture_height) / 256 ) * 42 * number_of_lists * number_of_reference_frames
 * e.g.1:
 * 	1920 * 1088 / 256 => 8160 * 42 * 2 * 5 = 3427200 (over 6 Workers => 571200)
 *
 * e.g.2:
 *  176 * 144 / 256 => 99 * 42 * 2 * 1 => 8316
 *
 * */
/* #define MAX_FLAT_PMC_SIZE_FOR_COORDINATOR 571200 */ /* number of flat PMCs for a picture 1920x1088 for 10 reference pictures divided by 6 */
#define MAX_FLAT_PMC_SIZE_FOR_COORDINATOR ((((MAX_PICTURE_WIDTH * MAX_PICTURE_HEIGHT) / 256) * NUMBER_OF_BLOCKS_FOR_ALL_MODES * 2 * 1) / 1)

#define MAX_FLAT_PMC_SIZE_FOR_WORKER 1

#define MAX_REFERENCE_PICTURE_LIST0_SIZE 1 /* 5 */
#define MAX_REFERENCE_PICTURE_LIST1_SIZE 1 /* 5 */

/* #define MAX_SAMPLES_LUMA_FOR_WORKER 2000 */
/* #define MAX_SAMPLES_CHROMA_FOR_WORKER 1000 */
/* A menor partição será de 16x16 (tamanho de macrobloco) e todas as partições
 * são multiplas de macrobloco. Portanto um cache de 256 sempre será multiplo
 * das partições. Funcionou para pictures QCIF, CIF e 4CIF.
 *
 * 1024 (32x32) também é um bom valor para ser testado. */
#define MAX_SAMPLES_LUMA_FOR_WORKER 256
#define MAX_SAMPLES_CHROMA_FOR_WORKER 64 /* 8x8 */


/* For use with single planes pictures */
/* #define MAX_SAMPLES_FOR_WORKER 256 */
/* #define MAX_SAMPLES_FOR_WORKER 8192 */ /* 16K bytes / sizeof(MEC_Sample) */
#define MAX_SAMPLES_FOR_WORKER MAX_SAMPLES_LUMA_FOR_WORKER /* 16K bytes / sizeof(MEC_Sample) */


#define ALIGNMENT 128
/* #define ALIGNMENT 16 */


#define USE_OBSOLETE 0

/* This must be enabled when "PIC_DOUBLE_BUFFER" is enabled in dmec_objects */
#define LAZY_PICTURE_ENABLE_DOUBLE_BUFFER 1


#define ENABLE_ASYNCH_DMA_FOR_CELL 1


#define ENABLE_ASYNCH_TRANSF 0


/* -------------------------------------------------------------------------- */

#endif /* COMMON_CONSTANTS_H_ */


