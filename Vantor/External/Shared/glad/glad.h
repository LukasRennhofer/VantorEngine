// This file is used for abstracting the glad loaders 

#if defined(__WINDOWS__)
#include "../../Windows-glad/glad.h"
#elif defined(__LINUX__)
#include "../../Linux/linux-glad/glad.h"
#else
#error "Unsupported platform for glad include"
#endif