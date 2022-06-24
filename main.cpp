/*

Un-named Falling Voxel game
---------------------------
written by Trevor Haggerty
Version 0.0.1.1
*/

//includes
#include "includes.h"
//local headers
#include "voxfuncs.h"
#include "substance.h"
#include "gamemem.h"
#include "world.h"
#include "voxels.h"
#include "chunk.h"

//namespaces
using namespace Game;

//TODO MOVE COLORS TO SHADER
//colors to be used
float basecolor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
float sandcol[] = { 0.50f, 0.50f, 0.0f, 0.0f };
float watercol[] = { 0.0f, 0.0f, 0.50f, 0.0f };


//TODO MAKE DICTIONARY TO HOLD SUBSTANCES
//Substances for voxel manipulation and representation
// (name, lo phase, hi phase, lo temp, hi temp, color, density, current phase)
substance* air = new substance("air", NULL, NULL, -INF, INF, basecolor, 0.001f, "gas");
substance* sand = new substance("snd", NULL, NULL, -INF, 0.0f, sandcol, 1.5f, "powder");

substance* steam = new substance("stm", NULL, NULL, 100.0f, INF, basecolor, 0.0005f, "gas");
substance* ice = new substance("ice", NULL, NULL, -INF, 0.0f, basecolor, 0.9f, "solid");
substance* water = new substance("wtr", ice, steam, 0.0f, 100.0f, watercol, 1.0f, "liquid");





void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int processInput(GLFWwindow* window, loc3d &gameCursor);


//TODO MOVE SHADERS TO SEPERATE FILE
//shaders
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0f);\n"
"}\n\0";



//which save file to use
int gameNum = createNewSave();

// ints to hold current screen size
int screen_width, screen_height;

//location of player cursor
loc3d gameCursor = {0,0,0};


int main()
{

    // glfw: initialize and configure
    // ------------------------------
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    

    
    glfwMakeContextCurrent(window);
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[7680] = {
        // positions   // colors
         0.0f, -0.0f,  0.0f, 0.0f, 0.0f, 

    };




    std::srand(SEED);


    chunk A;

   
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
   
   

    
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);

    // as we only have a single shader, we could also just activate our shader once beforehand if we want to 
    glUseProgram(shaderProgram);

    
    




    int timeVar = 0;
    int state = 0;

    while (!glfwWindowShouldClose(window))
    {
        // input
        state = processInput(window, gameCursor);
        timeVar++;

        //update the vertices here
       
        A.Simulate();
        A.DrawVoxels(vertices);
       
        if (state == 1) {
            A.Insert(voxel(sand, 75.0f, false), gameCursor.x, gameCursor.y, gameCursor.z);
        }
        if (state == 2) {
            A.Insert(voxel(water, 75.0f, false), gameCursor.x, gameCursor.y, gameCursor.z);
        }
        if (state == 3) {
            A.Insert(voxel(air, 75.0f, false), gameCursor.x, gameCursor.y, gameCursor.z);
        }
        if (state == 4) {
            std::string loc = "Saves/" + std::to_string(gameNum) + "/chunks/";
            A.Save(loc, 0, 0, 0);
        }
        int maxIndex = *(&vertices) - vertices;
        
//cout << gameCursor;


        


        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);



        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwGetWindowSize(window,&screen_width,&screen_height);
        glPointSize(4);

        // render the triangle
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, 120000);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
int processInput(GLFWwindow* window, loc3d &gameCursor)
{
    int result = 0;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        result = 1;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        result = 2;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        result = 3;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        result = 4;
    if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
        if (gameCursor.y > 0 )
            gameCursor.y--;
    if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
        if (gameCursor.y < 12 - 1)
            gameCursor.y++;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        if (gameCursor.z > 0)
            gameCursor.z--;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        if (gameCursor.z < 12 - 1)
            gameCursor.z++;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        if (gameCursor.x > 0)
            gameCursor.x--;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        if (gameCursor.x < 12 - 1)
            gameCursor.x++;
    return result;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}









