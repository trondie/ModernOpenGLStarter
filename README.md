# OpenGL Starter

Basic OpenGL setup for adding shapes, textures, and VAOs, and for composing a scenegraph out of them for rendering.

## Installation
Make sure to have :
```bash
sudo apt install libopenal-dev libvorbis-dev libflac-dev xorg-dev
```
When cloning, make sure to use the --recursive flag for the submodules under /lib, or do this after : 
```bash
git submodule update --init
```
Run the glad.sh script under /lib
```bash
sh ./glad.sh
```
Run CMake in the root directory and cross your fingers! 

```bash
cmake .
```
```bash
make
```
![Sphere](screenshot.png)
