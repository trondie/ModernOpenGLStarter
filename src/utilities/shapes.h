#pragma once
#include <iostream>
#include "shape.h"

/**
 * A couple of primitive shapes (Square, Box, Spehere)
 */ 
class Square : public Shape {

private:    
    float width, height;

public:

    Square(float width, float height) : width(width), height(height) { 
        generateMesh(); 
    }

    Square(float width, float height, bool interception) : width(width), height(height) { 
        this->interception = interception; 
        generateMesh(); 
    }

    virtual bool intercept(const glm::vec3 shapePos, const glm::vec3 objectPos) {
        if (!interception) {
            return false; 
        }
        if ( (objectPos.x < (shapePos.x + width))
            && (objectPos.x > (shapePos.x)) ) {
                if ( (objectPos.y < (shapePos.y + height))
                    && (objectPos.y > (shapePos.y))){
                        return true; 
                    }
            }
            return false; 
    }

    virtual void generateMesh() {

        this->mesh.vertices = {
            {0, 0, 0},
            {width, 0, 0}, 
            {width, height, 0}, 
            {0, height, 0}
        };

        this->mesh.normals = {
            {0.0, 0.0, 1.0},
            {0.0, 0.0, 1.0},
            {0.0, 0.0, 1.0},
            {0.0, 0.0, 1.0}
        };

        this->mesh.textureCoordinates = {
            {0, 0},
            {1, 0},
            {1, 1},
            {0, 1} 
        };

        this->mesh.indices = {
            0,1,2,0,2,3
        };
    }
};

class Box : public Shape {

private:
    float width, height, depth; 
    bool flipFaces;

public: 
    Box(float width, float height, float depth) : width(width), height(height), depth(depth) {
        generateMesh(); 
    }
    Box(float width, float height, float depth, bool interception) : width(width), height(height), depth(depth) {
        this->interception = interception; 
        generateMesh(); 
    }
    
protected:
 
    virtual bool intercept(const glm::vec3 shapePos, const glm::vec3 objectPos) {
        if (!interception) {
            return false; 
        }
        if ( (objectPos.x < (shapePos.x + width))
            && (objectPos.x > (shapePos.x))) {
                if ( (objectPos.y < (shapePos.y + height))
                    && (objectPos.y > (shapePos.y))){
                        return true; 
                    }
            }
            return false; 
    }

    virtual void generateMesh() {

    this->mesh.vertices = {
            {0,     0,      0},
            {0,     0,      depth},
            {0,     height, depth},

            {0,     0,      0},
            {0,     height, depth},
            {0,     height, 0},

            {width, 0,      0},
            {width, height, depth},
            {width, 0,      depth},

            {width, 0,      0},
            {width, height, 0},
            {width, height, depth},

            {0,     0,      0},
            {width, height, 0},
            {width, 0,      0},

            {0,     0,      0},
            {0,     height, 0},
            {width, height, 0},

            {0,     0,      depth},
            {width, 0,      depth},
            {width, height, depth},

            {0,     0,      depth},
            {width, height, depth},
            {0,     height, depth},

            {0,     0,      0},
            {width, 0,      0},
            {width, 0,      depth},

            {0,     0,      0},
            {width, 0,      depth},
            {0,     0,      depth},

            {width, height, 0},
            {0,     height, 0},
            {0,     height, depth},

            {width, height, 0},
            {0,     height, depth},
            {width, height, depth},
    };

    // These are technically inverted relative to the vertex coordinates.
    // But for some strange reason the faces are rendered inverted.
    // So to make the assignment work this is the best I can do.

    this->mesh.normals = {
            {1.0, 0.0, 0.0},
            {1.0, 0.0, 0.0},
            {1.0, 0.0, 0.0},

            {1.0, 0.0, 0.0},
            {1.0, 0.0, 0.0},
            {1.0, 0.0, 0.0},

            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},

            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},

