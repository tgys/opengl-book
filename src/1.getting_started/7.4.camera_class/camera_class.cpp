#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <imgui/imgui.h>
//#include <imgui/imgui_impl_glfw_gl3.h>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <map>

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void RenderText(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
unsigned int VBO, VAO;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

unsigned int loadTexture(char const *path);
constexpr double pi() { return std::atan(1)*4; }

int main()
{


    if (!glfwInit())
        return 1;
    glfwWindowHint(GLFW_SAMPLES, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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
//    ImGui_ImplGlfwGL3_Init(window, true);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);


    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); // always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))


    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("camera_walls.vs", "camera_walls.fs");
    Shader floorShader("camera_floor.vs", "camera_floor.fs");
    Shader lightCubeShader("camera_lightsrc.vs", "camera_lightsrc.fs");

    // load textures
    // -------------

    unsigned int floorMap_diffuse = loadTexture(FileSystem::getPath("resources/textures/rect_tiles.jpg").c_str());
    unsigned int floorMap_normal  = loadTexture(FileSystem::getPath("resources/textures/rect_tiles_nmp.jpg").c_str());

    unsigned int concreteTexture  = loadTexture(FileSystem::getPath("resources/textures/concrete.jpg").c_str());

    unsigned int diffuse_floorTexture = loadTexture(FileSystem::getPath("resources/textures/container2.png").c_str());
    unsigned int specular_floorTexture = loadTexture(FileSystem::getPath("resources/textures/container2_specular.png").c_str());

    unsigned int brickTexture = loadTexture(FileSystem::getPath("resources/textures/bric.jpg").c_str());

   // glm::vec3 lightPos(4.5f, 3.5f, 20.0f);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float doorVertices[] = {
    //        9.5f, -5.0f, -80.0f,  1.0f, 0.0f,
    //        10.0f,  5.0f, -80.0f,  1.0f, 1.0f,
    //        10.0f,  5.0f, -80.0f,  1.0f, 1.0f,
    //        -10.0f,  5.0f, -80.0f,  0.0f, 1.0f,
    //        -10.0f, -5.0f, -80.0f,  0.0f, 0.0f,
        9.5f, -4.4f, -2.9f,  0.0f, 0.0f,
        9.5f, -4.4f, 2.9f,  1.0f, 0.0f,
        9.5f,  3.5f, 2.9f,  1.0f, 1.0f,
        9.5f,  3.5f, 2.9f,  1.0f, 1.0f,
        9.5f,  3.5f, -2.9f,  0.0f, 1.0f,
        9.5f, -4.4f, -2.9f,  0.0f, 0.0f
    };

    float lightVertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    float roomVertices[] = {

            //-10.0f, -5.0f, -80.0f,  0.0f, 0.0f,
            // 10.0f, -5.0f, -80.0f,  1.0f, 0.0f,
            // 10.0f,  5.0f, -80.0f,  1.0f, 1.0f,
            // 10.0f,  5.0f, -80.0f,  1.0f, 1.0f,
            //-10.0f,  5.0f, -80.0f,  0.0f, 1.0f,
            //-10.0f, -5.0f, -80.0f,  0.0f, 0.0f,

            -10.0f, -5.0f,  20.0f,  0.0f, 0.0f,
             10.0f, -5.0f,  20.0f,  1.0f, 0.0f,
             10.0f,  5.0f,  20.0f,  1.0f, 1.0f,
             10.0f,  5.0f,  20.0f,  1.0f, 1.0f,
            -10.0f,  5.0f,  20.0f,  0.0f, 1.0f,
            -10.0f, -5.0f,  20.0f,  0.0f, 0.0f,

            -10.0f,  5.0f,  20.0f,  1.0f, 0.0f,
            -10.0f,  5.0f, -20.0f,  1.0f, 1.0f,
            -10.0f, -5.0f, -20.0f,  0.0f, 1.0f,
            -10.0f, -5.0f, -20.0f,  0.0f, 1.0f,
            -10.0f, -5.0f,  20.0f,  0.0f, 0.0f,
            -10.0f,  5.0f,  20.0f,  1.0f, 0.0f,

             10.0f,  5.0f,  20.0f,  1.0f, 0.0f,
             10.0f,  5.0f, -20.0f,  1.0f, 1.0f,
             10.0f, -5.0f, -20.0f,  0.0f, 1.0f,
             10.0f, -5.0f, -20.0f,  0.0f, 1.0f,
             10.0f, -5.0f,  20.0f,  0.0f, 0.0f,
             10.0f,  5.0f,  20.0f,  1.0f, 0.0f,

            -10.0f, -5.0f, -20.0f,  0.0f, 1.0f,
             10.0f, -5.0f, -20.0f,  1.0f, 1.0f,
             10.0f, -5.0f,  20.0f,  1.0f, 0.0f,
             10.0f, -5.0f,  20.0f,  1.0f, 0.0f,
            -10.0f, -5.0f,  20.0f,  0.0f, 0.0f,
            -10.0f, -5.0f, -20.0f,  0.0f, 1.0f,

            -10.0f,  5.0f, -20.0f,  0.0f, 1.0f,
             10.0f,  5.0f, -20.0f,  1.0f, 1.0f,
             10.0f,  5.0f,  20.0f,  1.0f, 0.0f,
             10.0f,  5.0f,  20.0f,  1.0f, 0.0f,
            -10.0f,  5.0f,  20.0f,  0.0f, 0.0f,
            -10.0f,  5.0f, -20.0f,  0.0f, 1.0f,
    };

    float hallVertices[] = {

        -10.0f, -5.0f, -80.0f,  0.0f, 0.0f,
         10.0f, -5.0f, -80.0f,  1.0f, 0.0f,
         10.0f,  5.0f, -80.0f,  1.0f, 1.0f,
         10.0f,  5.0f, -80.0f,  1.0f, 1.0f,
        -10.0f,  5.0f, -80.0f,  0.0f, 1.0f,
        -10.0f, -5.0f, -80.0f,  0.0f, 0.0f,

        -10.0f, -5.0f,  80.0f,  0.0f, 0.0f,
         10.0f, -5.0f,  80.0f,  1.0f, 0.0f,
         10.0f,  5.0f,  80.0f,  1.0f, 1.0f,
         10.0f,  5.0f,  80.0f,  1.0f, 1.0f,
        -10.0f,  5.0f,  80.0f,  0.0f, 1.0f,
        -10.0f, -5.0f,  80.0f,  0.0f, 0.0f,

        -10.0f,  5.0f,  80.0f,  1.0f, 0.0f,
        -10.0f,  5.0f, -80.0f,  1.0f, 1.0f,
        -10.0f, -5.0f, -80.0f,  0.0f, 1.0f,
        -10.0f, -5.0f, -80.0f,  0.0f, 1.0f,
        -10.0f, -5.0f,  80.0f,  0.0f, 0.0f,
        -10.0f,  5.0f,  80.0f,  1.0f, 0.0f,

         10.0f,  5.0f,  80.0f,  1.0f, 0.0f,
         10.0f,  5.0f, -80.0f,  1.0f, 1.0f,
         10.0f, -5.0f, -80.0f,  0.0f, 1.0f,
         10.0f, -5.0f, -80.0f,  0.0f, 1.0f,
         10.0f, -5.0f,  80.0f,  0.0f, 0.0f,
         10.0f,  5.0f,  80.0f,  1.0f, 0.0f,

       // -10.0f, -5.0f, -80.0f,  0.0f, 1.0f,
       //  10.0f, -5.0f, -80.0f,  1.0f, 1.0f,
       //  10.0f, -5.0f,  80.0f,  1.0f, 0.0f,
       //  10.0f, -5.0f,  80.0f,  1.0f, 0.0f,
       // -10.0f, -5.0f,  80.0f,  0.0f, 0.0f,
       // -10.0f, -5.0f, -80.0f,  0.0f, 1.0f,

        -10.0f,  5.0f, -80.0f,  0.0f, 1.0f,
         10.0f,  5.0f, -80.0f,  1.0f, 1.0f,
         10.0f,  5.0f,  80.0f,  1.0f, 0.0f,
         10.0f,  5.0f,  80.0f,  1.0f, 0.0f,
        -10.0f,  5.0f,  80.0f,  0.0f, 0.0f,
        -10.0f,  5.0f, -80.0f,  0.0f, 1.0f,

        };

    float floorHallVertices[] = {
        -10.0f, -5.0f, -80.0f,  0.0f, 1.0f,
         10.0f, -5.0f, -80.0f,  1.0f, 1.0f,
         10.0f, -5.0f,  80.0f,  1.0f, 0.0f,
         10.0f, -5.0f,  80.0f,  1.0f, 0.0f,
        -10.0f, -5.0f,  80.0f,  0.0f, 0.0f,
        -10.0f, -5.0f, -80.0f,  0.0f, 1.0f,
    };
        // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
       // glm::vec3( 2.0f,  5.0f, -15.0f),
      //  glm::vec3(-1.5f, -2.2f, -2.5f),
      //  glm::vec3(-3.8f, -2.0f, -12.3f),
      //  glm::vec3( 2.4f, -0.4f, -3.5f),
      //  glm::vec3(-1.7f,  3.0f, -7.5f),
      //  glm::vec3( 1.3f, -2.0f, -2.5f),
      //  glm::vec3( 1.5f,  2.0f, -2.5f),
      //  glm::vec3( 1.5f,  0.2f, -1.5f),
      //  glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(hallVertices), hallVertices, GL_STATIC_DRAW);

    Shader textShader("text_cam.vs", "text_cam.fs");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
    textShader.use();
    glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));



    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // door VAO
    unsigned int doorVAO, doorVBO;
    glGenVertexArrays(1, &doorVAO);
    glGenBuffers(1, &doorVBO);
    glBindVertexArray(doorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, doorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(doorVertices), &doorVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    // room VAO
    unsigned int roomVAO, roomVBO;
    glGenVertexArrays(1, &roomVAO);
    glGenBuffers(1, &roomVBO);
    glBindVertexArray(roomVAO);
    glBindBuffer(GL_ARRAY_BUFFER, roomVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(roomVertices), &roomVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    // hall floor VAO
    unsigned int hfloorVAO, hfloorVBO;
    glGenVertexArrays(1, &hfloorVAO);
    glGenBuffers(1, &hfloorVBO);
    glBindVertexArray(hfloorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, hfloorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorHallVertices), &floorHallVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    // first, configure the cube's VAO (and VBO)
    unsigned int lightCubeVBO;
    glGenBuffers(1, &lightCubeVBO);

    glBindBuffer(GL_ARRAY_BUFFER, lightCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertices), lightVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lightCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertices), lightVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, lightCubeVBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lightCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertices), lightVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //unsigned int lightCubeVAO, lightCubeVBO;
    //glGenVertexArrays(1, &lightCubeVAO);
    //glGenBuffers(1, &lightCubeVBO);
    //glBindVertexArray(lightCubeVAO);
    //glBindBuffer(GL_ARRAY_BUFFER, lightCubeVBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertices), &lightVertices, GL_STATIC_DRAW);
    //glEnableVertexAttribArray(0);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    //glBindVertexArray(0);


    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    ourShader.setInt("diffuseMap", 0);
    ourShader.setInt("normalMap", 1);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




   //     // bind diffuse map
   //     glActiveTexture(GL_TEXTURE0);
   //     glBindTexture(GL_TEXTURE_2D, diffuseMap);
   //     // bind specular map
   //     glActiveTexture(GL_TEXTURE1);
   //     glBindTexture(GL_TEXTURE_2D, specularMap);


       // RenderText(textShader, "this is a test", 50.0f, 20.0f, 0.5f, glm::vec3(0.8, 0.8f, 0.2f));
       // RenderText(textShader, "text2", 550.0f, 300.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

        // bind textures on corresponding texture units
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, texture1);
        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, texture2);



        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, brickTexture);
        // activate shader
        ourShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

       glm::mat4 model = glm::mat4(1.0f);

        // render boxes
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 1; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, cubePositions[i]);
            //if(i == 1)
             //  glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            float angle = 20.0f * i;
           // model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);
            //floorShader.setMat4("model", model);

            //glDrawArrays(GL_TRIANGLES, 0, 36);
            glDrawArrays(GL_TRIANGLES, 0, 30);
        }

       // ourShader.setVec3("viewPos", camera.Position);
      //  ourShader.setVec3("lightPos", lightPos);

        glBindVertexArray(doorVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, concreteTexture);
        //glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 60.0f));
        ourShader.setMat4("model", model);
