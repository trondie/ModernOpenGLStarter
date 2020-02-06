#pragma once
#include <iostream>
#include "mesh.h"

class Shape {
    
    protected:
        Mesh mesh;   
        virtual void generateMesh() = 0; 
        bool interception = true; 
        
    public:
        Shape() {}
        virtual bool intercept(const glm::vec3 shapePos, const glm::vec3 objectPos) = 0; 

        const Mesh * getMesh() const { 
            return &mesh; 
        }
};