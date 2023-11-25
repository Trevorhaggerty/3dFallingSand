#pragma once


//Includes
#include <iostream>

//opengl
//graphics engine
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//array manipulation
#include <queue>
#include <vector>
#include <string>
#include <algorithm>

//time management
#include <chrono>

//math libraries
#include <limits>
#include <cstdlib>     
#include <cmath>

//file and data manipulation and saving
#include <thread>
#include <fstream>
#include <iostream>
#include <fstream>
#include <sstream>

const int RDIM = 12;

//MAGIC NUMBERS

//version number
const std::string VERSION = "0.0.6.4";
//infinity for floats 
const float INF = std::numeric_limits<float>::infinity();
//pi for everything pi related
const double PI = std::atan(1.0) * 4;

// SETTINGS
//screen size
const unsigned int SCR_WIDTH = 640;
const unsigned int SCR_HEIGHT = 480;

//seed data
const int SEED = 0;

//these need to remain untouched
const int PIXELCOUNT = 19200;
const int PIXELCOUNT3 = 57600;

//debuging and logging information
// 0 shows no logs
// 1 through 10 shows logs with priority lower than it
const int DEBUGVALUE = 10;

