#pragma once

// this is a macro that stb_image.h uses to preprocess itself into only the relevant source code
// it effectively turns it from a .h to a .cpp file
// this must be done in a .cpp file and not a .h file or you will get linker errors. To use the implementation, include
// this file (StbImageEnabler.cpp) where you need it (in cpp files, not h files), or you will also get linker errors
#define STB_IMAGE_IMPLEMENTATION

// I have to prepend "stb/" because that's the folder inside of the
// include directory I specified for this project
#include <stb/stb_image.h>