            {0.0, 0.0, 1.0},
            {0.0, 0.0, 1.0},
            {0.0, 0.0, 1.0},

            {0.0, 0.0, 1.0},
            {0.0, 0.0, 1.0},
            {0.0, 0.0, 1.0},

            {0.0, 0.0, -1.0},
            {0.0, 0.0, -1.0},
            {0.0, 0.0, -1.0},

            {0.0, 0.0, -1.0},
            {0.0, 0.0, -1.0},
            {0.0, 0.0, -1.0},

            {0.0, 1.0, 0.0},
            {0.0, 1.0, 0.0},
            {0.0, 1.0, 0.0},

            {0.0, 1.0, 0.0},
            {0.0, 1.0, 0.0},
            {0.0, 1.0, 0.0},

            {0.0, -1.0, 0.0},
            {0.0, -1.0, 0.0},
            {0.0, -1.0, 0.0},

            {0.0, -1.0, 0.0},
            {0.0, -1.0, 0.0},
            {0.0, -1.0, 0.0},
    };

    float texScaleFactorX = depth / height;
    float texScaleFactorY = width / depth;
    float texScaleFactorZ = width / height;
    
    this->mesh.textureCoordinates = {
            {0, 0},
            {texScaleFactorX, 0},
            {texScaleFactorX, 1},

            {0, 0},
            {texScaleFactorX, 1},
            {0, 1},

            {0, 0},
            {texScaleFactorX, 1},
            {texScaleFactorX, 0},

            {0, 0},
            {0, 1},
            {texScaleFactorX, 1},

            {0, 0},
            {texScaleFactorZ, 0},
            {texScaleFactorZ, 1},

            {0, 0},
            {texScaleFactorZ, 1},
            {0, 1},

            {0, 0},
            {texScaleFactorZ, 0},
            {texScaleFactorZ, 1},

            {0, 0},
            {texScaleFactorZ, 1},
            {0, 1},

            {0, 0},
            {texScaleFactorY, 0},
            {texScaleFactorY, 1},

            {0, 0},
            {texScaleFactorY, 1},
            {0, 1},

            {0, 0},
            {texScaleFactorY, 0},
            {texScaleFactorY, 1},

            {0, 0},
            {texScaleFactorY, 1},
            {0, 1},

    };


    this->mesh.indices = {
            0, 1, 2,
            3, 4, 5,
            6, 7, 8,
            9, 10, 11,
            12, 13, 14,
            15, 16, 17,
            18, 19, 20,
            21, 22, 23,
            24, 25, 26,
            27, 28, 29,
            30, 31, 32,
            33, 34, 35
    };

    if(flipFaces) {
        for(int i = 0; i < 36; i += 3) {
            unsigned int temp = this->mesh.indices[i + 1];
            this->mesh.indices[i + 1] = this->mesh.indices[i + 2];
            this->mesh.indices[i + 2] = temp;

            this->mesh.normals[i + 0] *= -1;
            this->mesh.normals[i + 1] *= -1;
            this->mesh.normals[i + 2] *= -1;
        }
    }

    }
};

class Sphere : public Shape {
    
    private:

        float sphereRadius; 
        int slices; 
        int layers; 

    public:

        Sphere(float sphereRadius, int slices, int layers, bool interception) : sphereRadius(sphereRadius), slices(slices), layers(layers){
            this->interception = interception; 
            generateMesh();
        }

        virtual bool intercept(const glm::vec3 shapePos, const glm::vec3 objectPos) {
            if (!interception) {
                return false; 
            }
            if (glm::abs(objectPos.x - shapePos.x) < sphereRadius) {
                    if ( (abs(objectPos.y - shapePos.y) < sphereRadius))
                    {
                        return true; 
                    }
                }
                return false; 
        }

