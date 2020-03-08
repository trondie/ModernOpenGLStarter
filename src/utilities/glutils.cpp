#include <glad/glad.h>
#include <program.hpp>
#include "glutils.h"

/**
 * Layout locations for the buffers. Needs to be maintained wrt to the shaders. 
 */
static const int VERTEX_BUFFER_LAYOUT_LOCATION = 0; 
static const int NORMAL_LAYOUT_LOCATION = 1; 
static const int TEXCOORD_LAYOUT_LOCATION = 2; 
static const int TANGENT_LAYOUT_LOCATION = 11;
static const int BITANGENT_LAYOUT_LOCATION = 12;

int vertexAttributeArray = 0;

/**
 * Generates buffers and transfers buffers for vertices, normals, tangents, and bitangent to GPU.
 * 
 */ 
unsigned int generateBuffer(const Mesh &mesh) {
    
    unsigned int vaoID;
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    unsigned int vertexBufferID;
    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(glm::vec3), mesh.vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(VERTEX_BUFFER_LAYOUT_LOCATION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(vertexAttributeArray++);

    unsigned int normalBufferID;
    glGenBuffers(1, &normalBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
    glBufferData(GL_ARRAY_BUFFER, mesh.normals.size() * sizeof(glm::vec3), mesh.normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(NORMAL_LAYOUT_LOCATION, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(vertexAttributeArray++);

    if (mesh.tangents.size() > 0) {
        unsigned int tangentBufferID;
        glGenBuffers(1, &tangentBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, tangentBufferID);
        glBufferData(GL_ARRAY_BUFFER, mesh.tangents.size() * sizeof(glm::vec3), mesh.tangents.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(TANGENT_LAYOUT_LOCATION, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), 0);
        glEnableVertexAttribArray(vertexAttributeArray++);
    }

    if (mesh.bitangents.size() > 0) {
        unsigned int biTangentBufferID;
        glGenBuffers(1, &biTangentBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, biTangentBufferID);
        glBufferData(GL_ARRAY_BUFFER, mesh.bitangents.size() * sizeof(glm::vec3), mesh.bitangents.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(BITANGENT_LAYOUT_LOCATION, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), 0);
        glEnableVertexAttribArray(vertexAttributeArray++);
    }

    unsigned int indexBufferID;
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), mesh.indices.data(), GL_STATIC_DRAW);

    return vaoID;
}

/**
 * Generates an array buffer and transfers texcoord data from mesh.
 * See Scenegraph for how texture data is generated (this buffer is created with the current bound VAO).
 */ 
unsigned int generateTextureCoordinateBufferFromMesh(const Mesh & mesh) {

    unsigned int textureBufferID;
    glGenBuffers(1, &textureBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, textureBufferID);
    glBufferData(GL_ARRAY_BUFFER, mesh.textureCoordinates.size() * sizeof(glm::vec2), mesh.textureCoordinates.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(TEXCOORD_LAYOUT_LOCATION, 2, GL_FLOAT, GL_FALSE, 0 * sizeof(float), 0);
    glEnableVertexAttribArray(vertexAttributeArray++);
    return textureBufferID; 
}

PNGImage loadTexture(std::string filename) {
    return loadPNGFile(filename); 
}

unsigned int generateTexture(PNGImage & texture) {
    
    unsigned int textureId; 
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &textureId);
   // glActiveTexture(GL_TEXTURE0);        
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.pixels.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);  
    
    return textureId; 
}
    
