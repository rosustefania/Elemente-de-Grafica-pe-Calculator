#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace Objects
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateRectangle(const std::string& name, glm::vec3 leftBottomCorner, float height, float width, glm::vec3 color, bool fill = false);
    Mesh* CreateCircle(const std::string& name, glm::vec3 centre, float radius, int number_of_sides, glm::vec3 color, bool fill = false);

}