/*

Un-named Falling Voxel game
---------------------------
written by Trevor Haggerty
Version 0.0.4.3
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


struct imap {
    bool key[104];
    double mousex;
    double mousey;
    int mousei;
    imap() {
        for (bool b : key) {
            b = false;
            
        }
        mousex = 0;
        mousey = 0;
        mousei = 0;
    };
};



//TODO MOVE COLORS TO SHADER
//colors to be used
float basecolor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
float aircol[] = { 26 / 25500.0f, 107 / 25500.0f, 102 / 25500.0f, 0.01f };
float sandcol[] = { 0.50f, 0.50f, 0.0f, 1.0f };
float watercol[] = { -0.10f, -0.10f, 0.25f, 1.0f };



//TODO MAKE DICTIONARY TO HOLD SUBSTANCES
//Substances for voxel manipulation and representation
// (name, lo phase, hi phase, lo temp, hi temp, color, density, current phase)
substance* air = new substance("air", NULL, NULL, -INF, INF, aircol, 0.001f, "gas");
substance* sand = new substance("snd", NULL, NULL, -INF, 0.0f, sandcol, 1.5f, "powder");

substance* steam = new substance("stm", NULL, NULL, 100.0f, INF, basecolor, 0.0005f, "gas");
substance* ice = new substance("ice", NULL, NULL, -INF, 0.0f, basecolor, 0.9f, "solid");
substance* water = new substance("wtr", ice, steam, 0.0f, 100.0f, watercol, 1.0f, "liquid");




void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window, imap &state);


//TODO MOVE SHADERS TO SEPERATE FILE
//shaders
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
    loc3df cursorColor = { 0.50f,1.0f,0.0f };
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

    loc3d locSizer(0, 0, 0);
    logger.log(1, "loc3d generated", "size of obj " + std::to_string(sizeof(vertices)) + " bytes");

    
    for (int i = 0; i < 19200; i++) {

        
        vertices[i * 3] = ( i % 160 ) / 160.0f;
        vertices[i * 3 + 1] = (i / 160.0f) / 120.f;
        vertices[i * 3 + 2] = i / 19200.0f;
       
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

    int uniform_WindowSize = glGetUniformLocation(vertexShader, "WindowSize");
    
    //tell the program which shader program to use
    glUseProgram(shaderProgram);

    
    chunk * A = new chunk;
    
    float frameStart = 0.0f;
    float frameFinish = 0.0f;
    float frameLimiter = 0.016f;

    int vcountiter = 0;
    int vcount = 0;
    imap state;

    int oddman = 0;
    

    loc4df colorone(0.0f,1.0f,0.0f,1.0f);
    loc4df colortwo(1.0f,0.0f,1.0f,0.5);
    loc4df colorres = blend(colorone,colortwo);

    loc2d mousequrt;



    while (!glfwWindowShouldClose(window))
    {
        
        if (frameFinish - frameStart > frameLimiter) {

            vcount = 0;
            vcountiter = 0;
            
            frameStart = glfwGetTime();

            
            // input
            processInput(window, state);
            

            glfwGetCursorPos(window, &state.mousex, &state.mousey);
            glfwGetWindowSize(window, &screen_width, &screen_height);

            A->DrawVoxels(vertices, gameCursor, 0);

            for (int i = 0; i < 19200; i++) {
               
                
                    oddman = (i / (screen_width / 4) + (int)(frameStart * 10) % 2) % 2;
                    loc3df randcol(randomInt(100) / (1000.f / (oddman / 2.1f)), randomInt(100) / (1000.f / (oddman / 2.1f)), randomInt(100) / (1000.f / (oddman / 2.1f)));
                    vertices[i * 3] = randcol.x;
                    vertices[i * 3 + 1] = randcol.y;
                    vertices[i * 3 + 2] = randcol.z;
                
            }
            A->Simulate();
            A->DrawVoxels(vertices, gameCursor, 0);
            
            
            //logger.log(1, "MOUSE AT", "location( " + std::to_string(state.mousex) +","+ std::to_string(state.mousey) + ")");

            if (state.mousex < 0) {
                state.mousex = 0;
            }
            if (state.mousex > screen_width-1) {
                state.mousex = screen_width-1;
            }
            if (state.mousey < 0) {
                state.mousey = 0;
            }
            if (state.mousey > screen_height-1) {
                state.mousey = screen_height-1;
            }



            state.mousei = int(state.mousex / 4) +(19200) - (int(state.mousey / 4) * 160 );

            if (state.mousei < 0) {
                state.mousei = 0;
            }
            if (state.mousei > 19200) {
                state.mousei = 19200;
            }
            
            mousequrt = loc2d(state.mousex/4 -80, 60 - state.mousey/4 );

            //vertices[state.mousei * 3] = cursorColor.x;
            //vertices[state.mousei * 3 + 1] = cursorColor.y;
            //vertices[state.mousei * 3 + 2] = cursorColor.z;

            gameCursor = findAtHeight(mousequrt, gameCursor.y);
            if (state.key[68] && gameCursor.y < 11) {
                gameCursor.y++;
            }
            if (state.key[73] && gameCursor.y > 0) {
                gameCursor.y--;
            }
            if (state.key[82] && gameCursor.x < 11) {
                gameCursor.x++;
            }
            if (state.key[81] && gameCursor.z < 11) {
                gameCursor.z++;
            }
            if (state.key[80] && gameCursor.x > 0) {
                gameCursor.x--;
            }
            if (state.key[83] && gameCursor.z > 0) {
                gameCursor.z--;
            }



            if (state.key[18]) {
                A->Insert(new voxel(&substances[1], 75.0f, false), gameCursor.x, gameCursor.y, gameCursor.z);
            }
            if (state.key[19]) {
                A->Insert(new voxel(&substances[2], 75.0f, false), gameCursor.x, gameCursor.y, gameCursor.z);
            }
            if (state.key[17]) {
                A->Insert(new voxel(&substances[0], 75.0f, false), gameCursor.x, gameCursor.y, gameCursor.z);
            }
            /*
            if (state == 4) {
                std::string loc = "Saves/" + std::to_string(gameNum) + "/chunks/";
                for (int x = 0; x < 3; x++) {
                    for (int y = 0; y < 3; y++) {
                        for (int z = 0; z < 3; z++) {
                            A.Save(loc);
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

    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
        state.key[1] = true;
    }
    else {
        state.key[1] = false;
    }
    
    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {
        state.key[2] = true;
    }
    else {
        state.key[2] = false;
    }
    
    if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS) {
        state.key[3] = true;
    }
    else {
        state.key[3] = false;
    }
    
    if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS) {
        state.key[4] = true;
    }
    else {
        state.key[4] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS) {
        state.key[5] = true;
    }
    else {
        state.key[5] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_F6) == GLFW_PRESS) {
        state.key[6] = true;
    }
    else {
        state.key[6] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_F7) == GLFW_PRESS) {
        state.key[7] = true;
    }
    else {
        state.key[7] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_F8) == GLFW_PRESS) {
        state.key[8] = true;
    }
    else {
        state.key[8] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_F9) == GLFW_PRESS) {
        state.key[9] = true;
    }
    else {
        state.key[9] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_F10) == GLFW_PRESS) {
        state.key[10] = true;
    }
    else {
        state.key[10] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS) {
        state.key[11] = true;
    }
    else {
        state.key[11] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_F12) == GLFW_PRESS) {
        state.key[12] = true;
    }
    else {
        state.key[12] = false;
    }
    
    if (glfwGetKey(window, GLFW_KEY_PRINT_SCREEN) == GLFW_PRESS) {
        state.key[13] = true;
    }
    else {
        state.key[13] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_SCROLL_LOCK) == GLFW_PRESS) {
        state.key[14] = true;
    }
    else {
        state.key[14] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_PAUSE) == GLFW_PRESS) {
        state.key[15] = true;
    }
    else {
        state.key[15] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS) {
        state.key[16] = true;
    }
    else {
        state.key[16] = false;
    }
    
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        state.key[17] = true;
    }
    else {
        state.key[17] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        state.key[18] = true;
    }
    else {
        state.key[18] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        state.key[19] = true;
    }
    else {
        state.key[19] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        state.key[20] = true;
    }
    else {
        state.key[20] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
        state.key[21] = true;
    }
    else {
        state.key[21] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
        state.key[22] = true;
    }
    else {
        state.key[22] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
        state.key[23] = true;
    }
    else {
        state.key[23] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
        state.key[24] = true;
    }
    else {
        state.key[24] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
        state.key[25] = true;
    }
    else {
        state.key[25] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
        state.key[26] = true;
    }
    else {
        state.key[26] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
        state.key[27] = true;
    }
    else {
        state.key[27] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
        state.key[28] = true;
    }
    else {
        state.key[28] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
        state.key[29] = true;
    }
    else {
        state.key[29] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
        state.key[30] = true;
    }
    else {
        state.key[30] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        state.key[31] = true;
    }
    else {
        state.key[31] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        state.key[32] = true;
    }
    else {
        state.key[32] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        state.key[33] = true;
    }
    else {
        state.key[33] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        state.key[34] = true;
    }
    else {
        state.key[34] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        state.key[35] = true;
    }
    else {
        state.key[35] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        state.key[36] = true;
    }
    else {
        state.key[36] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        state.key[37] = true;
    }
    else {
        state.key[37] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        state.key[38] = true;
    }
    else {
        state.key[38] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        state.key[39] = true;
    }
    else {
        state.key[39] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        state.key[40] = true;
    }
    else {
        state.key[40] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS) {
        state.key[41] = true;
    }
    else {
        state.key[41] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS) {
        state.key[42] = true;
    }
    else {
        state.key[42] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        state.key[43] = true;
    }
    else {
        state.key[43] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS) {
        state.key[44] = true;
    }
    else {
        state.key[44] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        state.key[45] = true;
    }
    else {
        state.key[45] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        state.key[46] = true;
    }
    else {
        state.key[46] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        state.key[47] = true;
    }
    else {
        state.key[47] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        state.key[48] = true;
    }
    else {
        state.key[48] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        state.key[49] = true;
    }
    else {
        state.key[49] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
        state.key[50] = true;
    }
    else {
        state.key[50] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        state.key[51] = true;
    }
    else {
        state.key[51] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        state.key[52] = true;
    }
    else {
        state.key[52] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        state.key[53] = true;
    }
    else {
        state.key[53] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_SEMICOLON) == GLFW_PRESS) {
        state.key[54] = true;
    }
    else {
        state.key[54] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_APOSTROPHE) == GLFW_PRESS) {
        state.key[55] = true;
    }
    else {
        state.key[55] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_BACKSLASH) == GLFW_PRESS) {
        state.key[56] = true;
    }
    else {
        state.key[56] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        state.key[57] = true;
    }
    else {
        state.key[57] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        state.key[58] = true;
    }
    else {
        state.key[58] = false;

    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        state.key[59] = true;
    }
    else {
        state.key[59] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        state.key[60] = true;
    }
    else {
        state.key[60] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        state.key[61] = true;
    }
    else {
        state.key[61] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        state.key[62] = true;
    }
    else {
        state.key[62] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
        state.key[63] = true;
    }
    else {
        state.key[63] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        state.key[64] = true;
    }
    else {
        state.key[64] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS) {
        state.key[65] = true;
    }
    else {
        state.key[65] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS) {
        state.key[66] = true;
    }
    else {
        state.key[66] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_SLASH) == GLFW_PRESS) {
        state.key[67] = true;
    }
    else {
        state.key[67] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
        state.key[68] = true;
    }
    else {
        state.key[68] = false;

    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        state.key[69] = true;
    }
    else {
        state.key[69] = false;
    }


    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        state.key[70] = true;
    }
    else {
        state.key[70] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        state.key[71] = true;
    }
    else {
        state.key[71] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS) {
        state.key[72] = true;
    }
    else {
        state.key[72] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
        state.key[73] = true;
    }
    else {
        state.key[73] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_INSERT) == GLFW_PRESS) {
        state.key[74] = true;
    }
    else {
        state.key[74] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) {
        state.key[75] = true;
    }
    else {
        state.key[75] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
        state.key[76] = true;
    }
    else {
        state.key[76] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS) {
        state.key[77] = true;
    }
    else {
        state.key[77] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_END) == GLFW_PRESS) {
        state.key[78] = true;
    }
    else {
        state.key[78] = false;

    }
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
        state.key[79] = true;
    }
    else {
        state.key[79] = false;
    }



    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        state.key[80] = true;
    }
    else {
        state.key[80] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        state.key[81] = true;
    }
    else {
        state.key[81] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        state.key[82] = true;
    }
    else {
        state.key[82] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        state.key[83] = true;
    }
    else {
        state.key[83] = false;
    }



    /*
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    
    if (glfwGetKey(window, GLFW_KEY_KP_0) == GLFW_PRESS)
    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
    if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS)
    if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS)
    if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS)
    if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS)
    if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    */

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}









