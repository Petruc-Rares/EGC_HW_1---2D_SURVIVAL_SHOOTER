#pragma once
#include "GameElement.h"

class Enemy : public GameElement
{
	private:
		double thetaEnemy;
		double speed;

	public:
		void setThetaEnemy(double thetaEnemy);
		void setSpeed(double speed);

		double getThetaEnemy();
		double getSpeed();
};