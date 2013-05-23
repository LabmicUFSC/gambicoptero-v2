#ifndef AQMATH_H
#define AQMATH_H
#include<system/config.h>

__BEGIN_SYS

class AQMath{
public:

static int findMedianInt(short *data, int arraySize);
static float constrain(float x,float  a, float b);

static float filterSmooth(float currentData, float previousData, float smoothFactor);

static void vectorSubtract(int length, float vectorC[], float vectorA[], float vectorB[]);

static void matrixMultiply(int aRows, int aCols_bRows, int bCols,
                    float matrixC[], float matrixA[], float matrixB[]);

static void matrixAdd(int rows, int cols, float matrixC[], float matrixA[], float matrixB[]);

static float vectorDotProduct(int length, float vector1[], float vector2[]);

static void vectorScale(int length, float scaledVector[], float inputVector[], float scalar);

static void vectorAdd(int length, float vectorC[], float vectorA[], float vectorB[]);

static void vectorCrossProduct(float vectorC[3], float vectorA[3], float vectorB[3]);

};
__END_SYS

#endif // AQMATH_H
