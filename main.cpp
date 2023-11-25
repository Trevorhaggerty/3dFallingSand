/*

Un-named Falling Voxel game
---------------------------
written by Trevor Haggerty
Version 0.1.6.4
*/

//includes
#include "includes.h"

//local headers
//#include "camera.h"
#include "imagefunc.h"
#include "imap.h"
#include "shaders.h"
#include "session.h"


//file manipulation library
#include <fstream>

//namespaces
using namespace Game;

//logging object
Logger logger("Global");

//used to monitor player input
imap inputState;


//set the window dimensions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//get input started
void processInput(GLFWwindow* window, imap &state);

//make the screen fuzzy
void fuzz(float* vertices, float frameStart);

session currentSession;





//main function
int main()
{   
    logger.log(1, "PRGM START", "main func started");
    logger.updateLocation("Main");

    //which save file to use
    int gameNum = 0;
    //gameNum = createNewSave();
    logger.log(1, "NEW SAVE", "save located at " + std::to_string(gameNum));

    // ints to hold current screen size
    int screenWidth, screenHeight;

    //Create the substances that the games engine will run on
    logger.log(1, "MAKING SUBSTANCE ARRAY", "size of each obj " + std::to_string(sizeof(substance)) + " bytes");
    substance substances[255];
    logger.log(1, "SUBSTS LIST MADE", "size of obj " + std::to_string(sizeof(substances)) + " bytes");

    //fill in the substance values
    logger.log(1, "FILLING SUBSTANCE DATA", "size of obj " + std::to_string(sizeof(substances)) + " bytes");
    substances[0] = substance(0, "air", NULL, NULL, -INF, INF, loc4df{ 0.0f, 0.0f, 0.0f, 0.0f }, 0, "gas");
    substances[1] = substance(1,"snd", NULL, NULL, -INF, 0.0f, loc4df{ 0.0f, 0.0f, 0.0f, 0.0f }, 1.5f, "powder");
    substances[2] = substance(2,"wtr", NULL, NULL, 0.0f, 100.0f, loc4df{ 0.0f, 0.0f, 0.0f, 0.0f }, 1.0f, "liquid");
    logger.log(1, "SUBSTS MADE", "size of obj " + std::to_string(sizeof(substances)) + " bytes");


    // glfw: initialize and configure
    // ------------------------------
    
    logger.log(1, "CREATING WINDOW", " version 3.3 glfw window...");
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


    logger.log(1, "INIT GLAD", "initializing GLAD...");

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
    float vertices[57600];

    









    //update the vertices here

    
    
    for (int i = 0; i < PIXELCOUNT; i++) {

        
        vertices[i * 3] = ( i % 160 ) / 160.0f;
        vertices[i * 3 + 1] = (i / 160.0f) / 120.f;
        vertices[i * 3 + 2] = i / PIXELCOUNT * 1.0f;
       
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

    int uniform_WindowSize = glGetUniformLocation(vertexShader, "WindowSize");
    
    //tell the program which shader program to use
    glUseProgram(shaderProgram);

    //glfwSetScrollCallback(window, scrollCallback);
    

    float frameStart = 0.0f;
    float frameFinish = 0.0f;
    float frameLimiter = 1.0f / 60.0f;
    float splashTime = 1.0f;
    
    

    loc2d mousequrt;

    const char* test1 = "C:/dev/c++/voxel001/assets/test1.bmp";
    const char* test2 = "C:/dev/c++/voxel001/assets/test2.bmp";
    const char* splashloc = "C:/dev/c++/voxel001/assets/company_splash2.bmp";
    const char* backgroundLoc = "C:/dev/c++/voxel001/assets/background.bmp";
    const char* backgroundLoc2 = "C:/dev/c++/voxel001/assets/background2.bmp";
    const char* backgroundLoc1 = "C:/dev/c++/voxel001/assets/backgroundha.bmp";








    int ticker = 0;
    int gamestate = 0; //game state 0 - splash screen






        while (!glfwWindowShouldClose(window))
        {

        if (frameFinish - frameStart > frameLimiter) {

            frameStart = glfwGetTime();
            
            // input
            processInput(window, inputState);
            
            glfwGetCursorPos(window, &inputState.mousex, &inputState.mousey);
            glfwGetWindowSize(window, &screenWidth, &screenHeight);
            glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

           
            
            loadBitmap(backgroundLoc, vertices, 160, 120);
            
            fuzz(vertices, frameStart);
            

            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // color attribute
            //glVertexAttribPointer(1, 1, GL_INT, GL_FALSE, sizeof(int), (void*)( 1 * sizeof(int)));
            //glEnableVertexAttribArray(1);

            // render
            // ------
            glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
            glClear(GL_COLOR_BUFFER_BIT);

            int pSize = (screenWidth * screenHeight) / (19200 * 4);
            glPointSize(pSize);

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

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window, imap &state)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        state.key[0] = true;
        glfwSetWindowShouldClose(window, true);
    }
    else {
        state.key[0] = false;
    }

    state.key[1] = (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS);
    state.key[2] = (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS);
    state.key[3] = (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS);
    state.key[4] = (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS);
    state.key[5] = (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS);
    state.key[6] = (glfwGetKey(window, GLFW_KEY_F6) == GLFW_PRESS);
    state.key[7] = (glfwGetKey(window, GLFW_KEY_F7) == GLFW_PRESS);
    state.key[8] = (glfwGetKey(window, GLFW_KEY_F8) == GLFW_PRESS);
    state.key[9] = (glfwGetKey(window, GLFW_KEY_F9) == GLFW_PRESS);

    state.key[10] = (glfwGetKey(window, GLFW_KEY_F10) == GLFW_PRESS);
    state.key[11] = (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS);
    state.key[12] = (glfwGetKey(window, GLFW_KEY_F12) == GLFW_PRESS);
    state.key[13] = (glfwGetKey(window, GLFW_KEY_PRINT_SCREEN) == GLFW_PRESS);

    state.key[14] = (glfwGetKey(window, GLFW_KEY_SCROLL_LOCK) == GLFW_PRESS);
    state.key[15] = (glfwGetKey(window, GLFW_KEY_PAUSE) == GLFW_PRESS);
    state.key[16] = (glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS);

    state.key[17] = (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS);
    state.key[18] = (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS);
    state.key[19] = (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS);
    state.key[20] = (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS);
    state.key[21] = (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS);
    state.key[22] = (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS);
    state.key[23] = (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS);
    state.key[24] = (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS);
    state.key[25] = (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS);
    state.key[26] = (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS);

    state.key[27] = (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS);
    state.key[28] = (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS);
    state.key[29] = (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS);
    state.key[30] = (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS);

    state.key[31] = (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS);
    state.key[32] = (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS);
    state.key[33] = (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS);
    state.key[34] = (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS);
    state.key[35] = (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS);
    state.key[36] = (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS);
    state.key[37] = (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS);
    state.key[38] = (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS);
    state.key[39] = (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS);

    state.key[40] = (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS);
    state.key[41] = (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS);
    state.key[42] = (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS);
    state.key[43] = (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS);
    
    state.key[44] = (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS);
    state.key[45] = (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS);
    state.key[46] = (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS);
    state.key[47] = (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS);
    state.key[48] = (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS);
    state.key[49] = (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS);
    state.key[50] = (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS);
    state.key[51] = (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS);
    state.key[52] = (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS);

    state.key[53] = (glfwGetKey(window, GLFW_KEY_SEMICOLON) == GLFW_PRESS);
    state.key[54] = (glfwGetKey(window, GLFW_KEY_APOSTROPHE) == GLFW_PRESS);
    state.key[55] = (glfwGetKey(window, GLFW_KEY_BACKSLASH) == GLFW_PRESS);
    state.key[56] = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
    
    state.key[57] = (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS);
    state.key[58] = (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS);
    state.key[59] = (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS);

    state.key[60] = (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS);
    state.key[61] = (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS);
    state.key[62] = (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS);
    state.key[63] = (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS);
    state.key[64] = (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS);
    state.key[65] = (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS);
    state.key[66] = (glfwGetKey(window, GLFW_KEY_SLASH) == GLFW_PRESS);
    state.key[67] = (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);
    state.key[68] = (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS);
    state.key[69] = (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS);

    state.key[70] = (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);
    state.key[71] = (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS);
    state.key[72] = (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS);
    
    state.key[73] = (glfwGetKey(window, GLFW_KEY_INSERT) == GLFW_PRESS);
    state.key[74] = (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS);
    state.key[75] = (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS);
    state.key[76] = (glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS);
    state.key[77] = (glfwGetKey(window, GLFW_KEY_END) == GLFW_PRESS);
    state.key[78] = (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS);
    state.key[79] = (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS);
    state.key[80] = (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS);
    state.key[81] = (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS);
    state.key[82] = (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS);

    state.key[83] = (glfwGetKey(window, GLFW_KEY_NUM_LOCK) == GLFW_PRESS);
    state.key[84] = (glfwGetKey(window, GLFW_KEY_KP_DIVIDE) == GLFW_PRESS);
    state.key[85] = (glfwGetKey(window, GLFW_KEY_KP_MULTIPLY) == GLFW_PRESS);
    state.key[86] = (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS);
    state.key[87] = (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS);
    state.key[88] = (glfwGetKey(window, GLFW_KEY_KP_EQUAL) == GLFW_PRESS);
    state.key[89] = (glfwGetKey(window, GLFW_KEY_KP_DECIMAL) == GLFW_PRESS);

    state.key[90] = (glfwGetKey(window, GLFW_KEY_KP_0) == GLFW_PRESS);
    state.key[91] = (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS);
    state.key[92] = (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS);
    state.key[93] = (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS);
    state.key[94] = (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS);
    state.key[95] = (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS);
    state.key[96] = (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS);
    state.key[97] = (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS);
    state.key[98] = (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS);
    state.key[99] = (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS);

    state.mousekeys[0] = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS);
    state.mousekeys[1] = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS);
    state.mousekeys[2] = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS);
    state.mousekeys[3] = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3) == GLFW_PRESS);
    state.mousekeys[4] = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_4) == GLFW_PRESS);
    state.mousekeys[5] = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_5) == GLFW_PRESS);
    state.mousekeys[6] = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_6) == GLFW_PRESS);
    state.mousekeys[7] = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_7) == GLFW_PRESS);
    state.mousekeys[8] = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_8) == GLFW_PRESS);
    state.mousekeys[9] = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
    state.mousekeys[10] = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
    state.mousekeys[11] = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LAST) == GLFW_PRESS);

}
//void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
//    gameCursor.y += (int)(yOffset);
//}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


void fuzz(float* vertices, float frameStart) {
    int odd = 0;
    for (int i = 0; i < PIXELCOUNT; i++) {


        odd = (i / (SCR_WIDTH / 4) + (int)(frameStart * 10) % 2) % 2;
        loc3df randcol(randomInt(100) / (1000.f / (odd / 2.1f)), randomInt(100) / (1000.f / (odd / 2.1f)), randomInt(100) / (1000.f / (odd / 2.1f)));
        vertices[i * 3] = randcol.x;
        vertices[i * 3 + 1] = randcol.y;
        vertices[i * 3 + 2] = randcol.z;

    }

}



