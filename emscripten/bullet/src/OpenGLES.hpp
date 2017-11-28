
#pragma once

// #ifndef __EMSCRIPTEN__
// #   define USE_GLEW 1
// #endif
//
// #if USE_GLEW
// #   include "GL/glew.h"
// #endif
//
// #if !USE_GLEW
#   define GL_GLEXT_PROTOTYPES 1
#   include <SDL2/SDL.h>
// #   include "SDL2/SDL_opengles2.h"
#   include <SDL2/SDL_opengl.h>
// #endif
