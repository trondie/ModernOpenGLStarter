#include "program.hpp"
#include "utilities/window.hpp"
#include "gamelogic.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <utilities/shapes.h>
#include <utilities/glutils.h>
#include <utilities/shader.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utilities/timeutils.h>

void runProgram(GLFWwindow* window)
{
    // Z buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

    /** Init before rendering loop */ 
	initGame(window);

    /**
     * Rendering loop
     */ 
    while (!glfwWindowShouldClose(window))
    {
	    // Clear colour and depth buffers
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        updateFrame(window);
        renderFrame(window);

        glfwPollEvents();
        handleKeyboardInput(window);
        glfwSwapBuffers(window);
    }
}



