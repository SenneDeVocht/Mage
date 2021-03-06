#pragma once

#include <stdio.h>
#include <iostream> // std::cout
#include <sstream>  // stringstream
#include <memory>   // smart pointers
#include <vector>
#include <algorithm>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#pragma warning(push, 0)
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

#include <SDL.h>

#define GLEW_STATIC
#include "glew.h"
#pragma warning (pop)