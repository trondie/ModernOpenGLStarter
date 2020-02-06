#pragma once


#include <utilities/window.hpp>

/**
 * Initilize scenegraph 
 */ 
void initGame(GLFWwindow* window);

/**
 * Update frame before each render 
 */ 
void updateFrame(GLFWwindow* window);

/**
 * Render frame (used when parsing scenegraph for each pass)
 */  
void renderFrame(GLFWwindow* window);

/**
 * For handling keyboard or mouse input in the window
 */ 
void handleKeyboardInput(GLFWwindow* window);
