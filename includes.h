#pragma once


//Includes
#include <iostream>

//opengl
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//array manipulation and such
#include <queue>
#include <vector>
#include <string>
#include <algorithm>                  

#include <chrono>

//math libraries
#include <limits>
#include <cstdlib>     
#include <cmath>

//file and data manipulation and saving
#include <thread>
#include <fstream>



//MAGIC NUMBERS
//infinity for floats 
const std::string VERSION = "0.0.4.3";
const float INF = std::numeric_limits<float>::infinity();
const double PI = std::atan(1.0) * 4;

// SETTINGS
//screen size
const unsigned int SCR_WIDTH = 640;
const unsigned int SCR_HEIGHT = 480;

//seed data
const int SEED = 0;
