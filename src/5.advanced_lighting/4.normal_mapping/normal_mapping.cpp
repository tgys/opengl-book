#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);
unsigned int renderQuad();
unsigned int loadCubemap(vector<std::string> faces);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

struct RoomSettings{
    Shader shader;
    unsigned int lr_diffuseMap;
    unsigned int lr_normalMap;
    unsigned int fb_diffuseMap;
    unsigned int fb_normalMap;
    unsigned int floor_normalMap;
    unsigned int floor_diffuseMap;
    unsigned int ceil_normalMap;
    unsigned int ceil_diffuseMap;
    Shader mirror_shader;
    int num_mirrors;
    unsigned int door_outer_dMap;
    unsigned int door_outer_nMap;
    unsigned int door_inner_dMap;
    unsigned int door_inner_nMap;
    unsigned int mirror_normalMap;
    int num_lights;
    unsigned int lightCubeVAO;
    unsigned int light_diffuseMap;
    unsigned int light_normalMap;
    Shader lightShader;
    glm::mat4 view;
    glm::mat4 projection;
    float room_scale;
    float sz;
};

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

unsigned int renderRoom(RoomSettings& rsts, int height, int m, int n, glm::vec3 move_to, glm::vec3 viewPos, bool& pass_fb);

unsigned int _renderRoom(Shader& shader, int height, int m, int n, unsigned int lr_dMap, unsigned int lr_nMap,
                         unsigned int fb_dMap, unsigned int fb_nMap, unsigned int c_nMap, unsigned int c_dMap,
                unsigned int f_nMap, unsigned int f_dMap, glm::vec3 move_to,

                float sz, Shader mirror_shader, int num, unsigned int textureColorbuffer,
                unsigned int door_outer_nMap, unsigned int door_inner_dMap, unsigned int door_inner_nMap, unsigned int mirror_normalMap,

                int num_lights, unsigned int lightCubeVAO, unsigned int light_diffuseMap, unsigned int lightNormalMap,
                Shader& lightShader, glm::mat4 view, glm::mat4 projection, glm::vec3 viewPos, float scale, bool& pass_fb);

unsigned int renderCube(Shader& shader, int height, int m, int n, unsigned int lr_dMap, unsigned int lr_nMap, unsigned int fb_dMap, unsigned int fb_nMap,
                        unsigned int f_nMap, unsigned int f_dMap, unsigned int c_nMap, unsigned int c_dMap, glm::vec3 move_to, glm::vec3 viewPos, glm::mat4 view, float scale, bool& pass_fb);

void renderMirrors(float sz, Shader shader, int num, int n, int m, unsigned int textureColorbuffer,
                           unsigned int mirror_normalMap, glm::vec3 move_to,
                           glm::mat4 view, glm::vec3 viewPos,  glm::mat4 projection);

void renderDoors(float sz, Shader shader, Shader normal_shader, int num_doors, int n, int m, int h,
            unsigned int mirror_diffuseMap, unsigned int mirror_normalMap,
            unsigned int inner_diffuseMap, unsigned int inner_normalMap, glm::vec3 move_to,
            glm::mat4 view, glm::mat4 projection, glm::vec3 viewPos);
int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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

    // lighting info
    // -------------
    glm::vec3 lightPos(20.2f, 0.7f, 2.9f);


    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // build and compile shaders
    // -------------------------

