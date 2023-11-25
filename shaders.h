#pragma once


const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 col;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   int winx =  640 / 4;\n"
"   int winy = 480 / 4;\n"
"   int v_id = gl_VertexID;\n"
"   int idx = ((v_id % winx) - winx / 2);\n"
"   int idy = ((v_id / winx) - winy / 2 );\n"
"   float newPosx = float(idx)/(winx / 2.0);\n"
"   float offsety = float(((v_id - 1) % 2) / float(winy));\n"
"   float newPosy = float(idy)/(winy / 2.0) + offsety  ;\n"


"   gl_Position = vec4(newPosx,newPosy,0,1);\n"
"   ourColor = col;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0f);\n"
"}\n\0";
