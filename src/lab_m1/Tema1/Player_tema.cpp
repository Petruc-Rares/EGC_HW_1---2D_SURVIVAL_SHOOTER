#include "Player_tema.h"

void Player_tema::setRadius(float radius) {
    this->radius = radius;
}

void Player_tema::setCenter(glm::vec3 center) {
    this->center = center;
}

float Player_tema::getRadius() {
    return radius;
}

glm::vec3 Player_tema::getCenter() {
    return center;
}