//        floorShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

    //    glActiveTexture(GL_TEXTURE0);
    //    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    //    glActiveTexture(GL_TEXTURE1);
    //    glBindTexture(GL_TEXTURE_2D, normalMap);
    //    renderQuad();



        glBindVertexArray(roomVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, brickTexture);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(30.0f, 0.0f, 60.0f));
        model = glm::rotate(model, static_cast<float>(pi()/2), glm::vec3(0, 1, 0));
        ourShader.setMat4("model", model);
       // floorShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        //floorShader.use();
        // bind diffuse map
        glBindVertexArray(hfloorVAO);

        floorShader.use();

        glm::vec3 lightPos(2.5f, 0.5f, 10.0f);

        floorShader.setVec3("light.position", lightPos);
        floorShader.setVec3("viewPos", camera.Position);

        // light properties
        floorShader.setInt("material.diffuse", 0);
        floorShader.setInt("material.specular", 1);


        floorShader.setVec3("light.ambient", 2.5f, 2.5f, 2.5f);
        floorShader.setVec3("light.diffuse", 5.5f, 5.5f, 5.5f);
        floorShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        floorShader.setFloat("light.constant", 1.0f);
        floorShader.setFloat("light.linear", 0.09f);
        floorShader.setFloat("light.quadratic", 0.032f);

        // material propertie
        floorShader.setFloat("material.shininess", 52.0f);

        // view/projection transformations
        floorShader.setMat4("projection", projection);
        floorShader.setMat4("view", view);




        // world transformation
         //glm::mat4(1.0f);
     //   model = glm::translate(model, glm::vec3(30.0f, 0.0f, 60.0f));
     //   model = glm::rotate(model, static_cast<float>(pi()/2), glm::vec3(0, 1, 0));
        floorShader.setMat4("model", model);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuse_floorTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specular_floorTexture);


        glDrawArrays(GL_TRIANGLES, 0, 6);
       // glBindVertexArray(0);

       // render containers
