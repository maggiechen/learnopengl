#pragma once
// this is a macro that stb_image.h uses to preprocess itself into only the relevant source code
// it effectively turns it from a .h to a .cpp file
#define STB_IMAGE_IMPLEMENTATION

// I have to prepend "stb/" because that's the folder inside of the
// include directory I specified for this project
#include "stb/stb_image.h"