        virtual void generateMesh() {

            const unsigned int triangleCount = slices * layers * 2;

            std::vector<glm::vec3> vertices;
            std::vector<glm::vec3> normals;
            std::vector<unsigned int> indices;

            vertices.reserve(3 * triangleCount);
            normals.reserve(3 * triangleCount);
            indices.reserve(3 * triangleCount);

            const float degreesPerLayer = 180.0 / (float) layers;
            const float degreesPerSlice = 360.0 / (float) slices;

            unsigned int i = 0;

            for (int layer = 0; layer < layers; layer++) {
                int nextLayer = layer + 1;
                float currentAngleZDegrees = degreesPerLayer * layer;
                float nextAngleZDegrees = degreesPerLayer * nextLayer;

                float currentZ = -cos(glm::radians(currentAngleZDegrees));
                float nextZ = -cos(glm::radians(nextAngleZDegrees));

                float radius = sin(glm::radians(currentAngleZDegrees));
                float nextRadius = sin(glm::radians(nextAngleZDegrees));

                for (int slice = 0; slice < slices; slice++) {

                    // The direction of the start and the end of the slice in the xy-plane
                    float currentSliceAngleDegrees = slice * degreesPerSlice;
                    float nextSliceAngleDegrees = (slice + 1) * degreesPerSlice;

                    // Determining the direction vector for both the start and end of the slice
                    float currentDirectionX = cos(glm::radians(currentSliceAngleDegrees));
                    float currentDirectionY = sin(glm::radians(currentSliceAngleDegrees));

                    float nextDirectionX = cos(glm::radians(nextSliceAngleDegrees));
                    float nextDirectionY = sin(glm::radians(nextSliceAngleDegrees));

                    this->mesh.vertices.emplace_back(sphereRadius * radius * currentDirectionX,
                                        sphereRadius * radius * currentDirectionY,
                                        sphereRadius * currentZ);
                    this->mesh.vertices.emplace_back(sphereRadius * radius * nextDirectionX,
                                        sphereRadius * radius * nextDirectionY,
                                        sphereRadius * currentZ);
                    this->mesh.vertices.emplace_back(sphereRadius * nextRadius * nextDirectionX,
                                        sphereRadius * nextRadius * nextDirectionY,
                                        sphereRadius * nextZ);
                    this->mesh.vertices.emplace_back(sphereRadius * radius * currentDirectionX,
                                        sphereRadius * radius * currentDirectionY,
                                        sphereRadius * currentZ);
                    this->mesh.vertices.emplace_back(sphereRadius * nextRadius * nextDirectionX,
                                        sphereRadius * nextRadius * nextDirectionY,
                                        sphereRadius * nextZ);
                    this->mesh.vertices.emplace_back(sphereRadius * nextRadius * currentDirectionX,
                                        sphereRadius * nextRadius * currentDirectionY,
                                        sphereRadius * nextZ);

                    this->mesh.normals.emplace_back(radius * currentDirectionX,
                                        radius * currentDirectionY,
                                        currentZ);
                    this->mesh.normals.emplace_back(radius * nextDirectionX,
                                        radius * nextDirectionY,
                                        currentZ);
                    this->mesh.normals.emplace_back(nextRadius * nextDirectionX,
                                        nextRadius * nextDirectionY,
                                        nextZ);
                    this->mesh.normals.emplace_back(radius * currentDirectionX,
                                        radius * currentDirectionY,
                                        currentZ);
                    this->mesh.normals.emplace_back(nextRadius * nextDirectionX,
                                        nextRadius * nextDirectionY,
                                        nextZ);
                    this->mesh.normals.emplace_back(nextRadius * currentDirectionX,
                                        nextRadius * currentDirectionY,
                                        nextZ);

                    this->mesh.indices.emplace_back(i + 0);
                    this->mesh.indices.emplace_back(i + 1);
                    this->mesh.indices.emplace_back(i + 2);
                    this->mesh.indices.emplace_back(i + 3);
                    this->mesh.indices.emplace_back(i + 4);
                    this->mesh.indices.emplace_back(i + 5);

                    i += 6;
                }
            }

        }
};


