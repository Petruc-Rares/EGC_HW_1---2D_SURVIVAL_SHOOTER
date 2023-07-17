#include "Enemy.h"

void Enemy::setThetaEnemy(double thetaEnemy) {
	this->thetaEnemy = thetaEnemy;
}

void Enemy::setSpeed(double speed) {
	this->speed= speed;
}


double Enemy::getThetaEnemy() {
	return thetaEnemy;
}

double Enemy::getSpeed() {
	return speed;
}
