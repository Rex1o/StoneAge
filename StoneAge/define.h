#ifndef DEFINE_H__
#define DEFINE_H__

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#ifdef _WIN32
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#else
#include <GL/glew.h>
#endif

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 128
#define CHUNK_SIZE_Z 16

typedef uint8_t BlockType;
// BTYPE_LAST ==> Toujours mettre cette valeur d'enum
// à la fin pour savoir le nombre d'élément dans un enum en CPP
enum BLOCK_TYPE { BTYPE_AIR, BTYPE_DIRT, BTYPE_GRASS, BTYPE_STONE, BTYPE_SAND, BTYPE_WOOD, BTYPE_BEDROCK, BTYPE_SNOW, BTYPE_LAST };

#define SHADER_PATH "../StoneAge/media/shaders/"
#define TEXTURE_PATH "../StoneAge/media/textures/"
#define VIEW_DISTANCE 128
#define FIELD_OF_VIEW 45
#define SPRINT_FIELD_OF_VIEW 10
#define OCEAN_HEIGHT 60
// Pour que la méthode fonctionne
#define MAX_SELECTION_DISTANCE 6

#endif // DEFINE_H__
