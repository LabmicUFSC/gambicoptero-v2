#include <utility/AQMath.h>

__USING_SYS

float AQMath::constrain(float x, float a,float  b){
    if(x >= a && x <= b) return x;
    if (x < a) return a;
    if (x > b) return b;


}

float AQMath::filterSmooth(float currentData, float previousData, float smoothFactor){
    if(smoothFactor != 1.0) {
        return (previousData * (1.0 -smoothFactor) + (currentData * smoothFactor));
    }
    return currentData;
}

void AQMath::vectorSubtract(int length, float vectorC[], float vectorA[], float vectorB[]){
    for(int i = 0; i < length; i++)
    {
       vectorC[i] = vectorA[i] - vectorB[i];
    }
}


void AQMath::matrixMultiply(int aRows, int aCols_bRows, int bCols, float matrixC[], float matrixA[], float matrixB[])
{
  for (int i = 0; i < aRows * bCols; i++)
  {
    matrixC[i] = 0.0;
  }

  for (int i = 0; i < aRows; i++)
  {
    for(int j = 0; j < aCols_bRows; j++)
    {
      for(int k = 0;  k < bCols; k++)
      {
       matrixC[i * bCols + k] += matrixA[i * aCols_bRows + j] * matrixB[j * bCols + k];
      }
    }
  }
}

void AQMath::matrixAdd(int rows, int cols, float matrixC[], float matrixA[], float matrixB[])
{
  for (int i = 0; i < rows * cols; i++)
  {
    matrixC[i] = matrixA[i] + matrixB[i];
  }
}

float AQMath::vectorDotProduct(int length, float vector1[], float vector2[])
{
  float dotProduct = 0;
  //int   i;

  for (int i = 0; i < length; i++)
  {
  dotProduct += vector1[i] * vector2[i];
  }

  return dotProduct;
}


void AQMath::vectorScale(int length, float scaledVector[], float inputVector[], float scalar)
{
  for (int i = 0; i < length; i++)
  {
   scaledVector[i] = inputVector[i] * scalar;
  }
}

void AQMath::vectorAdd(int length, float vectorC[], float vectorA[], float vectorB[])
{
  for(int i = 0; i < length; i++)
  {
     vectorC[i] = vectorA[i] + vectorB[i];
  }
}

void AQMath::vectorCrossProduct(float vectorC[3], float vectorA[3], float vectorB[3])
{
  vectorC[0] = (vectorA[1] * vectorB[2]) - (vectorA[2] * vectorB[1]);
  vectorC[1] = (vectorA[2] * vectorB[0]) - (vectorA[0] * vectorB[2]);
  vectorC[2] = (vectorA[0] * vectorB[1]) - (vectorA[1] * vectorB[0]);
}
