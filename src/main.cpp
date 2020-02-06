
#include "utilities/window.hpp"
#include "program.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <arrrgh.hpp>


static void glfwErrorCallback(int error, const char *description)
{
    fprintf(stderr, "GLFW returned an error:\n\t%s (%i)\n", description, error);
}
GLFWwindow* initialise()
{
    if (!glfwInit())
    {
        fprintf(stderr, "Could not start GLFW\n");
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwSetErrorCallback(glfwErrorCallback);
    glfwWindowHint(GLFW_RESIZABLE, windowResizable);
    glfwWindowHint(GLFW_SAMPLES, windowSamples);  

    GLFWwindow* window = glfwCreateWindow(windowWidth,
                                          windowHeight,
                                          windowTitle.c_str(),
                                          nullptr,
                                          nullptr);
    if (!window)
    {
        fprintf(stderr, "Could not open GLFW window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    //Glad
    gladLoadGL();

    printf("%s: %s\n", glGetString(GL_VENDOR), glGetString(GL_RENDERER));
    printf("GLFW\t %s\n", glfwGetVersionString());
    printf("OpenGL\t %s\n", glGetString(GL_VERSION));
    printf("GLSL\t %s\n\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    return window;
}
int main(int argc, const char* argb[])
{
    GLFWwindow* window = initialise();
    runProgram(window);
    glfwTerminate();
    return 0;
}
