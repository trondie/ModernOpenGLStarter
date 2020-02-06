#include "imageLoader.hpp"
#include <iostream>

PNGImage loadPNGFile(std::string fileName)
{
	std::vector<unsigned char> png;
	std::vector<unsigned char> pixels;
	unsigned int width, height;

	unsigned error = lodepng::load_file(png, fileName);
	if(!error) {
		error = lodepng::decode(pixels, width, height, png);
	}

	unsigned int widthBytes = 4 * width;

	for(unsigned int row = 0; row < (height / 2); row++) {
		for(unsigned int col = 0; col < widthBytes; col++) {
			std::swap(pixels[row * widthBytes + col], pixels[(height - 1 - row) * widthBytes + col]);
		}
	}

	PNGImage image;
	image.width = width;
	image.height = height;
	image.pixels = pixels;

	return image;

}
