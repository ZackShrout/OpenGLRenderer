#pragma once

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // !STD_IMAGE_IMPLEMENTATION

// C/C++ Standard
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <fstream>
#include <cassert>
#include <stdio.h>

// OpenGL related
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

constexpr unsigned int MAX_POINT_LIGHTS{ 3 };
constexpr unsigned int MAX_SPOT_LIGHTS{ 3 };