#include "lab_m1/tema1/objects.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

Mesh* Objects::CreateSquare(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* Objects::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float height,
    float width,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(height, 0, 0), color),
        VertexFormat(corner + glm::vec3(height, width, 0), color),
        VertexFormat(corner + glm::vec3(0, width, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}


Mesh* Objects::CreateCircle(
    const std::string& name,
    glm::vec3 centre,
    float radius,
    int number_of_sides,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 circle_centre = centre;

    std::vector<VertexFormat> vertices;
    vertices.push_back(VertexFormat(circle_centre, color));
    std::vector<unsigned int> indices;

    for (int i = 0; i < number_of_sides; i++) {
        vertices.push_back(VertexFormat(glm::vec3(
            (circle_centre.x + radius * cos(i * M_PI * 2 / number_of_sides)),
            (circle_centre.y + radius * sin(i * M_PI * 2 / number_of_sides)),
            0), color));
        indices.push_back(i);
    }

    Mesh* circle = new Mesh(name);

    indices.push_back(1);
    indices.push_back(number_of_sides);


    if (!fill) {
        circle->SetDrawMode(GL_TRIANGLE_FAN);
    }


    circle->InitFromData(vertices, indices);
    return circle;
}
