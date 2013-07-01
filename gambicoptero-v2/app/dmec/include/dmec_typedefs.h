/*
 * dmec_typedefs.h
 *
 *  Created on: Dec 22, 2010
 *      Author: mateus
 */

#ifndef DMEC_TYPEDEFS_H_
#define DMEC_TYPEDEFS_H_

#if 1 /* PC // 32 bits */
typedef unsigned long Address;

#else // Cell
typedef unsigned long long Address;

#endif

#endif /* DMEC_TYPEDEFS_H_ */
