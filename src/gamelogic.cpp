#include <chrono>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <utilities/shader.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <utilities/timeutils.h>
#include <utilities/mesh.h>
#include <utilities/shapes.h>
#include <utilities/glutils.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "gamelogic.h"
#include "sceneGraph.hpp"
#include "lodepng.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "utilities/shapes.h"
#include "utilities/shape.h"

/**
 * Basic scenenodes that are to be traversed
 */ 
SceneNode * rootNode;
GeometryNode * squareNode;
SceneNode * pointLightNode;

/**
 * Globals, e.g view and perspection matrices
 */ 
glm::vec3 pointLightPosition(20.0f, 0.0f, 0.0f);
glm::mat4 projection, cameraTransform; 
Starter::Shader * shader; 
const float debug_startTime = 0;
double totalElapsedTime = debug_startTime;

/**
 * Constant for keyboardinputs (see "updateFrame(...)")
 */  

// Translate variables used for sphere 
GLfloat translateX = 0.0f;
GLfloat translateY = 0.0f;
GLfloat translateZ = 0.0f;

// Translate variables used for light source 
GLfloat translateLX = 0.0f;
GLfloat translateLY = 0.0f;
GLfloat translateLZ = 0.0f;

// Rotation variables used for sphere
GLfloat rotateX = 0.0f;
GLfloat rotateY = 0.0f;
GLfloat rotateZ = 0.0f;

GLfloat scaleR = 0.5f;
float translateGain = 0.001f; 

bool hasStarted = true; 

void mouseCallback(GLFWwindow* window, double x, double y) {
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    glViewport(0, 0, windowWidth, windowHeight);
}

/**
 * Initializes nodes and the scenegraph
 */ 
void initGame(GLFWwindow* window) {

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetCursorPosCallback(window, mouseCallback);

    shader = new Starter::Shader();
    shader->createShader(
        "./res/shaders/simple.vert", 
        "./res/shaders/simple.frag"
        );
    shader->activate();

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);


    /**
     * Create a shape(s)
     */ 
    //Shape * sphere = new Sphere(0.5f, 40, 40, true);
    Shape * square = new Square(1.5f, 1.5f);

    /** 
     * Initialize scene nodes 
     */
    rootNode = new EmptyNode();
    squareNode = new GeometryNode(3, square, glm::vec3(0.0, 0.0, -1.0), "./src/textures/brick-color.png"); 
    // These two textures are not used
    squareNode->addNormalmap("./src/textures/brick-normal.png");
    squareNode->addHeightmap("./src/textures/brick-height.png");

    pointLightNode = new PointLightNode(6, pointLightPosition);

    /** 
     * Compose scenegraph (adding a sphere and a light to the root node)
     * Continue to push back other nodes as needed 
     * */
    rootNode->getChildren().push_back(squareNode);
    rootNode->getChildren().push_back(pointLightNode); 
    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, squareNode->getTextureId());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, squareNode->getHeightmapId());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, squareNode->getNormalmapId());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    /** 
     * Initialize perspective matrix and view matrix 
     */
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), float(windowWidth) / float(windowHeight), 0.1f,
                                            260.f);
    glm::mat4 cameraTransform = glm::translate(glm::mat4(1), glm::vec3(0.0, 0, 0));

    /** 
     * We pass the matrices as uniforms to the vertex shader and keep them as "constants" for now 
     */
    glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(cameraTransform));
    glUniformMatrix4fv(5, 1, GL_FALSE, glm::value_ptr(projection)); 

    getTimeDeltaSeconds();
}

/**
 * Recursively parses nodetransforms, and updates connected model transforms in the scenegraph
 */ 
void updateNodeTransformations(SceneNode* node, glm::mat4 transformationThusFar) {

    glm::mat4 transformationMatrix(1.0);

    switch(node->getNodetype()) {
        case GEOMETRY: {

            GeometryNode * geometryNode = static_cast<GeometryNode*>(node); 

            transformationMatrix =
                    glm::translate(glm::mat4(1.0), geometryNode->getPosition())
                    * glm::translate(glm::mat4(1.0), geometryNode->getReferencePoint())
                    * glm::rotate(glm::mat4(1.0), geometryNode->getRotation().z, glm::vec3(0,0,1))
                    * glm::rotate(glm::mat4(1.0), geometryNode->getRotation().y, glm::vec3(0,1,0))
                    * glm::rotate(glm::mat4(1.0), geometryNode->getRotation().x, glm::vec3(1,0,0))
                    * glm::translate(glm::mat4(1.0), -geometryNode->getReferencePoint())
                    * glm::scale(glm::mat4(1.0), geometryNode->getScale());
            
            break;
        }
        case POINT_LIGHT:
            break;
        case SPOT_LIGHT:
            break;
    }

    /**
     * Updates model matrix transform for the current node
     */ 
    node->currentTransformationMatrix = transformationThusFar * transformationMatrix;

    for(SceneNode* child : node->getChildren()) {
        updateNodeTransformations(child, node->currentTransformationMatrix);
    }
}

