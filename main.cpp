/*

Un-named Falling Voxel game
---------------------------
written by Trevor Haggerty
Version 0.0.3.2
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
#include "logger.h"

//namespaces
using namespace Game;
using namespace logspace;

//logging object
Logger logger("Global");



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
"layout (location = 0) in vec3 col;\n"
//"layout (location = 1) in int colman;\n"

"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   int id = gl_VertexID;\n"
"   int idx = ((id % 160)-80);\n"
"   int idy = ((id / 160)-60 );\n"
"   float newPosx = float(idx)/80.0;\n"
"   float newPosy = float(idy)/60.0 + (id % 2)/120.0;\n"
"   float newPos = float(id)/1900;\n"


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






int main()
{   
    logger.log(1, "PRGM START", "main func started");
    logger.updateLocation("Main");

    //which save file to use
    int gameNum = 0;
    //gameNum = createNewSave();
    logger.log(1, "NEW SAVE", "save located at " + std::to_string(gameNum));

    // ints to hold current screen size
    int screen_width, screen_height;

    //location of player cursor
    loc3d gameCursor = {0,0,0};
    logger.log(1, "CURSOR MADE", "size of obj " + std::to_string(sizeof(gameCursor)) + " bytes");

    substance substances[255];
    substances[0] = substance("air", NULL, NULL, -INF, INF, basecolor, 0, "gas");
    substances[1] = substance("snd", NULL, NULL, -INF, 0.0f, sandcol, 1.5f, "powder");
    substances[2] = substance("wtr", ice, steam, 0.0f, 100.0f, watercol, 1.0f, "liquid");
    
    logger.log(1, "SUBSTS MADE", "size of obj " + std::to_string(sizeof(substances)) + " bytes");

    // glfw: initialize and configure
    // ------------------------------
    
    logger.log(1, "CREATING WINDOW", " version 3.3 glfw window");
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Falling Voxels", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    else {
        logger.log(1, "WINDOW CREATED", std::to_string(SCR_WIDTH)+"X"+ std::to_string(SCR_HEIGHT));
    }
    

    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    else {
        logger.log(1, "INIT GLAD", "glad loaded");
    }

    //build shaders
    logger.log(1, "BUILDING VERT SHADER", "...");
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
    else {
        logger.log(1, "VERT SHADER BUILT", "vertex shader compiling complete");
    }
    
    
    // fragment shader
    // may go un-used and cut later on
    logger.log(1, "BUILDING FRAG SHADER", "compiling fragment shader");

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
    else {
        logger.log(1, "FRAG SHADER BUILT", "fragment shader compiling complete");
    }
    
    logger.log(1, "LINKING SHADERS", "...");
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
    else {
        logger.log(1, "SHADERS LINKED", "vertex and fragment shaders linked");
    }

    //cleanup
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    
    //vertices that will be drawn to screen
    //TODO ABSTRACT THE COLORS FOR VERTEX SHADER, WILL POTENTIALLY REDUCE VERTICES ARRAY SIZE
    //TODO MAKE TEXT FOR MENUS AND SUCH
    //??TODO?? USE FRAGMENT SHADER TO CREATE EFFECTS
    float vertices[57600];
    //update the vertices here

    
    for (int i = 0; i < 19200; i++) {
        if (i < 160) {
            vertices[i * 3] = 0;
            vertices[i * 3 + 1] = 1;
            vertices[i * 3 + 2] = 0;
        }
        else {
            vertices[i * 3] = randomInt(255) / 255.0f;
            vertices[i * 3 + 1] = (i + 1) / 19200.0f;
            vertices[i * 3 + 2] = (i + 2) / 19200.0f;
        }
        //std::cout << vertices[i * 2] << ",";
        //std::cout << vertices[i * 2 + 1] << ",";
        //std::cout << "(" << vertices[i * 5 + 2] << ",";
        //std::cout << vertices[i * 5 + 3] << ",";
        //std::cout << vertices[i * 5 + 4] << ")" << std::endl;
        
    }
    
    logger.log(1, "VERTICES MADE", "size of obj " + std::to_string(sizeof(vertices)) + " bytes");


    //seed generation
    std::srand(SEED);
    logger.log(1, "SEED SET ", "seed set to " + std::to_string(SEED));


    //OPENGL stuff. create the Vertex Array Objects ???
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the arrays and set settings
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    //tell the program which shader program to use
    glUseProgram(shaderProgram);

    
    //create the "cursor" chunks
    loc3d loc3;
    chunk worldland[3][3][3];
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            for (int z = 0; z < 3; z++) {
                loc3.x = x - 3 / 2;
                loc3.y = y - 3 / 2;
                loc3.z = z - 3 / 2;
                worldland[x][y][z].SetLocation(loc3);
            }
        }
    }
    
    logger.log(1, "CHUNK CREATED", "size of obj " + std::to_string(sizeof(worldland)) + " bytes");


    float frameStart = 0.0f;
    float frameFinish = 0.0f;
    float frameLimiter = 0.016f;

    int vcountiter = 0;
    int vcount = 0;
    int state = 0;

   
    
    while (!glfwWindowShouldClose(window))
    {
        
        if (frameFinish - frameStart > frameLimiter) {

            vcount = 0;
            vcountiter = 0;
            
            frameStart = glfwGetTime();

            
            // input
            state = processInput(window, gameCursor);
            
            for (int i = 0; i < 19200; i++) {
                vertices[i * 3] = randomInt(100) / 1000.f;
                vertices[i * 3 + 1] = randomInt(100) / 1000.f;
                vertices[i * 3 + 2] = randomInt(100) / 1000.f;
            }
            
            worldland[0][0][0].Simulate();
            
            for (int x = 0; x < 3; x++) {
                for (int y = 0; y < 3; y++) {
                    for (int z = 0; z < 3; z++) {
                        vcountiter = worldland[x][y][z].DrawVoxels(vertices, gameCursor, vcount);
                        vcount += vcountiter - 2;
                    }
                }
            }
            


            if (state == 1) {
                worldland[0][0][0].Insert(new voxel(&substances[1], 75.0f, false), gameCursor.x, gameCursor.y, gameCursor.z);
            }
            if (state == 2) {
                worldland[0][0][0].Insert(new voxel(&substances[2], 75.0f, false), gameCursor.x, gameCursor.y, gameCursor.z);
            }
            if (state == 3) {
                worldland[0][0][0].Insert(new voxel(&substances[0], 75.0f, false), gameCursor.x, gameCursor.y, gameCursor.z);
            }
            /*
            if (state == 4) {
                std::string loc = "Saves/" + std::to_string(gameNum) + "/chunks/";
                for (int x = 0; x < 3; x++) {
                    for (int y = 0; y < 3; y++) {
                        for (int z = 0; z < 3; z++) {
                            worldland[x][y][z].Save(loc);
                        }
                    }
                }
            }

            int maxIndex = *(&vertices) - vertices;

            //update the vertices here
            
            //cout << gameCursor;

            */

            


            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // color attribute
            //glVertexAttribPointer(1, 1, GL_INT, GL_FALSE, sizeof(int), (void*)( 1 * sizeof(int)));
            //glEnableVertexAttribArray(1);

            // render
            // ------
            glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
            glClear(GL_COLOR_BUFFER_BIT);

            glfwGetWindowSize(window, &screen_width, &screen_height);
            glPointSize(4);

            // render the triangle
            glBindVertexArray(VAO);
            glDrawArrays(GL_POINTS, 0, 19200);

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        frameFinish = glfwGetTime();

    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    delete worldland;

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
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        result = 5;
    if (glfwGetKey(window, GLFW_KEY_KP_0) == GLFW_PRESS)
        if (gameCursor.y > 0 )
            gameCursor.y--;
    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
        if (gameCursor.y < 12 - 1)
            gameCursor.y++;
    if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS)
        if (gameCursor.z > 0)
            gameCursor.z--;
    if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS)
        if (gameCursor.z < 12 - 1)
            gameCursor.z++;
    if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS)
        if (gameCursor.x > 0)
            gameCursor.x--;
    if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS)
        if (gameCursor.x < 12 - 1)
            gameCursor.x++;
    if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        if (gameCursor.z > 0)
            gameCursor.z--;
        if (gameCursor.x > 0)
            gameCursor.x--;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        if (gameCursor.z > 0)
            gameCursor.z--;
        if (gameCursor.x < 12 - 1)
            gameCursor.x++;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        if (gameCursor.x > 0)
            gameCursor.x--;
        if (gameCursor.z < 12 - 1)
            gameCursor.z++;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        if (gameCursor.x < 12 - 1)
            gameCursor.x++;
        if (gameCursor.z < 12 - 1)
            gameCursor.z++;
    }
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