//       glBindVertexArray(cubeVAO);
//
//       float z_inc = 4.5;
//       float x_inc = 0.2;
//       float x = 0.0f;
//       float z = 0.0f;
//       //glm::vec3( 0.0f,  0.0f,  0.0f);
//       for (unsigned int i = 0; i < 2; i++)
//       {
//           // calculate the model matrix for each object and pass it to shader before drawing
//           model = glm::mat4(1.0f);
//        //   model = glm::translate(model, cubePositions[i]);
//           model = glm::translate(model, glm::vec3( x + x_inc,  -4.4f,  z+z_inc));
//           model = glm::scale(model, glm::vec3(2.0f));
//           float angle = 20.0f * i;
//           glActiveTexture(GL_TEXTURE0);
//           glBindTexture(GL_TEXTURE_2D, diffuse_floorTexture);
//           glActiveTexture(GL_TEXTURE1);
//           glBindTexture(GL_TEXTURE_2D, specular_floorTexture);
//           floorShader.setMat4("model", model);
//           glDrawArrays(GL_TRIANGLES, 0, 36);
//       }

       glBindVertexArray(lightCubeVAO);
       // also draw the lamp object
       lightCubeShader.use();
       lightCubeShader.setMat4("projection", projection);
       lightCubeShader.setMat4("view", view);
       model = glm::mat4(1.0f);
       model = glm::translate(model, lightPos);
       model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
       lightCubeShader.setMat4("model", model);

       glDrawArrays(GL_TRIANGLES, 0, 36);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &lightCubeVBO);
    glDeleteVertexArrays(1, &hfloorVAO);
    glDeleteBuffers(1, &hfloorVBO);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &roomVAO);
    glDeleteBuffers(1, &roomVBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


// renders a 1x1 quad in NDC with manually calculated tangent vectors
// ------------------------------------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad(int num_triangles)
{
    if (quadVAO == 0)
    {
        // positions
        glm::vec3 pos1(-1.0f,  1.0f, 0.0f);
        glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
        glm::vec3 pos3( 1.0f, -1.0f, 0.0f);
        glm::vec3 pos4( 1.0f,  1.0f, 0.0f);
        // texture coordinates
        glm::vec2 uv1(0.0f, 1.0f);
        glm::vec2 uv2(0.0f, 0.0f);
        glm::vec2 uv3(1.0f, 0.0f);
        glm::vec2 uv4(1.0f, 1.0f);
        // normal vector
        glm::vec3 nm(0.0f, 0.0f, 1.0f);

        // calculate tangent/bitangent vectors of both triangles
        glm::vec3 tangent1, bitangent1;
        glm::vec3 tangent2, bitangent2;
        // triangle 1
        // ----------
        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

        // triangle 2
        // ----------
        edge1 = pos3 - pos1;
        edge2 = pos4 - pos1;
        deltaUV1 = uv3 - uv1;
        deltaUV2 = uv4 - uv1;

        f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


        bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);


        float quadVertices[] = {
                // positions            // normal         // texcoords  // tangent                          // bitangent
                pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
                pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
                pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

                pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
                pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
                pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
        };
        // configure plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, num_triangles);
    glBindVertexArray(0);
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboardRoom(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboardRoom(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboardRoom(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboardRoom(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
void RenderText(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color)
{
    // activate corresponding render state
    shader.use();
    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const *path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