//    Shader shader("normal_map.vs", "normal_map.fs");
    Shader shader("normal_map_hallw.vs", "normal_map_hallw.fs");
    Shader screenShader("framebuffers_screen.vs", "framebuffers_screen.fs");
    Shader basicShader("framebuffers.vs", "framebuffers.fs");
    //Shader skyboxShader("skybox_hallw.vs", "skybox_hallw.fs");
    Shader lightShader("light_cube.vs", "light_cube.fs");
    Shader mirrorShader("mirror_shader.vs", "mirror_shader.fs");

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

    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
            // positions   // texCoords

            -0.3f,  1.0f,  0.0f, 1.0f,
            -0.3f,  0.7f,  0.0f, 0.0f,
            0.3f,  0.7f,  1.0f, 0.0f,

            -0.3f,  1.0f,  0.0f, 1.0f,
            0.3f,  0.7f,  1.0f, 0.0f,
            0.3f,  1.0f,  1.0f, 1.0f
    };

    float doorVertices[] = {
            9.5f, 0.2f, 2.0f,  0.0f, 0.0f,
            9.5f, 0.2f, 2.0f,  1.0f, 0.0f,
            9.5f, 0.2f, 2.0f,  1.0f, 1.0f,
            9.5f, 0.2f, 2.0f,  1.0f, 1.0f,
            9.5f, 0.2f, 2.0f,  0.0f, 1.0f,
            9.5f, 0.2f, 2.0f,  0.0f, 0.0f
    };

    //float skyboxVertices[] = {
    //        // positions
    //        -500.0f,  500.0f, -500.0f,
    //        -500.0f, -500.0f, -500.0f,
    //         500.0f, -500.0f, -500.0f,
    //         500.0f, -500.0f, -500.0f,
    //         500.0f,  500.0f, -500.0f,
    //        -500.0f,  500.0f, -500.0f,

    //        -500.0f, -500.0f,  500.0f,
    //        -500.0f, -500.0f, -500.0f,
    //        -500.0f,  500.0f, -500.0f,
    //        -500.0f,  500.0f, -500.0f,
    //        -500.0f,  500.0f,  500.0f,
    //        -500.0f, -500.0f,  500.0f,

    //         500.0f, -500.0f, -500.0f,
    //         500.0f, -500.0f,  500.0f,
    //         500.0f,  500.0f,  500.0f,
    //         500.0f,  500.0f,  500.0f,
    //         500.0f,  500.0f, -500.0f,
    //         500.0f, -500.0f, -500.0f,

    //        -500.0f, -500.0f,  500.0f,
    //        -500.0f,  500.0f,  500.0f,
    //         500.0f,  500.0f,  500.0f,
    //         500.0f,  500.0f,  500.0f,
    //         500.0f, -500.0f,  500.0f,
    //        -500.0f, -500.0f,  500.0f,

    //        -500.0f,  500.0f, -500.0f,
    //         500.0f,  500.0f, -500.0f,
    //         500.0f,  500.0f,  500.0f,
    //         500.0f,  500.0f,  500.0f,
    //        -500.0f,  500.0f,  500.0f,
    //        -500.0f,  500.0f, -500.0f,

    //        -500.0f, -500.0f, -500.0f,
    //        -500.0f, -500.0f,  500.0f,
    //         500.0f, -500.0f, -500.0f,
    //         500.0f, -500.0f, -500.0f,
    //        -500.0f, -500.0f,  500.0f,
    //         500.0f, -500.0f,  500.0f
    //};

    // load textures
    // -------------
    unsigned int diffuseMap = loadTexture(FileSystem::getPath("resources/textures/bric.jpg").c_str());
    unsigned int normalMap  = loadTexture(FileSystem::getPath("resources/textures/bric_nmp.jpg").c_str());
    unsigned int door_outer_nMap  = loadTexture(FileSystem::getPath("resources/textures/block_nmp.png").c_str());
    unsigned int mirror_diffuseMap  = loadTexture(FileSystem::getPath("resources/textures/glass3.jpg").c_str());
    unsigned int mirror_normalMap  = loadTexture(FileSystem::getPath("resources/textures/glass3_nmp.jpg").c_str());

    unsigned int window_dMap  = loadTexture(FileSystem::getPath("resources/textures/window.png").c_str());
    unsigned int window_nMap  = loadTexture(FileSystem::getPath("resources/textures/window_nmp.png").c_str());
    unsigned int window_noborder_nMap  = loadTexture(FileSystem::getPath("resources/textures/window2.png").c_str());
    unsigned int window_noborder_dMap  = loadTexture(FileSystem::getPath("resources/textures/window2_nmp.png").c_str());

    unsigned int crystal_diffuseMap  = loadTexture(FileSystem::getPath("resources/textures/crystal.jpg").c_str());
    unsigned int crystal_normalMap  = loadTexture(FileSystem::getPath("resources/textures/crystal_nmp.jpg").c_str());

    unsigned int floor_diffuseMap = loadTexture(FileSystem::getPath("resources/textures/rect_tiles.jpg").c_str());   ///////////
    unsigned int floor_normalMap  = loadTexture(FileSystem::getPath("resources/textures/rect_tiles_nmp.jpg").c_str()); //////////

    unsigned int concreteTexture  = loadTexture(FileSystem::getPath("resources/textures/concrete.jpg").c_str());


    // shader configuration
    // --------------------
    screenShader.use();
    screenShader.setInt("screenTexture", 0);

    // framebuffer configuration
    // -------------------------
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a color attachment texture
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
   // unsigned int rbo;
   // glGenRenderbuffers(1, &rbo);
   // glBindRenderbuffer(GL_RENDERBUFFER, rbo);
   // glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
   // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    shader.use();
    shader.setInt("diffuseMap", 0);
    shader.setInt("normalMap", 1);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightCubeVBO;
    glGenBuffers(1, &lightCubeVBO);

    glBindBuffer(GL_ARRAY_BUFFER, lightCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertices), lightVertices, GL_STATIC_DRAW);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, lightCubeVBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

   // // door VAO
   // unsigned int doorVAO, doorVBO;
   // glGenVertexArrays(1, &doorVAO);
   // glGenBuffers(1, &doorVBO);
   // glBindVertexArray(doorVAO);
   // glBindBuffer(GL_ARRAY_BUFFER, doorVBO);
   // glBufferData(GL_ARRAY_BUFFER, sizeof(doorVertices), &doorVertices, GL_STATIC_DRAW);
   // glEnableVertexAttribArray(0);
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
   // glEnableVertexAttribArray(1);
   // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
   // glBindVertexArray(0);

   // skybox VAO
   //unsigned int skyboxVAO, skyboxVBO;
   //glGenVertexArrays(1, &skyboxVAO);
   //glGenBuffers(1, &skyboxVBO);
   //glBindVertexArray(skyboxVAO);
   //glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
   //glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
   //glEnableVertexAttribArray(0);
   //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

   //vector<std::string> faces
   //{
   //    FileSystem::getPath("resources/textures/skybox2/negx.jpg"),
   //    FileSystem::getPath("resources/textures/skybox2/posx.jpg"),
   //    FileSystem::getPath("resources/textures/skybox2/posy.jpg"),
   //    FileSystem::getPath("resources/textures/skybox2/negy.jpg"),
   //    FileSystem::getPath("resources/textures/skybox2/negz.jpg"),
   //    FileSystem::getPath("resources/textures/skybox2/posz.jpg")
   //};
   //unsigned int cubemapTexture = loadCubemap(faces);
   //skyboxShader.use();
   //skyboxShader.setInt("skybox", 0);


   // // screen quad VAO
   // unsigned int screenquadVAO, screenquadVBO;
   // glGenVertexArrays(1, &screenquadVAO);
   // glGenBuffers(1, &screenquadVBO);
   // glBindVertexArray(screenquadVAO);
   // glBindBuffer(GL_ARRAY_BUFFER, screenquadVBO);
   // glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
   // glEnableVertexAttribArray(0);
   // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
   // glEnableVertexAttribArray(1);
   // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    unsigned int lastQuad = 0;
    unsigned int total_rooms = 4;

    glm::vec3 origin(0.0f,0.0f,0.0f);
    glm::vec3 move1(-20.0f,0.0f,10.0f);
    glm::vec3 move2(3.05f,0.0f,0.0f);
    glm::vec3 move3(3.0f,0.0f,-30.0f);
    glm::vec3 move4(3.05f,0.0f,-60.0f);
    glm::vec3 move5(20.0f,0.0f,-63.0f);

    //camera.AddRoom(2,3,80, origin);
    //camera.AddRoom(2,20,20,move1);
    //camera.AddRoom(3,30,15,move2);
    //camera.AddRoom(2,20,10,move3);
    //camera.AddRoom(2,70,3,move4);
    //camera.AddRoom(2,3,30,move5);

    camera.AddRoom(2,80,3, origin);
    camera.AddRoom(2,20,20,move1);
    camera.AddRoom(3,15,30,move2);
    camera.AddRoom(2,10,20,move3);
    camera.AddRoom(2,3,70,move4);
    camera.AddRoom(2,30,3,move5);

    camera.Position = glm::vec3(1.0f, 1.0f, 1.0f);


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
        // bind to framebuffer and draw scene as we normally would to color texture
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        //glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        camera.Yaw   += 180.0f; // rotate the camera's yaw 180 degrees around
        camera.ProcessMouseMovement(0, 0, false); // call this to make sure it updates its camera vectors, note that we disable pitch constrains for this specific case (otherwise we can't reverse camera's pitch values)
        glm::mat4 view = camera.GetViewMatrix();
        camera.Yaw   -= 180.0f; // reset it back to its original orientation
        camera.ProcessMouseMovement(0, 0, true);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom-10), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);

        shader.use();
        shader.setMat4("projection", projection);
       // shader.setMat4("view", view);
        //shader.setVec3("viewPos", camera.Position);
        // render normal-mapped quad
        glm::mat4 model = glm::mat4(1.0f);

        glm::vec3 origin(0.0f,0.0f,0.0f);

        glm::vec3 room_left(20.0f, 0.0f, -40.0f);

        int num_lights = 8;
        int num_mirrors = 20;

        //renderMirrors(0.3, shader, 4, 3, 80, textureColorbuffer, mirror_diffuseMap, mirror_normalMap, origin);
        float room_scale = 1.0f;
        glm::vec3 viewPos = camera.Position;
        bool pass_fb = false;

        RoomSettings rsts{shader,diffuseMap,normalMap, diffuseMap,normalMap, floor_normalMap,floor_diffuseMap, normalMap, diffuseMap,
                          basicShader, num_mirrors, textureColorbuffer, door_outer_nMap, window_noborder_dMap, window_noborder_nMap, mirror_normalMap,
                          num_lights, lightCubeVAO, crystal_diffuseMap, crystal_normalMap, lightShader, view, projection, room_scale, 0.3};
        rsts.projection = projection;
        rsts.view = view;

        lastQuad += renderRoom(rsts,2,80,3,origin,viewPos, pass_fb);
        lastQuad += renderRoom(rsts,2,20,20,move1,viewPos, pass_fb);
        lastQuad += renderRoom(rsts, 3,15,30,move2,viewPos, pass_fb);
        lastQuad += renderRoom(rsts, 2,10,20,move3,viewPos, pass_fb);
        lastQuad += renderRoom(rsts, 2,3,70,move4,viewPos, pass_fb);
        lastQuad += renderRoom(rsts, 2,30,3,move5,viewPos, pass_fb);

        pass_fb = true;

        lastQuad += renderRoom(rsts,2,80,3,origin,viewPos, pass_fb);
        lastQuad += renderRoom(rsts,2,20,20,move1,viewPos, pass_fb);
        lastQuad += renderRoom(rsts, 3,15,30,move2,viewPos, pass_fb);
        lastQuad += renderRoom(rsts, 2,10,20,move3,viewPos, pass_fb);
        lastQuad += renderRoom(rsts, 2,3,70,move4,viewPos, pass_fb);
        lastQuad += renderRoom(rsts, 2,30,3,move5,viewPos, pass_fb);

        // second render pass: draw as normal
        // ----------------------------------
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glDepthFunc(GL_LEQUAL);
        view = camera.GetViewMatrix();

        shader.use();
        shader.setMat4("projection", projection);
        //shader.setMat4("view", view);
        //shader.setVec3("viewPos", viewPos);

        rsts.projection = projection;
        rsts.view = view;

        pass_fb = false;
        lastQuad += renderRoom(rsts,2,80,3,origin,viewPos,pass_fb);
        lastQuad += renderRoom(rsts,2,20,20,move1,viewPos,pass_fb);
        lastQuad += renderRoom(rsts, 3,15,30,move2,viewPos,pass_fb);
        lastQuad += renderRoom(rsts, 2,10,20,move3,viewPos,pass_fb);
        lastQuad += renderRoom(rsts, 2,3,70,move4,viewPos,pass_fb);
        lastQuad += renderRoom(rsts, 2,30,3,move5,viewPos,pass_fb);


        pass_fb = true;

        lastQuad += renderRoom(rsts,2,80,3,origin,viewPos,pass_fb);
        lastQuad += renderRoom(rsts,2,20,20,move1,viewPos,pass_fb);
        lastQuad += renderRoom(rsts, 3,15,30,move2,viewPos,pass_fb);
        lastQuad += renderRoom(rsts, 2,10,20,move3,viewPos,pass_fb);
        lastQuad += renderRoom(rsts, 2,3,70,move4,viewPos,pass_fb);
        lastQuad += renderRoom(rsts, 2,30,3,move5,viewPos,pass_fb);

            //// draw skybox as last
            //glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
            //skyboxShader.use();
            //view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
            //skyboxShader.setMat4("view", view);
            //skyboxShader.setMat4("projection", projection);
            //// skybox cube
            //glBindVertexArray(skyboxVAO);
            //glActiveTexture(GL_TEXTURE0);
            //glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
            //glDrawArrays(GL_TRIANGLES, 0, 36);
            //glBindVertexArray(0);
            //glDepthFunc(GL_LESS); // set depth function back to default
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &lightCubeVAO);
   // glDeleteVertexArrays(1, &screenquadVAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &screenquadVBO);
    glfwTerminate();
    return 0;
}

