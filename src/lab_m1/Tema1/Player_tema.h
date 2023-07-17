#pragma once
#include "GameElement.h"
#include <glm/glm.hpp>

class Player_tema : public GameElement
{
private:
    float radius;
    glm::vec3 center;

public:
    // Setter
    void setRadius(float radius);
    void setCenter(glm::vec3 center);

    // Getter
    float getRadius();
    glm::vec3 getCenter();
};



