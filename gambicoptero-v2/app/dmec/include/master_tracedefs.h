/*
 Author: Mateus Krepsky Ludwich.
*/

#ifndef MASTER_TRACEDEFS_H_
#define MASTER_TRACEDEFS_H_

#define ENABLE_TRACE 0
#define ENABLE_DUMP 0

#define ENABLE_MEC_OBSERVER 0


/* Chronometer defs */
#define ENABLE_CHRONOMETER 0

#define ENABLE_CHRONO_COPY_MEC2DMEC_TIME            0   /* Chronometer #1 coordinator-side */
#define ENABLE_CHRONO_WAITING_FOR_WORKERS_TIME		0   /* Chronometer #2 coordinator-side */
#define ENABLE_CHRONO_FILL_PMC_TIME					0   /* Chronometer #3 coordinator-side */
#define ENABLE_CHRONO_TOTAL_ME_TIME				    0   /* Chronometer #4 coordinator-side */


#define ENABLE_CHRONOMETER_FOR_WORKERS 0 && ENABLE_CHRONOMETER

#define ENABLE_CHRONO_WAITING_FOR_COORDINATOR 		0 	/* Chronometer #1 worker-side */
#define ENABLE_CHRONO_GET_SAMPLES_TIME 				0   /* Chronometer #2 worker-side */
#define ENABLE_CHRONO_GET_MV_TIME                   0 	/* Chronometer #3 worker-side */
#define ENABLE_CHRONO_MATCH_ALGORITHM_TIME          0   /* Chronometer #4 worker-side */
#define ENABLE_CHRONO_RECORDING_RESULTS_TIME        0	/* Chronometer #5 worker-side */
#define ENABLE_CHRONO_TOTAL_WORKER_TIME             0 	/* Chronometer #6 worker-side */

//#define ENABLE_CHRONOMETER_ME_TIME

#define ENABLE_STALL_VERIFICATION 1 /* If enabled (== 1) verifies if occurs DMA queue stall in CELL implementation. */

#endif /* MASTER_TRACEDEFS_H_ */
