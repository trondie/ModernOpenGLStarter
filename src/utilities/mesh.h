#pragma once

#include <vector>
#include <glm/glm.hpp>

/**
 * A mesh with vectors for vertices, normals, tex coords, and indices
 */ 
class Mesh {

public: 
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> textureCoordinates;

    std::vector<unsigned int> indices;
};