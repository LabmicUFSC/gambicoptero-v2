#include <utility/math.h>
#include<utility/ostream.h>

__USING_SYS

float Math::degrees(float radian){
    return radian * 57.2957795;

}

float Math::filterSmooth(float currentData, float previousData, float smoothFactor){
    if(smoothFactor != 1.0) {
        return (previousData * (1.0 -smoothFactor) + (currentData * smoothFactor));
    }
    return currentData;
}


int Math::findMedianInt(short *data, int arraySize)
{
  int temp;
  bool done = 0;
  unsigned char i;

   // Sorts numbers from lowest to highest
  while (done != 1)
  {
    done = 1;
    for (i=0; i<(arraySize-1); i++)
    {
      if (data[i] > data[i+1])
      {     // numbers are out of order - swap
        temp = data[i+1];
        data[i+1] = data[i];
        data[i] = temp;
        done = 0;
     //   db<ITG3200> (TRC) << data[i] << ", " ;
      }
    }
  }

  return data[arraySize/2]; // return the median value
}

float Math::logf(float num, float base , float epsilon ) {
    float integer = 0;
    if (num == 0) return 1;

    if (num < 1  && base < 1) return 0;

    while (num < 1) {
        integer--;
        num *= base;
    }

    while (num >= base) {
        integer++;
        num /= base;
    }

    float partial = 0.5;
    num *= num;
    float decimal = 0.0;
    while (partial > epsilon) {
        if (num >= base) {
            decimal += partial;
            num /= base;
        }
        partial *= 0.5;
        num *= num;
    }
    return (integer + decimal);
}

float Math::radians(float degrees){
    return degrees * 0.0174532925;

}




double Math::mxatan(double arg)
    {
        double argsq, value;

        argsq = arg*arg;
        value = ((((p4*argsq + p3)*argsq + p2)*argsq + p1)*argsq + p0);
        value = value/(((((argsq + q4)*argsq + q3)*argsq + q2)*argsq + q1)*argsq + q0);
        return value*arg;
    }

double Math::msatan(double arg)
   {
       if(arg < sq2m1)
           return mxatan(arg);
       if(arg > sq2p1)
           return PIO2 - mxatan(1/arg);
           return PIO2/2 + mxatan((arg-1)/(arg+1));
   }
float Math::abs(float x) {
    if (x < 0.0) return 0.0f - x;
    return x;

}
int Math::abs(int x) {
    if (x < 0) return 0 - x;
    return x;

}

long long Math::abs(long long x) {
    if (x < 0) return 0 - x;
    return x;
}

float Math::sqrt(float x){
    OStream cout;
  //  cout << " --- Square root ---" << endl;
    float left = 0;
    float right = x;
     float testValue = 0;
    while (abs(left - right) >=  0.01){//1e-15) {
       // cout << " left: " << (int)left << " right "<< (int)right << endl;
        testValue = (right + left)/2.0;
     //  cout << "TestValue: " << (int)testValue << endl;
     if (testValue * testValue <= x)
       left = testValue;
     else
      right = testValue;
    }
    return left;

}

double Math::atan(double arg)
    {
        if(arg > 0)
            return msatan(arg);
        return -msatan(-arg);
    }

double Math::atan2(double arg1, double arg2) {
    if(arg1+arg2 == arg1)
           {
               if(arg1 >= 0)
               return PIO2;
                   return -PIO2;
           }
           arg1 = atan(arg1/arg2);
           if(arg2 < 0)
          {
               if(arg1 <= 0)
                   return arg1 + PI;
               return arg1 - PI;
           }
           return arg1;
}


double Math::asin(double arg)
    {
        double temp;
        int sign;

        sign = 0;
        if(arg < 0)
        {
            arg = -arg;
            sign++;
        }
        if(arg > 1)
            return (0.0/0.0);
        temp = sqrt(1 - arg*arg);
        if(arg > 0.7)
            temp = PIO2 - atan(temp/arg);
        else
            temp = atan(arg/temp);
        if(sign > 0)
            temp = -temp;
        return temp;
    }


//feel free to implement this w/ recursion
float Math::factorial(int num){
    double fact = 1;
    for(int i = 1; i <= num; i++){
        fact *= i;
    }
    return fact;
}

float Math::pow(float num, float exp){
    if (exp == 0) return 1;
    for (int i = 1; i < exp; ++i){
        num *= num;
    }
   return num;

}

float Math::cosineTaylor(float num, float precision ){
    float value = 0;
    for(int n = 0; n < precision; n++){
        value += pow(-1.0, n) * pow(num, 2*n)/factorial(2*n);
    }
    return value;
}

float Math::fast_log2(float val)
{
   int * const exp_ptr = reinterpret_cast <int *> (&val);
   int x = *exp_ptr;
   const int log_2 = ((x >> 23) & 255) - 128;
   x &= ~(255 << 23);
   x += 127 << 23;
   (*exp_ptr) = x;

   val = ((-1.0f/3) * val + 2) * val - 2.0f/3;

   return (val + log_2);
}

float Math::fast_log(const float &val)
{
    static const float ln_2 = 0.69314718f;
    return (fast_log2(val) * ln_2);
}


//Taylor series cos(x) = (-1)^n*x^(2n)/(2n)!


/*
    What you'll see:

    0.540302                        0.540302
    -0.416147                       -0.416147
    -0.989992                       -0.989992
    -0.653644                       -0.653644
    0.283662                        0.283662
    0.96017                         0.96017
    0.753902                        0.753902
    -0.1455                         -0.1455
    -0.91113                        -0.91113
    -0.839072                       -0.839072

    Requires +1 to precision as 19 would give -0.839071 for the last number
*/






