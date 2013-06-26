#include "Kinematics.h"

Kinematics::Kinematics(float initPos, float initVelo, float initAccel) {
	this->position = initPos;
	this->velocity = initVelo;
	this->acceleration = initAccel;
}

Kinematics::~Kinematics() {

}

float Kinematics::getPos() {
	return this->position;
}

void Kinematics::setAccel(float accel) {
	this->acceleration = accel-10;
}

void Kinematics::updateTime(float amount) {
	this->position += this->velocity*amount + acceleration*amount*amount/2.0f;
	if(this->position < 0)
		this->position = 0;
	this->velocity += this->acceleration*amount;
}