unsigned int renderRoom(RoomSettings& rsts, int height, int m, int n, glm::vec3 move_to, glm::vec3 viewPos, bool& pass_fb){

    return _renderRoom(rsts.shader, height, m, n, rsts.lr_diffuseMap, rsts.lr_normalMap, rsts.fb_diffuseMap, rsts.fb_normalMap,
                       rsts.floor_normalMap, rsts.floor_diffuseMap, rsts.ceil_normalMap, rsts.ceil_diffuseMap, move_to,
                       rsts.sz, rsts.mirror_shader, rsts.num_mirrors, rsts.door_outer_dMap,
                       rsts.door_outer_nMap, rsts.door_inner_dMap, rsts.door_inner_nMap, rsts.mirror_normalMap,
                       rsts.num_lights, rsts.lightCubeVAO, rsts.light_diffuseMap, rsts.light_normalMap,
                       rsts.lightShader, rsts.view, rsts.projection, viewPos, rsts.room_scale, pass_fb);
}

//unsigned int mirrorVAO()
unsigned int _renderRoom(Shader& shader, int height, int m, int n, unsigned int lr_dMap, unsigned int lr_nMap, unsigned int fb_dMap, unsigned int fb_nMap,
                         unsigned int f_nMap, unsigned int f_dMap, unsigned int c_nMap, unsigned int c_dMap, glm::vec3 move_to,
                float sz, Shader mirror_shader, int num_mirrors, unsigned int textureColorbuffer,
                unsigned int door_outer_nMap, unsigned int door_inner_dMap, unsigned int door_inner_nMap, unsigned int mirror_normalMap,
                int num_lights, unsigned int lightCubeVAO, unsigned int light_diffuseMap, unsigned int light_normalMap,
                Shader& lightShader, glm::mat4 view, glm::mat4 projection, glm::vec3 viewPos, float scale, bool& pass_fb){



    shader.use();
    //shader.setVec3("viewPos", viewPos);
    //shader.setMat4("view", view);
    unsigned int lastQuad = 0;

    shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

    shader.setInt("nr_point_lights", num_lights);
    for(int i = 0; i < num_lights; ++i){
        shader.setVec3("pointLights["+std::to_string(i)+"].ambient", 0.05f, 0.05f, 0.05f);
        shader.setVec3("pointLights["+std::to_string(i)+"].diffuse", 0.8f, 0.8f, 0.8f);
        shader.setVec3("pointLights["+std::to_string(i)+"].specular", 1.0f, 1.0f, 1.0f);
        shader.setFloat("pointLights["+std::to_string(i)+"].constant", 1.0f);
        shader.setFloat("pointLights["+std::to_string(i)+"].linear", 0.09f);
        shader.setFloat("pointLights["+std::to_string(i)+"].quadratic", 0.032f);
    }
    // spotLight
    shader.setVec3("spotLight.position", camera.Position);
    shader.setVec3("spotLight.direction", camera.Front);
    shader.setVec3("spotLight.ambient", 0.3f, 0.3f, 0.3f);
    shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("spotLight.constant", 1.0f);
    shader.setFloat("spotLight.linear", 0.09f);
    shader.setFloat("spotLight.quadratic", 0.032f);
    shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

    // material properties
    shader.setFloat("shininess", 52.0f);




    //float il = m / num_lights;
    //float il2 = il/2;
    //float scale = 0.1f;

    ///for(int i = 0; i < num_lights; ++i){
    ///    float il_pos = (il*i)+il2;
    ///    shader.setVec3("pointLights["+std::to_string(i)+"].position", il_pos*(1/scale),  1.6f, 0.3f);
    ///}


    float il = m / 4;
    float il2 = il/2;
    float light_scale = 0.1f;

    for(int i = 0; i < num_lights/2; ++i){
        float il_pos = (il*i)+il2;
        shader.setVec3("pointLights["+std::to_string(i)+"].position", il_pos,  0.3f, 0.3f);
    }
    for(int i = 0; i < num_lights/2; ++i){
        float il_pos = (il*i)+il2;
        shader.setVec3("pointLights["+std::to_string((num_lights/2)+i)+"].position", il_pos,  0.3f, 2.9f);
    }

    // 1st. render pass, draw objects as normal, writing to the stencil buffer
    // --------------------------------------------------------------------
   // glStencilFunc(GL_ALWAYS, 1, 0xFF);
    //glStencilMask(0xFF);

    //renderMirrors(0.3, shader, 4, n, m, textureColorbuffer, mirror_diffuseMap, mirror_normalMap, move_to);

    //glStencilMask(0x00);

    //glDepthFunc(GL_LESS);


    //glStencilMask(0x00);

    if(pass_fb){
          glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
          glStencilMask(0x00);
          lastQuad = renderCube(shader, height, m, n, lr_dMap, lr_nMap, fb_dMap, fb_nMap, f_nMap, f_dMap, c_nMap, c_dMap, move_to, viewPos, view, scale, pass_fb);
          glBindVertexArray(0);
          glStencilMask(0xFF);
          glStencilFunc(GL_ALWAYS, 0, 0xFF);
          glEnable(GL_DEPTH_TEST);
    } else {

        lastQuad = renderCube(shader, height, m, n, lr_dMap, lr_nMap, fb_dMap, fb_nMap, f_nMap, f_dMap, c_nMap, c_dMap, move_to, viewPos, view, scale, pass_fb);

        renderDoors(10, mirror_shader, shader, 9, n, m, height, textureColorbuffer, door_outer_nMap, door_inner_dMap, door_inner_nMap, move_to,
                    view, projection, viewPos);


        //  glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        //  glStencilMask(0x00);
       //  lastQuad = renderCube(shader, height, m, n, dMap, nMap, f_nMap, f_dMap, move_to, scale, pass_fb);
        //  glBindVertexArray(0);
        //  glStencilMask(0xFF);
        //  glStencilFunc(GL_ALWAYS, 0, 0xFF);
        //  glEnable(GL_DEPTH_TEST);


        // draw the lamp object
        lightShader.use();
        for(int k = 0; k < num_lights/2; ++k){
            float il_pos = (il*k)+il2;
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(light_scale)); // a smaller cube
            lightShader.setMat4("projection", projection);
            lightShader.setMat4("view", view);
            model = glm::translate(model, glm::vec3(-5*move_to.z, move_to.y, 5*move_to.x));

            model = glm::translate(model, glm::vec3( il_pos*(1/light_scale),  1.6f, 0.3f));
            lightShader.setMat4("model", model);
            //shader.setBool("isLightSrc", true);
            glBindVertexArray(lightCubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glm::mat4 model_2 = glm::mat4(1.0f);
            model_2 = glm::scale(model_2, glm::vec3(light_scale)); // a smaller cube
            //lightShader.setMat4("projection", projection);
            //lightShader.setMat4("view", view);
            model_2 = glm::translate(model_2, glm::vec3(-5*move_to.z, move_to.y, 5*move_to.x));
            model_2 = glm::translate(model_2, glm::vec3( il_pos*(1/light_scale),  1.6f, (1/light_scale)*static_cast<float>(n-0.01)));
            lightShader.setMat4("model", model_2);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        // glStencilFunc(GL_ALWAYS, 1, 0xFF);
        //glStencilMask(0xFF);

        //renderDoors(10, mirror_shader, shader, 9, n, m, height, textureColorbuffer, door_outer_nMap, door_inner_dMap, door_inner_nMap, move_to,
        //           view, projection);


        renderMirrors(0.6, mirror_shader, num_mirrors, n, m, textureColorbuffer, mirror_normalMap, move_to, view, camera.Position, projection);

        //renderDoors(0.8, mirror_shader, 9, n, m,textureColorbuffer, mirror_normalMap, move_to, view, projection);

        //    glStencilMask(0x00);
        //renderMirrors(0.8, shader, num_mirrors, n, m, textureColorbuffer, mirror_diffuseMap, mirror_normalMap, move_to);


        //   glStencilMask(0xFF);


    }

    return lastQuad != 0 ? 1 : 0;
}

unsigned int cubeVAO = 0;
unsigned int cubeVBO;
unsigned int renderCubeLayer(Shader& shader, int layer, unsigned int lr_diffuseMap, unsigned int lr_normalMap, unsigned int fb_diffuseMap, unsigned int fb_normalMap,
                     int m, int n, glm::vec3 move_to, glm::vec3 viewPos, glm::mat4 view, float scale, bool& pass_fb)
{
    unsigned int lastQuad = 0;
    glm::vec3 lightPos(10.5f, 1.0f, 0.3f);        /////////////////
    glm::mat4 model = glm::mat4(1.0f);

    float quad_sz = 1.5;
    //model = glm::rotate(model, glm::radians((float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0))); // rotate the quad to show normal mapping from multiple directions

    if(pass_fb){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fb_diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, fb_normalMap);
        glm::mat4 model_f = glm::translate(model, glm::vec3(0.0f,0.0f,0.0f));
        // model_f = glm::translate(model_f, move_to);
        glm::mat4 model_b = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model_b = glm::translate(model_b, glm::vec3(0.0f,0.0f,-scale*static_cast<float>(n)));
        //model_f = glm::translate(model_f, glm::vec3(-move_to.x,-move_to.z, move_to.y));
        //model_b = glm::translate(model_b, glm::vec3(move_to.x , move_to.z, move_to.y));

        model_f = glm::translate(model_f, glm::vec3(-move_to.z,move_to.y, move_to.x));
        model_b = glm::translate(model_b, glm::vec3(move_to.z,move_to.y, -move_to.x));

        for(int i = 0; i < m; ++i){
            glm::mat4 model_ff = glm::translate(model_f, glm::vec3(1.0f*i*scale,scale*static_cast<float>(layer),0.0f));
            shader.setMat4("model", glm::scale(model_ff, glm::vec3(scale)));
            lastQuad = renderQuad();

            glm::mat4 model_bb = glm::translate(model_b, glm::vec3(-1.0f*i*scale,scale*static_cast<float>(layer),0.0f));
            shader.setMat4("model", glm::scale(model_bb, glm::vec3(scale)));
            lastQuad = renderQuad();
        }
    } else {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, lr_diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, lr_normalMap);
        glm::mat4 model_l = glm::translate(model, glm::vec3(0.0f,0.0f,0.0f));
        model_l = glm::rotate(model_l, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 model_r = glm::translate(model, glm::vec3(0.0f,0.0f,0.0f));
        model_r = glm::rotate(model_r, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model_r = glm::translate(model_r, glm::vec3(0.0f,0.0f,-scale*static_cast<float>(m)));
        model_l = glm::translate(model_l, glm::vec3(-scale*0.5f,scale*0.0f,-scale*0.5f));
        model_r = glm::translate(model_r, glm::vec3(scale*0.5f,scale*0.0f,scale*0.5f));

        model_l = glm::translate(model_l, glm::vec3(-move_to.x,move_to.y,-move_to.z));
        model_r = glm::translate(model_r, glm::vec3(move_to.x,move_to.y,move_to.z));

        //  model_l = glm::translate(model_l, move_to);
        // model_r = glm::translate(model_r, move_to);
        for(int j = 0; j < n; ++j){

            glm::mat4 model_ll = glm::translate(model_l, glm::vec3(-1.0f*j*scale,scale*static_cast<float>(layer),0.0f));
            shader.setMat4("model", glm::scale(model_ll, glm::vec3(scale)));
            shader.setVec3("viewPos", viewPos);
            shader.setMat4("view", view);
            lastQuad = renderQuad();

            glm::mat4 model_rr = glm::translate(model_r, glm::vec3(1.0f*j*scale,scale*static_cast<float>(layer),0.0f));
            shader.setVec3("viewPos", viewPos);
            shader.setMat4("view", view);
            shader.setMat4("model", glm::scale(model_rr, glm::vec3(scale)));
            lastQuad = renderQuad();
        }
        //pass_fb = true;
    }

    return lastQuad;
}

unsigned int renderCube(Shader& shader, int height, int m, int n, unsigned int lr_dMap, unsigned int lr_nMap, unsigned int fb_dMap, unsigned int fb_nMap,
                        unsigned int f_nMap, unsigned int f_dMap, unsigned int c_nMap, unsigned int c_dMap, glm::vec3 move_to, glm::vec3 viewPos, glm::mat4 view, float scale, bool& pass_fb){

    //for(int i = 0; i < 4; ++i){
    //    shader.setVec3("pointLights["+std::to_string(i)+"].position", 3*i,  0.6f, 0.3f);
    //}
    //float il = m / num_lights;

    unsigned int lastQuad = 0;
    float quad_sz = 1.5;
    glm::vec3 lightPos(10.5f, 1.0f, 0.3f);        /////////////////
    glm::mat4 model = glm::mat4(1.0f);
    //model = glm::rotate(model, glm::radians((float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0))); // rotate the quad to show normal mapping from multiple directions
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setVec3("viewPos", viewPos);
    shader.setVec3("lightPos", lightPos);


    //glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    //glStencilMask(0x00);

    for(int k = 0; k < height; ++k){
        //glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        //glStencilMask(0x00);
        renderCubeLayer(shader, k, lr_dMap, lr_nMap, fb_dMap, fb_nMap, m, n, move_to, viewPos, view, scale, pass_fb);
        //glBindVertexArray(0);
        //glStencilMask(0xFF);
        //glStencilFunc(GL_ALWAYS, 0, 0xFF);
        //glEnable(GL_DEPTH_TEST);
        //renderCubeLayer(shader, k, dMap, nMap, m, n, move_to, scale, pass_fb);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, c_dMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, c_nMap);

    //glm::mat4 model_fl = glm::translate(model, glm::vec3(1.0f,0.5f,0.5f));
    if(!pass_fb){
        for(int i = 0; i < m; ++i){
            for(int j = 0; j < n; ++j){
                glm::mat4 model_cl = glm::translate(model, glm::vec3(0.0f,0.0f,0.0f));
                model_cl = glm::rotate(model_cl, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                model_cl = glm::translate(model_cl, glm::vec3(0.0f,scale*0.5f,-scale*0.5));
                model_cl = glm::translate(model_cl, glm::vec3(scale*static_cast<float>(i),scale*static_cast<float>(j),-static_cast<float>((scale*height)-1)));
                model_cl = glm::translate(model_cl, glm::vec3(-move_to.z, move_to.x, move_to.y));
                model_cl = glm::scale(model_cl, glm::vec3(scale));
                shader.setVec3("viewPos", viewPos);
                shader.setMat4("view", view);
                shader.setMat4("model", model_cl);
                lastQuad = renderQuad();
            }
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, f_dMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, f_nMap);

        for(int i = 0; i < m; ++i){
            for(int j = 0; j < n; ++j){
                glm::mat4 model_fl = glm::translate(model, glm::vec3(0.0f,0.0f,0.0f));
                model_fl = glm::rotate(model_fl, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                model_fl = glm::rotate(model_fl, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                model_fl = glm::translate(model_fl, glm::vec3(0.0f,scale*0.5f,-scale*0.5));
                model_fl = glm::translate(model_fl, glm::vec3(-scale*static_cast<float>(i),scale*static_cast<float>(j),0.0f));
                model_fl = glm::translate(model_fl, glm::vec3(move_to.z, move_to.x, move_to.y));
                model_fl = glm::scale(model_fl, glm::vec3(scale));
                shader.setVec3("viewPos", viewPos);
                shader.setMat4("view", view);
                shader.setMat4("model", model_fl);
                lastQuad = renderQuad();
            }
        }
    }
    //glBindVertexArray(0);
    //glStencilMask(0xFF);
    //glStencilFunc(GL_ALWAYS, 0, 0xFF);
    //glEnable(GL_DEPTH_TEST);
    return lastQuad;
}


// renders a 1x1 quad in NDC with manually calculated tangent vectors
// ------------------------------------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void _renderQuad()
{

    if(quadVAO == 0){
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);


        // positions
        glm::vec3 pos1(-0.5f,  0.5f, 0.0f);
        glm::vec3 pos2(-0.5f, -0.5f, 0.0f);
        glm::vec3 pos3( 0.5f, -0.5f, 0.0f);
        glm::vec3 pos4( 0.5f,  0.5f, 0.0f);

        glm::vec3 pos5(-0.5f, 0.0f,  0.5f);
        glm::vec3 pos6(-0.5f, 0.0f, -0.5f);
        glm::vec3 pos7( 0.5f, 0.0f, -0.5f);
        glm::vec3 pos8( 0.5f, 0.0f,  0.5f);

        glm::vec3 pos9( -0.5f, 0.0f,  0.5f);
        glm::vec3 pos10(-0.5f, 0.0f, -0.5f);
        glm::vec3 pos11( 0.5f, 0.0f, -0.5f);
        glm::vec3 pos12( 0.5f, 0.0f,  0.5f);


        // texture coordinates
        glm::vec2 uv1(0.0f, 1.0f);
        glm::vec2 uv2(0.0f, 0.0f);
        glm::vec2 uv3(1.0f, 0.0f);
        glm::vec2 uv4(1.0f, 1.0f);

        // normal vector
        glm::vec3 nm(0.0f, 0.0f, 1.0f);

        glm::vec3 nm2(0.0f, 1.0f, 0.0f);

        glm::vec3 nm3(0.0f, 1.0f, 0.0f);

        // calculate tangent/bitangent vectors of both triangles
        glm::vec3 tangent1, bitangent1;
        glm::vec3 tangent2, bitangent2;

        glm::vec3 tangent3, bitangent3;
        glm::vec3 tangent4, bitangent4;

        glm::vec3 tangent5, bitangent5;
        glm::vec3 tangent6, bitangent6;

        // triangle 1
        // ----------
        glm::vec3 edge1 = (pos2 - pos1);
        glm::vec3 edge2 = (pos3 - pos1);
        glm::vec2 deltaUV1 = (uv2 - uv1);
        glm::vec2 deltaUV2 = (uv3 - uv1);

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
                pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
                };
        // configure plane VAO

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
}
unsigned int renderQuad()
{
  //  unsigned int quadVAO = 0;
   // unsigned int quadVBO;

    _renderQuad();
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    return quadVAO;
}

void renderWdws(float sz, Shader shader, int num_doors, int n, int m, int h,
                unsigned int mirror_diffuseMap, unsigned int mirror_normalMap, glm::vec3 move_to,
                glm::mat4 view, glm::mat4 projection){

}

bool flag = false;
void renderDoors(float width, Shader shader, Shader normal_shader, int num_doors, int n, int m, int h,
                 unsigned int outer_diffuseMap, unsigned int outer_normalMap,
                 unsigned int inner_diffuseMap, unsigned int inner_normalMap,
                 glm::vec3 move_to,
                 glm::mat4 view, glm::mat4 projection,
                 glm::vec3 viewPos)
{

    int w = 0.3*(m/num_doors);
    glm::vec3 move_t = move_to;
    //renderCube(shader, 0.6, w, 0.3, mirror_diffuseMap, mirror_normalMap,
     //          mirror_normalMap, mirror_diffuseMap, move_to);

     float scale = 0.1f;
     float border_scale = 0.02;
     move_to.y -= 0.5*(1-scale);

     glStencilMask(0xFF);
     glStencilFunc(GL_ALWAYS, 1, 0xFF);
     //glDisable(GL_DEPTH_TEST);
     //glDepthFunc(GL_LESS);
     shader.use();
     shader.setVec3("viewPos", viewPos);
     shader.setMat4("view", view);
     shader.setMat4("projection", projection);

     shader.setVec3("dirLight.direction", -6.2f, -5.0f, -6.3f);
     shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
     shader.setVec3("dirLight.diffuse", 5.4f, 5.4f, 5.4f);
     shader.setVec3("dirLight.specular", 5.5f, 5.5f, 5.5f);


     bool pass_fb = false;
     renderCube(shader, 15, width, 1,
                outer_diffuseMap, outer_normalMap, inner_diffuseMap, inner_normalMap,
                outer_normalMap, outer_diffuseMap,  outer_normalMap, outer_diffuseMap, move_to, viewPos, view, scale-border_scale, pass_fb);
     pass_fb = true;
     renderCube(shader, 15, width, 1,
                outer_diffuseMap, outer_normalMap, inner_diffuseMap, inner_normalMap,
                outer_normalMap, outer_diffuseMap,  outer_normalMap, outer_diffuseMap, move_to, viewPos, view, scale-border_scale, pass_fb);
     //glDepthFunc(GL_ALWAYS);
     glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
     glStencilMask(0x00);
     //lglDisable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);

    normal_shader.use();
    normal_shader.setVec3("viewPos", viewPos);
    normal_shader.setMat4("view", view);
    unsigned int lastQuad = 0;

    normal_shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    normal_shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    normal_shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    normal_shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

    normal_shader.setInt("nr_point_lights", 8);
    for(int i = 0; i < 8; ++i){
        shader.setVec3("pointLights["+std::to_string(i)+"].ambient", 0.05f, 0.05f, 0.05f);
        shader.setVec3("pointLights["+std::to_string(i)+"].diffuse", 0.8f, 0.8f, 0.8f);
        shader.setVec3("pointLights["+std::to_string(i)+"].specular", 1.0f, 1.0f, 1.0f);
        shader.setFloat("pointLights["+std::to_string(i)+"].constant", 1.0f);
        shader.setFloat("pointLights["+std::to_string(i)+"].linear", 0.09f);
        shader.setFloat("pointLights["+std::to_string(i)+"].quadratic", 0.032f);
    }
    // spotLight
    normal_shader.setVec3("spotLight.position", camera.Position);
    normal_shader.setVec3("spotLight.direction", camera.Front);
    normal_shader.setVec3("spotLight.ambient", 0.3f, 0.3f, 0.3f);
    normal_shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    normal_shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    normal_shader.setFloat("spotLight.constant", 1.0f);
    normal_shader.setFloat("spotLight.linear", 0.09f);
    normal_shader.setFloat("spotLight.quadratic", 0.032f);
    normal_shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    normal_shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

    // material properties
    normal_shader.setFloat("shininess", 52.0f);


     //if(!flag){
     //    std::cout << "moveto z " << move_to.z << '\n';
     //   // std::cout << "w*scale " << scale*(move_to.z + width) << '\n';
     //    //std::cout << "w" << (scale-0.02)*(move_to.z + width) << '\n';
     //    std::cout << (move_to.z + width)*(border_scale/2) << '\n';
     //    flag = true;
     //}

    // move_to.z *= (1+border_scale);
     move_to.z += width*(border_scale/2);
     //move_to.z += (width*border_scale)/2;
     pass_fb = false;
     renderCube(normal_shader, 13, width, 1, outer_diffuseMap, outer_normalMap, outer_diffuseMap, outer_normalMap,
                outer_normalMap, outer_diffuseMap, outer_normalMap, outer_diffuseMap, move_to, viewPos, view, scale, pass_fb);
     pass_fb = true;
     renderCube(normal_shader, 13, width, 1, outer_diffuseMap, outer_normalMap, outer_diffuseMap, outer_normalMap,
                outer_normalMap, outer_diffuseMap, outer_normalMap, outer_diffuseMap, move_to, viewPos, view, scale, pass_fb);

     move_to.x -= 0.1;
     move_to.z -= width*(border_scale/4);
     pass_fb = false;
     renderCube(normal_shader, 13, width, 1, outer_diffuseMap, outer_normalMap, outer_diffuseMap, outer_normalMap,
                outer_normalMap, outer_diffuseMap, outer_normalMap, outer_diffuseMap, move_to, viewPos, view, scale, pass_fb);
     pass_fb = true;
     renderCube(normal_shader, 13, width, 1, outer_diffuseMap, outer_normalMap, outer_diffuseMap, outer_normalMap,
                outer_normalMap, outer_diffuseMap, outer_normalMap, outer_diffuseMap, move_to, viewPos, view, scale, pass_fb);
    // glBindVertexArray(0);
    // glStencilMask(0x00);
     //glStencilFunc(GL_ALWAYS, 0, 0x00);

     glBindVertexArray(0);
     glStencilMask(0xFF);
     glStencilFunc(GL_ALWAYS, 0, 0xFF);
     glEnable(GL_DEPTH_TEST);

    //move_t.z += w;
    //for(int i = 0; i < num_doors; ++i){
    //    move_t.z += (m/num_doors);
    //    renderCube(shader, 0.6, w, 0.3, mirror_diffuseMap, mirror_normalMap,
    //               mirror_normalMap, mirror_diffuseMap, move_t);
    //}
}

void renderMirrors(float sz, Shader shader, int num_mirrors, int n, int m, unsigned int textureColorbuffer,
                   unsigned int mirror_normalMap, glm::vec3 move_to, glm::mat4 view, glm::vec3 viewPos, glm::mat4 projection)
{

    shader.use();
    shader.setVec3("viewPos", viewPos);

    shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

    shader.setInt("nr_point_lights", 8);
    for(int i = 0; i < 8; ++i){
        shader.setVec3("pointLights["+std::to_string(i)+"].ambient", 0.05f, 0.05f, 0.05f);
        shader.setVec3("pointLights["+std::to_string(i)+"].diffuse", 0.8f, 0.8f, 0.8f);
        shader.setVec3("pointLights["+std::to_string(i)+"].specular", 1.0f, 1.0f, 1.0f);
        shader.setFloat("pointLights["+std::to_string(i)+"].constant", 1.0f);
        shader.setFloat("pointLights["+std::to_string(i)+"].linear", 0.09f);
        shader.setFloat("pointLights["+std::to_string(i)+"].quadratic", 0.032f);
    }
    // material properties
    shader.setFloat("shininess", 100.0f);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    //// spotLight
    //shader.setVec3("spotLight.position", camera.Position);
    //shader.setVec3("spotLight.direction", camera.Front);
    //shader.setVec3("spotLight.ambient", 0.3f, 0.3f, 0.3f);
    //shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    //shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    //shader.setFloat("spotLight.constant", 1.0f);
    //shader.setFloat("spotLight.linear", 0.09f);
    //shader.setFloat("spotLight.quadratic", 0.032f);
    //shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    //shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


    float scale = sz;
    float il = n/num_mirrors;
    float il2 = il/2;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-move_to.z,move_to.y,-move_to.x));
    model = glm::translate(model, glm::vec3(0.0f, 0.05f,static_cast<float>(m)+0.1));
    for(int k = 0; k < num_mirrors; ++k){
        float il_pos = (il*k)+il2;
        float c;
        _renderQuad();
        glBindVertexArray(quadVAO);
      //  glm::mat4 model_b = glm::translate(model, glm::vec3(-il_pos,  0.0f,0.0f));
        glm::mat4 model_b = glm::translate(model, glm::vec3(-il_pos,  0.0f,0.0f));
        model_b = glm::scale(model_b, glm::vec3(scale));
        //static_cast<float>(n-0.01)
        shader.setMat4("model", model_b);
        //glActiveTexture(GL_TEXTURE0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mirror_normalMap);

        //glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
        //glBindTexture(GL_TEXTURE_2D, concreteTexture);	// use the color attachment texture as the texture of the quad plane

        glDrawArrays(GL_TRIANGLES, 0, 6);
        //glBindVertexArray(0);
    }
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

    auto Position = camera.Position;
    //std::cout << Position.x << ' ' << Position.y << ' ' << Position.z << '\n';
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

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
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


// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
