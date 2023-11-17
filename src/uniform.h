#pragma once
#include <glm/glm.hpp>

struct Uniform {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 viewport;
    int shader;  // Agrega un miembro para almacenar información sobre el shader
};
