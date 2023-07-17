#include "GameElement.h"


void GameElement::setHitboxWidth(double width) {
    hitboxWidth = width;
}

void GameElement::setHitboxHeight(double height) {
    hitboxHeight = height;
}

void GameElement::setOx(double translateOx) {
    ox = translateOx;
}

void GameElement::setOy(double translateOy) {
    oy = translateOy;
}

void GameElement::setCrtOxHitbox(double translateOx) {
    crtOxHitbox = translateOx;
}

void GameElement::setCrtOyHitbox(double translateOy) {
    crtOyHitbox = translateOy;
}

void GameElement::setOxHitbox(double translateOx) {
    oxHitbox = translateOx;
}

void GameElement::setOyHitbox(double translateOy) {
    oyHitbox = translateOy;
}

void GameElement::setOxTranslate(double translateOx) {
    oxTranslate = translateOx;
}

void GameElement::setOyTranslate(double translateOy) {
    oyTranslate = translateOy;
}


double GameElement::getHitboxWidth() {
    return hitboxWidth;
}

double GameElement::getHitboxHeight() {
    return hitboxHeight;
}

double GameElement::getOx() {
    return ox;
}

double GameElement::getOy() {
    return oy;
}

double GameElement::getOxHitbox() {
    return oxHitbox;
}

double GameElement::getOyHitbox() {
    return oyHitbox;
}

double GameElement::getOxTranslate() {
    return oxTranslate;
}

double GameElement::getOyTranslate() {
    return oyTranslate;
}

double GameElement::getCrtOxHitbox() {
    return crtOxHitbox;
}

double GameElement::getCrtOyHitbox() {
    return crtOyHitbox;
}