/**
 * Update framevariables (e.g for input), and update nodetransformations.
 */ 
void updateFrame(GLFWwindow* window) {

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // Use for clock
    double timeDelta = getTimeDeltaSeconds();

    if(!hasStarted) {

        totalElapsedTime = debug_startTime;
        translateLX = pointLightNode->getPosition().x;
        translateLY = pointLightNode->getPosition().y;
        translateLZ = pointLightNode->getPosition().z;
        hasStarted = true; 

    } else {

        squareNode->changeRelativePosition(glm::vec3(
            squareNode->getPosition().x + translateX * translateGain,
            squareNode->getPosition().y + translateY * translateGain,
            squareNode->getPosition().z + translateZ * translateGain)
        );

        totalElapsedTime += timeDelta;

        /**
         * Update node transformations -> update model matrices 
         * for nodes to transform wrt changes in this "frame update"
         */ 
        updateNodeTransformations(rootNode, glm::mat4(1.0));
     
    }

}
/**
 * Parses a scene node, renders data associated with the node, and recursively calls its children passing through the graph
 */ 
void renderNode(SceneNode* node) {
 
    switch(node->getNodetype()) {
        case GEOMETRY: {
            GeometryNode * geometryNode = static_cast<GeometryNode*>(node); 
            if(geometryNode->getVaoId() != -1) { 
                glUniformMatrix4fv(geometryNode->getLocation(), 1, GL_FALSE, glm::value_ptr(geometryNode->currentTransformationMatrix));
                glBindVertexArray(geometryNode->getVaoId());
                glBindTextureUnit(10, geometryNode->getTextureId());
                glDrawElements(GL_TRIANGLES, geometryNode->getVaoIndexCount(), GL_UNSIGNED_INT, nullptr); 
                
            }  
            break;
        }
        case POINT_LIGHT: 
            glUniform3fv(node->getLocation(), 1, glm::value_ptr(node->getPosition()));
            break;
        
        case SPOT_LIGHT: break;
    }

    for(SceneNode* child : node->getChildren()) {
        renderNode(child);
    }
}

void renderFrame(GLFWwindow* window) {
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    glViewport(0, 0, windowWidth, windowHeight);
    renderNode(rootNode);
}

void handleKeyboardInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else if (glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS)
    {
        translateY += 0.50f;
    }
    else if (glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS)
    {
        translateY -= 0.50f;
    }
    else if (glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS)
    {
        translateX += 0.50f;
    }
    else if (glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS)
    {
        translateX -= 0.05f;
    }
    else if (glfwGetKey(window,GLFW_KEY_E) == GLFW_PRESS)
    {
        translateZ += 0.05f;
    }
    else if (glfwGetKey(window,GLFW_KEY_Q) == GLFW_PRESS)
    {
        translateZ -= 0.05f;
    }
    else if (glfwGetKey(window,GLFW_KEY_Z) == GLFW_PRESS)
    {
        scaleR += 0.1f;
    }
    else if (glfwGetKey(window,GLFW_KEY_UP) == GLFW_PRESS)
    {
        rotateX += 3.0f;
    }
    else if (glfwGetKey(window,GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        rotateX -= 3.0f;
    }
    else if (glfwGetKey(window,GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        rotateY += 3.0f;
    }
    else if (glfwGetKey(window,GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        rotateY -= 3.0f;
    }
     else if (glfwGetKey(window,GLFW_KEY_I) == GLFW_PRESS)
    {
        translateLY += 0.05f;
    }
    else if (glfwGetKey(window,GLFW_KEY_J) == GLFW_PRESS)
    {
        translateLX -= 0.05f;
    }
    else if (glfwGetKey(window,GLFW_KEY_K) == GLFW_PRESS)
    {
        translateLY +=0.05f;
    }
    else if (glfwGetKey(window,GLFW_KEY_L) == GLFW_PRESS)
    {
        translateLX += 0.05f;
    }
        else if (glfwGetKey(window,GLFW_KEY_U) == GLFW_PRESS)
    {
        translateLZ -=0.05f;
    }
    else if (glfwGetKey(window,GLFW_KEY_O) == GLFW_PRESS)
    {
        translateLZ += 3.0f;
    }
    else if (glfwGetKey(window,GLFW_KEY_R) == GLFW_PRESS)
    {
        translateX = 0.0f;
        translateY = 0.0f;
        translateZ = 0.0f;
    }
}