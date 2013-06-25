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
	this->acceleration = accel;
}

void Kinematics::updateTime(float amount) {
	this->position += this->velocity*amount;
	this->velocity += this->acceleration*amount;
}
