#include "Proiectile.h"

void Proiectile::setRadius(float radius) {
    this->radius = radius;
}

void Proiectile::setThetaCursor(double thetaCursor) {
    this->thetaCursor = thetaCursor;
}

void Proiectile::setSpeed(double speed) {
    this->speed = speed;
}

void Proiectile::setDistance(double distance) {
    this->distance = distance;
}

float Proiectile::getRadius() {
    return radius;
}

double Proiectile::getThetaCursor() {
    return thetaCursor;
}

double Proiectile::getSpeed() {
    return speed;
}

double Proiectile::getDistance() {
    return distance;
}