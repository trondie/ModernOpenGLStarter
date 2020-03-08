#pragma once

#include "mesh.h"
#include "imageLoader.hpp"
#include <string>

unsigned int generateBuffer(const Mesh &mesh);
unsigned int generateTextureCoordinateBufferFromMesh(const Mesh & mesh);
PNGImage loadTexture(std::string filename);
unsigned int generateTexture(PNGImage & texture);