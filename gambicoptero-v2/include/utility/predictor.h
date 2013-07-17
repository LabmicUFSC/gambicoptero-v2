#ifndef PREDICTOR_H_
#define PREDICTOR_H_

class Predictor
{
public:
  Predictor();
  ~Predictor();

  //Update predictor with new value
  void new_value(float value, int time);

  float get_prediction1(int now);
  float get_prediction2(int now);
  
  /* pairs (value, time) */
  float values[3];
  float times[3];
  int next;
};

#endif