#include "Predictor.h"

Predictor::Predictor() {
	this->next = 0;
}
Predictor::~Predictor() {

}

//Update predictor with new value
void Predictor::new_value(float value, int time) {
	values[this->next] = value;
	times[this->next] = time;
	this->next = (this->next+1)%3;
}

float Predictor::get_prediction1(int now) {
	int previous = this->next-1;
	if (previous < 0)
		previous += 3;
	float prediction = values[previous];
	new_value(prediction, now);
	return prediction;
}
float Predictor::get_prediction2(int now) {
	int previous = this->next-1;
	if (previous < 0)
		previous += 3;
	int preprevious = this->next-2;
	if (preprevious < 0)
		preprevious += 3;
	float derivative = (values[previous]-values[preprevious])/
				(times[previous]-times[preprevious]);

	float prediction = values[previous] + derivative*(now-times[previous]);
	new_value(prediction, now);
	return prediction;

}

float Predictor::get_prediction3(int now) {
	int previous = this->next-1;
	if (previous < 0)
		previous += 3;
	int preprevious = this->next-2;
	if (preprevious < 0)
		preprevious += 3;
	float d1 = (values[previous]-values[preprevious])/
			(times[previous]-times[preprevious]);

	float d2 = (values[this->next]-values[previous])/
			(times[this->next]-times[previous]);

	float d_predict = (d1-d2)/(times[previous]-times[preprevious]);

	float prediction = values[previous] + d_predict*(now-times[previous]);
	new_value(prediction, now);
	return prediction;

}




