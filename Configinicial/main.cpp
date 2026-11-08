#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs


// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models



// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "SOIL2/SOIL2.h"


// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();
void updateBusAnimation();
bool loadBusWheelData(const char* filePath);


//================= draw function =====================================
GLuint CreateSolidColorTexture(GLubyte r, GLubyte g, GLubyte b);
GLuint LoadDiffuseTexture(const char* path);
void drawCube(GLint modelLoc, GLuint VAO, const glm::mat4& parent, glm::vec3 position, glm::vec3 size, GLuint texture, float yaw = 0.0f);
void drawField(GLint modelLoc, GLint textureModeLoc, GLuint VAO, const glm::mat4& scene, GLuint texGrass, GLuint texWhite);
void drawPerimeter(GLint modelLoc, GLuint VAO, const glm::mat4& scene,
    GLuint texWall, GLuint texFloor, GLuint texAccent);
void drawBleachers(GLint modelLoc, GLuint VAO, const glm::mat4& scene, GLuint texConcrete, GLuint texSeats);
void drawGoal(GLint modelLoc, GLuint VAO, const glm::mat4& scene, float x, float direction, GLuint texPosts, GLuint texNet);
void drawPlayer(GLint modelLoc, GLuint VAO, const glm::mat4& scene,
    glm::vec3 position, float yaw, GLuint texJersey,
    GLuint texShorts, GLuint texSkin, GLuint texShoes,
    float kickAngle = 0.0f, float diveAngle = 0.0f);

struct BallMesh {
    GLuint VAO = 0, VBO = 0;
    GLsizei vertexCount = 0;
};

void createBall(BallMesh& whiteMesh, BallMesh& blackMesh);
void drawBall(GLint modelLoc, const glm::mat4& scene,
    const BallMesh& whiteMesh, const BallMesh& blackMesh,
    GLuint texWhite, GLuint texBlack, float rotation, glm::vec3 position);
void deleteBallMesh(BallMesh& mesh);
void drawSolidSphere(GLint modelLoc, const glm::mat4& parent, glm::vec3 position,
    glm::vec3 size, const BallMesh& whiteMesh, const BallMesh& blackMesh, GLuint texture);
void drawTrees(GLint modelLoc, GLint textureModeLoc, GLuint VAO, const glm::mat4& scene,
    const BallMesh& whiteMesh, const BallMesh& blackMesh,
    GLuint texBark, GLuint texLeavesDark, GLuint texLeavesLight);
void drawBird(GLint modelLoc, GLuint VAO, const glm::mat4& scene,
    const BallMesh& whiteMesh, const BallMesh& blackMesh,
    GLuint texBird, GLuint texBeak, float elapsed);
void drawFireworks(GLint modelLoc, GLuint VAO, const glm::mat4& scene,
    float elapsed, GLuint texGold, GLuint texBlue, GLuint texRed, GLuint texWhite);
void drawFloodlightTowers(GLint modelLoc, GLuint VAO, const glm::mat4& scene, GLuint texMetal);
void drawFloodlightPanels(GLint modelLoc, GLuint VAO);
void drawBroadcastCameraModels(GLint modelLoc, Shader& lightingShader, Model* cameraBase, Model* cameraHead);
glm::mat4 getBroadcastView();
glm::vec3 getBroadcastPosition();
void ResetPenalty();
float RandomFloat(float minValue, float maxValue);


// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(32.0f, 2.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;
bool fireworksActive = false;
float fireworksStartTime = 0.0f;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(-40.0f, 17.5f, 49.0f),
    glm::vec3(0.0f,0.0f, 0.0f),
    glm::vec3(0.0f,0.0f,  0.0f),
    glm::vec3(0.0f,0.0f, 0.0f)
};

float vertices[] = {
     -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};



glm::vec3 Light1 = glm::vec3(0);
enum PenaltyState { READY, PREPARING, KICKING, FLYING, FINISHED };
PenaltyState penaltyState = READY;
float stateTime = 0.0f;
float rotBall = 0.0f;
float kickAngle = 0.0f;
float keeperDive = 0.0f;
float keeperPositionZ = 0.0f;
float targetZ = 0.0f;
float targetY = 0.9f;
int shotSide = 0;
int keeperSide = 0;
int goals = 0;
int saves = 0;
int misses = 0;
bool penaltyRequested = false;
bool keeperCommitted = false;
glm::vec3 ballPosition(41.5f, 0.11f, 0.0f);
std::string penaltyResult = "READY";
int activeCamera = 4;
const glm::vec3 broadcastCameraPositions[8] = {
    glm::vec3(40.0f, 32.0f, -13.0f),
    glm::vec3(40.0f, 32.0f, 13.0f),
    glm::vec3(34.0f, 6.0f, -25.0f),
    glm::vec3(35.0f, 5.0f, 0.0f),
    glm::vec3(34.0f, 6.0f, 25.0f),
    glm::vec3(49.0f, 7.0f, -39.0f),
    glm::vec3(49.0f, 7.0f, 39.0f),
    glm::vec3(0.0f, 21.0f, -44.0f)
};
const glm::vec3 facadeCameraPosition(24.0f, 10.0f, -95.0f);
glm::vec3 trackedBallPosition(41.5f, 0.11f, 0.0f);
glm::vec3 cameraHeadPivot(0.0f, 1.5f, 0.0f);
const float cameraModelScale = 0.65f;
const float cameraModelForwardYawDegrees = 0.0f;
const std::string cameraAssetDirectory = "Models/Camaras/";
const glm::vec3 floodlightPositions[4] = {
    glm::vec3(-40.0f, 0.0f, -49.0f),
    glm::vec3(-40.0f, 0.0f, 49.0f),
    glm::vec3(40.0f, 0.0f, -49.0f),
    glm::vec3(40.0f, 0.0f, 49.0f)
};

enum BusState { BUS_WAITING, BUS_STRAIGHT, BUS_TURNING, BUS_LEAVING, BUS_COMPLETED };
BusState busState = BUS_WAITING;
struct WheelData {
    glm::vec3 position;
    float radius;
    bool isFrontWheel;
};
WheelData busWheelData[6];
const float busPi = 3.14159265358979323846f;
const float busSpeed = 3.0f;
const float busInitialDistance = 12.0f;
const float busTurnRadius = 9.0f;
const float busFinalDistance = 10.0f;
const float busTurnLength = busTurnRadius * busPi * 0.5f;
const float busRouteLength = busInitialDistance + busTurnLength + busFinalDistance;
glm::vec3 busStartPosition(66.0f, 0.0f, -13.0f);
glm::vec3 busPosition = busStartPosition;
float busDistanceTraveled = 0.0f;
float busHeading = 0.0f;
float busSteeringAngle = 0.0f;
bool busRunning = false;
bool busAssetsReady = false;

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Soccer - Penalty kick", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    srand(static_cast<unsigned int>(time(nullptr)));



    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    //models
    const char* busFiles[] = {
        "Models/Autobus/carroceria.obj",
        "Models/Autobus/llanta_1.obj",
        "Models/Autobus/llanta_2.obj",
        "Models/Autobus/llanta_3.obj",
        "Models/Autobus/llanta_4obj.obj",
        "Models/Autobus/llanta_5.obj",
        "Models/Autobus/llanta_6.obj"
    };
    busAssetsReady = loadBusWheelData("Models/Autobus/ruedas.txt");
    if (busAssetsReady) {
        float lowestWheelPoint = busWheelData[0].position.y - busWheelData[0].radius;
        for (int i = 1; i < 6; ++i)
            lowestWheelPoint = std::min(lowestWheelPoint, busWheelData[i].position.y - busWheelData[i].radius);
        busStartPosition.y = -lowestWheelPoint;
        busPosition = busStartPosition;
    }
    for (const char* path : busFiles) {
        std::ifstream file(path);
        if (!file) {
            std::cerr << "Missing bus model: " << path << std::endl;
            busAssetsReady = false;
        }
    }
    std::unique_ptr<Model> busBodyModel;
    std::unique_ptr<Model> busWheelModels[6];
    if (busAssetsReady) {
        busBodyModel.reset(new Model(const_cast<char*>(busFiles[0])));
        for (int i = 0; i < 6; ++i)
            busWheelModels[i].reset(new Model(const_cast<char*>(busFiles[i + 1])));
    }

    std::unique_ptr<Model> broadcastCameraBase;
    std::unique_ptr<Model> broadcastCameraHead;
    const std::string cameraBasePath = cameraAssetDirectory + "cameraBase.obj";
    const std::string cameraHeadPath = cameraAssetDirectory + "cameraHead.obj";
    {
        std::ifstream baseFile(cameraBasePath.c_str());
        std::ifstream headFile(cameraHeadPath.c_str());
        if (baseFile.good() && headFile.good()) {
            broadcastCameraBase.reset(new Model(const_cast<char*>(cameraBasePath.c_str())));
            broadcastCameraHead.reset(new Model(const_cast<char*>(cameraHeadPath.c_str())));
            std::ifstream pivotFile((cameraAssetDirectory + "camera_pivot.txt").c_str());
            if (pivotFile && !(pivotFile >> cameraHeadPivot.x >> cameraHeadPivot.y >> cameraHeadPivot.z)) {
                cameraHeadPivot = glm::vec3(0.0f, 1.5f, 0.0f);
                std::cerr << "Invalid camera_pivot.txt; using default pivot." << std::endl;
            }
        }
        else {
            std::cout << "Camera OBJ parts not found; views 1-8 remain available." << std::endl;
        }
    }

    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // Single pixel solid color textures 
    GLuint texGrass = LoadDiffuseTexture("Models/Textures/Grass005_Color.jpg");
    if (!texGrass) texGrass = CreateSolidColorTexture(35, 145, 55);
    GLuint texWhite = CreateSolidColorTexture(245, 245, 245);
    GLuint texBlack = CreateSolidColorTexture(20, 20, 20);
    GLuint texBlue = CreateSolidColorTexture(25, 70, 210);
    GLuint texRed = CreateSolidColorTexture(205, 45, 45);
    GLuint texShorts = CreateSolidColorTexture(35, 35, 55);
    GLuint texSkin = CreateSolidColorTexture(205, 150, 105);
    GLuint texShoes = CreateSolidColorTexture(30, 30, 30);
    GLuint texPosts = CreateSolidColorTexture(240, 240, 240);
    GLuint texNet = CreateSolidColorTexture(178, 190, 190);
    GLuint texSpecular = CreateSolidColorTexture(35, 35, 35);
    GLuint texBark = LoadDiffuseTexture("Models/Textures/pine_bark_diff_1k.jpg");
    if (!texBark) texBark = CreateSolidColorTexture(110, 73, 43);
    GLuint texLeavesDark = LoadDiffuseTexture("Models/Textures/LeafSet024_Dark.jpg");
    if (!texLeavesDark) texLeavesDark = CreateSolidColorTexture(35, 102, 47);
    GLuint texLeavesLight = LoadDiffuseTexture("Models/Textures/LeafSet024_Light.jpg");
    if (!texLeavesLight) texLeavesLight = CreateSolidColorTexture(62, 141, 67);
    GLuint texBird = CreateSolidColorTexture(83, 89, 96);
    GLuint texBeak = CreateSolidColorTexture(241, 167, 52);
    GLuint texGold = CreateSolidColorTexture(255, 206, 56);
    GLuint texFloodlight = CreateSolidColorTexture(130, 137, 145);


    // spherical ball 
    BallMesh whiteMesh, blackMesh;
    createBall(whiteMesh, blackMesh);

    // Set texture units
    lightingShader.Use();
    GLint textureModeLoc = glGetUniformLocation(lightingShader.Program, "proceduralTextureMode");
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texSpecular);
    glActiveTexture(GL_TEXTURE0);

    glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 300.0f);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {

        // Calculate deltatime of current frame
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        DoMovement();
        Animation();
        updateBusAnimation();
        float trackingAmount = 1.0f - std::exp(-4.0f * std::max(0.0f, deltaTime));
        trackedBallPosition = glm::mix(trackedBallPosition, ballPosition, trackingAmount);
        std::string title = "Penalty - " + penaltyResult + " | Goals: " + std::to_string(goals) +
            " | Saves: " + std::to_string(saves) + " | Misses: " + std::to_string(misses) + " | N: Kick  R: Reset";
        glfwSetWindowTitle(window, title.c_str());

        // Clear the colorbuffer
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // OpenGL options
        glEnable(GL_DEPTH_TEST);


        glm::mat4 modelTemp = glm::mat4(1.0f); //Temp



        // Use cooresponding shader when setting uniforms/drawing objects
        lightingShader.Use();

        glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
        glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glm::vec3 viewPosition = getBroadcastPosition();
        glUniform3f(viewPosLoc, viewPosition.x, viewPosition.y, viewPosition.z);


        // Directional light
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);


        // Point light 1
        glm::vec3 lightColor;
        lightColor.x = abs(sin(glfwGetTime() * Light1.x));
        lightColor.y = abs(sin(glfwGetTime() * Light1.y));
        lightColor.z = sin(glfwGetTime() * Light1.z);
        glm::vec3 stadiumPointPosition = pointLightPositions[0];
        glm::vec3 stadiumDiffuse = glm::vec3(1.4f, 1.3f, 1.1f) + 0.35f * lightColor;
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"),
            stadiumPointPosition.x, stadiumPointPosition.y, stadiumPointPosition.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.24f, 0.23f, 0.20f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"),
            stadiumDiffuse.x, stadiumDiffuse.y, stadiumDiffuse.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.7f, 0.7f, 0.6f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.006f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.0005f);

        // SpotLight
        glm::vec3 stadiumSpotPosition = floodlightPositions[2] + glm::vec3(0.0f, 17.5f, 0.0f);
        glm::vec3 stadiumSpotDirection = glm::normalize(glm::vec3(42.0f, 0.0f, 0.0f) - stadiumSpotPosition);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"),
            stadiumSpotPosition.x, stadiumSpotPosition.y, stadiumSpotPosition.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"),
            stadiumSpotDirection.x, stadiumSpotDirection.y, stadiumSpotDirection.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.1f, 0.1f, 0.09f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 1.5f, 1.4f, 1.2f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.6f, 0.6f, 0.5f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.006f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.0005f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(42.0f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(50.0f)));

        // Set material properties
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

        // Create camera transformations
        glm::mat4 view;
        view = getBroadcastView();
        projection = glm::perspective(glm::radians(activeCamera == 1 || activeCamera == 2 ? 54.0f : camera.GetZoom()),
            (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 300.0f);

        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


        glm::mat4 model(1);



        //Carga de modelo 
        view = getBroadcastView();
        model = glm::mat4(1);
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

        modelTemp = model;
        drawPerimeter(modelLoc, VAO, modelTemp, texFloodlight, texShorts, texBlue);
        drawField(modelLoc, textureModeLoc, VAO, modelTemp, texGrass, texWhite);
        drawBleachers(modelLoc, VAO, modelTemp, texFloodlight, texBlue);
        drawGoal(modelLoc, VAO, modelTemp, -52.5f, -1.0f, texPosts, texNet);
        drawGoal(modelLoc, VAO, modelTemp, 52.5f, 1.0f, texPosts, texNet);

        drawPlayer(modelLoc, VAO, modelTemp,
            glm::vec3(40.4f, 0.0f, 0.0f), 90.0f,
            texBlue, texShorts, texSkin, texShoes, kickAngle);

        drawPlayer(modelLoc, VAO, modelTemp,
            glm::vec3(51.2f, 0.0f, keeperPositionZ), -90.0f,
            texRed, texShorts, texSkin, texShoes, 0.0f, keeperDive);

        drawBall(modelLoc, modelTemp, whiteMesh, blackMesh,
            texWhite, texBlack, rotBall, ballPosition);
        drawTrees(modelLoc, textureModeLoc, VAO, modelTemp, whiteMesh, blackMesh,
            texBark, texLeavesDark, texLeavesLight);
        drawFloodlightTowers(modelLoc, VAO, modelTemp, texFloodlight);
        drawBroadcastCameraModels(modelLoc, lightingShader,
            broadcastCameraBase.get(), broadcastCameraHead.get());
        drawBird(modelLoc, VAO, modelTemp, whiteMesh, blackMesh,
            texBird, texBeak, currentFrame);
        if (fireworksActive) {
            float elapsed = currentFrame - fireworksStartTime;
            if (elapsed < 4.8f)
                drawFireworks(modelLoc, VAO, modelTemp, elapsed,
                    texGold, texBlue, texRed, texWhite);
            else
                fireworksActive = false;
        }
        if (busAssetsReady) {
            glm::mat4 busMatrix(1.0f);
            busMatrix = glm::translate(busMatrix, busPosition);
            busMatrix = glm::rotate(busMatrix, busHeading, glm::vec3(0.0f, 1.0f, 0.0f));
            const glm::vec3 busPivot(0.36695f, 0.0f, 3.16461f);
            busMatrix = glm::translate(busMatrix, -busPivot);
            glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(busMatrix));
            busBodyModel->Draw(lightingShader);
            for (int i = 0; i < 6; ++i) {
                const glm::vec3 wheelCenter = busWheelData[i].position;
                glm::mat4 wheelMatrix = glm::translate(busMatrix, wheelCenter);
                if (busWheelData[i].isFrontWheel)
                    wheelMatrix = glm::rotate(wheelMatrix, busSteeringAngle, glm::vec3(0.0f, 1.0f, 0.0f));
                const float rollingAngle = -busDistanceTraveled / busWheelData[i].radius;
                wheelMatrix = glm::rotate(wheelMatrix, rollingAngle, glm::vec3(1.0f, 0.0f, 0.0f));
                wheelMatrix = glm::translate(wheelMatrix, -wheelCenter);
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(wheelMatrix));
                busWheelModels[i]->Draw(lightingShader);
            }
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texSpecular);
            glActiveTexture(GL_TEXTURE0);
        }
        lampShader.Use();
        GLint lampModelLoc = glGetUniformLocation(lampShader.Program, "model");
        GLint lampViewLoc = glGetUniformLocation(lampShader.Program, "view");
        GLint lampProjectionLoc = glGetUniformLocation(lampShader.Program, "projection");
        glUniformMatrix4fv(lampViewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(lampProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        drawFloodlightPanels(lampModelLoc, VAO);
        glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    //DELETE BLOCK. THIS FREE MEMORY 
    deleteBallMesh(whiteMesh);
    deleteBallMesh(blackMesh);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    GLuint textures[] = { texGrass, texWhite, texBlack, texBlue, texRed,
                         texShorts, texSkin, texShoes, texPosts, texNet,
                         texSpecular, texBark, texLeavesDark,
                         texLeavesLight, texBird, texBeak, texGold, texFloodlight };
    glDeleteTextures(18, textures);
    busBodyModel.reset();
    for (int i = 0; i < 6; ++i) busWheelModels[i].reset();
    broadcastCameraBase.reset();
    broadcastCameraHead.reset();
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();



    return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);

    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);


    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);


    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);


    }

    if (keys[GLFW_KEY_T])
    {
        pointLightPositions[0].x += 0.01f;
    }
    if (keys[GLFW_KEY_G])
    {
        pointLightPositions[0].x -= 0.01f;
    }

    if (keys[GLFW_KEY_Y])
    {
        pointLightPositions[0].y += 0.01f;
    }

    if (keys[GLFW_KEY_H])
    {
        pointLightPositions[0].y -= 0.01f;
    }
    if (keys[GLFW_KEY_U])
    {
        pointLightPositions[0].z -= 0.1f;
    }
    if (keys[GLFW_KEY_J])
    {
        pointLightPositions[0].z += 0.01f;
    }

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }

    if (action != GLFW_PRESS)
        return;

    if (key >= GLFW_KEY_1 && key <= GLFW_KEY_9) {
        activeCamera = key - GLFW_KEY_1 + 1;
    }
    if (key == GLFW_KEY_0) {
        activeCamera = 0;
    }
    if (key == GLFW_KEY_SPACE) {
        active = !active;
        Light1 = active ? glm::vec3(0.2f, 0.8f, 1.0f) : glm::vec3(0.0f);
    }
    if (key == GLFW_KEY_N && (penaltyState == READY || penaltyState == FINISHED))
        penaltyRequested = true;
    if (key == GLFW_KEY_P) {
        fireworksActive = true;
        fireworksStartTime = static_cast<float>(glfwGetTime());
    }
    if (key == GLFW_KEY_B && busAssetsReady) {
        if (busState == BUS_WAITING || busState == BUS_COMPLETED) {
            busDistanceTraveled = 0.0f;
            busPosition = busStartPosition;
            busHeading = 0.0f;
            busSteeringAngle = 0.0f;
            busState = BUS_STRAIGHT;
            busRunning = true;
        }
        else {
            busRunning = !busRunning;
        }
    }
    if (key == GLFW_KEY_R) {
        ResetPenalty();
        goals = 0;
        saves = 0;
        misses = 0;
    }
}

float RandomFloat(float minValue, float maxValue)
{
    return minValue + (maxValue - minValue) *
        (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
}

void ResetPenalty()
{
    penaltyState = READY;
    stateTime = 0.0f;
    rotBall = 0.0f;
    kickAngle = 0.0f;
    keeperDive = 0.0f;
    keeperPositionZ = 0.0f;
    targetZ = 0.0f;
    targetY = 0.9f;
    shotSide = 0;
    keeperSide = 0;
    keeperCommitted = false;
    penaltyRequested = false;
    ballPosition = glm::vec3(41.5f, 0.11f, 0.0f);
    penaltyResult = "READY";
}

void Animation()
{
    if (penaltyRequested) {
        ResetPenalty();
        penaltyState = PREPARING;
        penaltyResult = "PREPARING";
        targetZ = RandomFloat(-4.0f, 4.0f);
        targetY = RandomFloat(0.35f, 2.15f);
        shotSide = (targetZ < -1.1f) ? -1 : ((targetZ > 1.1f) ? 1 : 0);
        penaltyRequested = false;
    }

    if (penaltyState == READY || penaltyState == FINISHED)
        return;

    stateTime += deltaTime;

    if (penaltyState == PREPARING) {
        float t = std::min(stateTime / 0.45f, 1.0f);
        kickAngle = -35.0f * t;
        if (t >= 1.0f) {
            penaltyState = KICKING;
            stateTime = 0.0f;
        }
    }
    else if (penaltyState == KICKING) {
        float t = std::min(stateTime / 0.24f, 1.0f);
        kickAngle = -35.0f + 100.0f * t;
        if (t >= 1.0f) {
            penaltyState = FLYING;
            penaltyResult = "BALL IN FLIGHT";
            stateTime = 0.0f;
        }
    }
    else if (penaltyState == FLYING) {
        float t = std::min(stateTime / 1.1f, 1.0f);
        float travel = std::min(t * 11.9f, 11.9f);
        ballPosition.x = 41.5f + travel;
        ballPosition.z = targetZ * t;
        ballPosition.y = 0.11f + (targetY - 0.11f) * t +
            0.60f * sinf(3.1415926535f * t);
        rotBall += 600.0f * deltaTime;
        kickAngle = 65.0f * (1.0f - t);

        if (!keeperCommitted && stateTime >= 0.15f) {
            keeperSide = ((rand() % 100) < 45) ? shotSide : (rand() % 3 - 1);
            keeperCommitted = true;
        }
        if (keeperCommitted) {
            float reaction = std::max(0.0f, std::min((stateTime - 0.15f) / 0.62f, 1.0f));
            keeperPositionZ = keeperSide * 2.0f * reaction;
            keeperDive = keeperSide * 55.0f * reaction;
        }

        if (ballPosition.x >= 51.2f && t < 1.0f) {
            bool withinReach = std::fabs(ballPosition.z - keeperPositionZ) <= 1.25f &&
                ballPosition.y <= 2.20f;
            if (withinReach) {
                penaltyState = FINISHED;
                penaltyResult = "SAVED";
                ++saves;
                std::cout << "Keeper saved the penalty! Shot side: " << shotSide
                    << ", goalkeeper side: " << keeperSide << std::endl;
                return;
            }
        }

        if (t >= 1.0f) {
            penaltyState = FINISHED;
            if (std::fabs(targetZ) <= 3.55f && targetY <= 2.33f) {
                penaltyResult = "GOAL";
                ++goals;
            }
            else {
                penaltyResult = "MISS";
                ++misses;
            }
            std::cout << penaltyResult << "! Shot side: " << shotSide
                << ", goalkeeper side: " << keeperSide << std::endl;
        }
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}









GLuint CreateSolidColorTexture(GLubyte r, GLubyte g, GLubyte b)
{
    GLuint texture;
    GLubyte pixel[] = { r, g, b };
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

GLuint LoadDiffuseTexture(const char* path)
{
    int width, height;
    unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
    if (!image) {
        std::cerr << "Texture not loaded: " << path << " (" << SOIL_last_result() << ")" << std::endl;
        return 0;
    }
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

void drawCube(GLint modelLoc, GLuint VAO, const glm::mat4& parent,
    glm::vec3 position, glm::vec3 size, GLuint texture, float yaw)
{
    glm::mat4 model = glm::translate(parent, position);
    model = glm::rotate(model, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, size);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void drawField(GLint modelLoc, GLint textureModeLoc, GLuint VAO, const glm::mat4& scene,
    GLuint texGrass, GLuint texWhite)
{
    // Ground: the upper surface is at y=0.
    glUniform1i(textureModeLoc, 1);
    drawCube(modelLoc, VAO, scene, glm::vec3(0, -0.04f, 0),
        glm::vec3(105, 0.08f, 68), texGrass);
    glUniform1i(textureModeLoc, 0);

    // Touchlines, goal lines, and halfway line.
    for (int side = -1; side <= 1; side += 2) {
        drawCube(modelLoc, VAO, scene, glm::vec3(0, 0.012f, side * 34.0f),
            glm::vec3(105, 0.024f, 0.12f), texWhite);
        drawCube(modelLoc, VAO, scene, glm::vec3(side * 52.5f, 0.012f, 0),
            glm::vec3(0.12f, 0.024f, 68), texWhite);
    }
    drawCube(modelLoc, VAO, scene, glm::vec3(0, 0.012f, 0),
        glm::vec3(0.12f, 0.024f, 68), texWhite);

    // Center circle made of rotated segments of the original cube.
    const float PI = 3.14159265358979323846f;
    const int segments = 48;
    const float radius = 9.15f;
    for (int i = 0; i < segments; ++i) {
        float a = 2.0f * PI * i / segments;
        float b = 2.0f * PI * (i + 1) / segments;
        float m = (a + b) * 0.5f;
        drawCube(modelLoc, VAO, scene,
            glm::vec3(radius * cosf(m), 0.018f, radius * sinf(m)),
            glm::vec3(2.0f * radius * sinf((b - a) / 2.0f) + 0.02f,
                0.012f, 0.12f),
            texWhite, glm::degrees(-m - PI * 0.5f));
    }
    drawCube(modelLoc, VAO, scene, glm::vec3(0, 0.02f, 0),
        glm::vec3(0.3f, 0.03f, 0.3f), texWhite);

    // Penalty areas (16.5 x 40.32) and goal areas (5.5 x 18.32).
    for (int side = -1; side <= 1; side += 2) {
        const float penaltyX = side * 36.0f;
        const float goalAreaX = side * 47.0f;
        drawCube(modelLoc, VAO, scene, glm::vec3(penaltyX, 0.018f, 0),
            glm::vec3(0.12f, 0.02f, 40.32f), texWhite);
        drawCube(modelLoc, VAO, scene, glm::vec3(side * 44.25f, 0.018f, -20.16f),
            glm::vec3(16.5f, 0.02f, 0.12f), texWhite);
        drawCube(modelLoc, VAO, scene, glm::vec3(side * 44.25f, 0.018f, 20.16f),
            glm::vec3(16.5f, 0.02f, 0.12f), texWhite);
        drawCube(modelLoc, VAO, scene, glm::vec3(goalAreaX, 0.018f, 0),
            glm::vec3(0.12f, 0.02f, 18.32f), texWhite);
        drawCube(modelLoc, VAO, scene, glm::vec3(side * 49.75f, 0.018f, -9.16f),
            glm::vec3(5.5f, 0.02f, 0.12f), texWhite);
        drawCube(modelLoc, VAO, scene, glm::vec3(side * 49.75f, 0.018f, 9.16f),
            glm::vec3(5.5f, 0.02f, 0.12f), texWhite);
        drawCube(modelLoc, VAO, scene, glm::vec3(side * 41.5f, 0.018f, 0),
            glm::vec3(0.3f, 0.03f, 0.3f), texWhite);
    }
}

void drawBleachers(GLint modelLoc, GLuint VAO, const glm::mat4& scene,
    GLuint texConcrete, GLuint texSeats)
{
    // Cancha: |z| <= 34; gradas: 36.5..45.5; torres de luz: |z| = 49.
    constexpr float front = 36.5f, depth = 1.8f, rise = 0.72f;
    static_assert(front > 34.0f && front + 5 * depth < 49.0f,
        "Las gradas deben quedar entre la cancha y las torres de luz");
    for (int side = -1; side <= 1; side += 2) {
        for (int section = -1; section <= 1; section += 2) {
            // Dos secciones con acceso central libre de cuatro metros.
            float x = section * 21.0f;
            for (int row = 0; row < 5; ++row) {
                float z = side * (front + (row + 0.5f) * depth);
                float height = (row + 1) * rise;
                drawCube(modelLoc, VAO, scene, glm::vec3(x, height * 0.5f, z),
                    glm::vec3(38.0f, height, depth), texConcrete);
                drawCube(modelLoc, VAO, scene, glm::vec3(x, height + 0.06f, z),
                    glm::vec3(37.6f, 0.12f, 1.45f), texSeats);
            }
            // Fachada exterior: el muro deja libre el acceso al pasillo central.
            drawCube(modelLoc, VAO, scene, glm::vec3(x, 2.0f, side * 46.0f),
                glm::vec3(38.0f, 4.0f, 0.5f), texConcrete);
            drawCube(modelLoc, VAO, scene, glm::vec3(x, 3.65f, side * 46.30f),
                glm::vec3(38.0f, 0.22f, 0.10f), texSeats);
        }
        for (int column = -4; column <= 4; ++column) {
            if (column == 0) continue;
            drawCube(modelLoc, VAO, scene, glm::vec3(column * 10.0f, 2.0f, side * 46.4f),
                glm::vec3(0.45f, 4.0f, 0.55f), texConcrete);
        }
        // Acceso a nivel de piso: marco abierto y camino directo entre las gradas.
        for (int jamb = -1; jamb <= 1; jamb += 2)
            drawCube(modelLoc, VAO, scene, glm::vec3(jamb * 2.25f, 1.8f, side * 46.3f),
                glm::vec3(0.5f, 3.6f, 0.7f), texConcrete);
        drawCube(modelLoc, VAO, scene, glm::vec3(0.0f, 3.95f, side * 46.3f),
            glm::vec3(5.0f, 0.7f, 0.7f), texConcrete);
        drawCube(modelLoc, VAO, scene, glm::vec3(0.0f, 4.31f, side * 46.65f),
            glm::vec3(5.0f, 0.15f, 0.10f), texSeats);
    }
}

void drawPerimeter(GLint modelLoc, GLuint VAO, const glm::mat4& scene,
    GLuint texWall, GLuint texFloor, GLuint texAccent)
{
    // Incluye los árboles (|z| <= 57.2) y el recorrido del autobús (x <= 85).
    constexpr float halfLength = 89.0f, halfWidth = 64.0f;
    static_assert(halfLength > 85.0f && halfWidth > 57.2f,
        "El perimetro debe contener el autobus y los arboles");
    drawCube(modelLoc, VAO, scene, glm::vec3(0.0f, -0.20f, 0.0f),
        glm::vec3(2.0f * halfLength, 0.20f, 2.0f * halfWidth), texFloor);

    for (int side = -1; side <= 1; side += 2) {
        // Muros de las cabeceras: unen los dos laterales sin cerrar la vista superior.
        drawCube(modelLoc, VAO, scene, glm::vec3(side * halfLength, 2.0f, 0.0f),
            glm::vec3(0.5f, 4.0f, 2.0f * halfWidth), texWall);
        drawCube(modelLoc, VAO, scene, glm::vec3(side * (halfLength + 0.30f), 3.65f, 0.0f),
            glm::vec3(0.10f, 0.22f, 2.0f * halfWidth), texAccent);

        // Laterales con dos vanos alineados con los pasillos entre gradas.
        for (int section = -1; section <= 1; section += 2) {
            float x = section * (halfLength + 2.0f) * 0.5f;
            drawCube(modelLoc, VAO, scene, glm::vec3(x, 2.0f, side * halfWidth),
                glm::vec3(halfLength - 2.0f, 4.0f, 0.5f), texWall);
            drawCube(modelLoc, VAO, scene, glm::vec3(x, 3.65f, side * (halfWidth + 0.30f)),
                glm::vec3(halfLength - 2.0f, 0.22f, 0.10f), texAccent);
        }
        drawCube(modelLoc, VAO, scene, glm::vec3(0.0f, -0.01f, side * 55.0f),
            glm::vec3(4.0f, 0.08f, 22.0f), texWall);
        for (int jamb = -1; jamb <= 1; jamb += 2)
            drawCube(modelLoc, VAO, scene, glm::vec3(jamb * 2.25f, 1.8f, side * halfWidth),
                glm::vec3(0.5f, 3.6f, 0.7f), texWall);
        drawCube(modelLoc, VAO, scene, glm::vec3(0.0f, 3.95f, side * halfWidth),
            glm::vec3(5.0f, 0.7f, 0.7f), texWall);
    }
}

void drawGoal(GLint modelLoc, GLuint VAO, const glm::mat4& scene,
    float x, float direction, GLuint texPosts, GLuint texNet)
{
    glm::mat4 parent = glm::translate(scene, glm::vec3(x, 0, 0));
    // Goal posts and crossbar (soccer dimensions: 7.32 x 2.44 m).
    drawCube(modelLoc, VAO, parent, glm::vec3(0, 1.22f, -3.66f),
        glm::vec3(0.13f, 2.44f, 0.13f), texPosts);
    drawCube(modelLoc, VAO, parent, glm::vec3(0, 1.22f, 3.66f),
        glm::vec3(0.13f, 2.44f, 0.13f), texPosts);
    drawCube(modelLoc, VAO, parent, glm::vec3(0, 2.44f, 0),
        glm::vec3(0.13f, 0.13f, 7.32f), texPosts);
    // Rear frame and simplified net made of thin bars.
    float depth = direction * 1.75f;
    for (int i = 0; i <= 14; ++i) {
        float z = -3.66f + (7.32f / 14.0f) * i;
        drawCube(modelLoc, VAO, parent, glm::vec3(depth, 1.15f, z),
            glm::vec3(0.014f, 2.3f, 0.014f), texNet);
    }
    for (int i = 0; i <= 6; ++i) {
        float y = 2.3f * i / 6.0f;
        drawCube(modelLoc, VAO, parent, glm::vec3(depth, y, 0),
            glm::vec3(0.014f, 0.014f, 7.32f), texNet);
    }
    for (int i = 0; i <= 14; ++i) {
        float z = -3.66f + (7.32f / 14.0f) * i;
        drawCube(modelLoc, VAO, parent, glm::vec3(depth / 2.0f, 2.4f, z),
            glm::vec3(1.75f, 0.014f, 0.014f), texNet);
    }
}

void drawPlayer(GLint modelLoc, GLuint VAO, const glm::mat4& scene,
    glm::vec3 position, float yaw, GLuint texJersey,
    GLuint texShorts, GLuint texSkin, GLuint texShoes,
    float kickAngle, float diveAngle)
{
    // Hierarchical modeling: shoulder -> elbow and hip -> knee.
    glm::mat4 model = glm::translate(scene, position);
    model = glm::rotate(model, glm::radians(diveAngle), glm::vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(yaw), glm::vec3(0, 1, 0));
    glm::mat4 modelTemp = model; // Same principle used in the original example.

    drawCube(modelLoc, VAO, modelTemp, glm::vec3(0, 1.20f, 0),
        glm::vec3(0.45f, 0.64f, 0.25f), texJersey);
    drawCube(modelLoc, VAO, modelTemp, glm::vec3(0, 1.55f, 0),
        glm::vec3(0.12f, 0.13f, 0.12f), texSkin);
    drawCube(modelLoc, VAO, modelTemp, glm::vec3(0, 1.77f, 0),
        glm::vec3(0.27f, 0.29f, 0.27f), texSkin);

    for (int side = -1; side <= 1; side += 2) {
        // Arm: both segments are positioned relative to their joint.
        glm::mat4 shoulder = glm::translate(modelTemp, glm::vec3(side * 0.3f, 1.43f, 0));
        shoulder = glm::rotate(shoulder, glm::radians(-20.0f), glm::vec3(1, 0, 0));
        shoulder = glm::rotate(shoulder, glm::radians(side * std::fabs(diveAngle) * 1.1f), glm::vec3(0, 0, 1));
        drawCube(modelLoc, VAO, shoulder, glm::vec3(0, -0.15f, 0),
            glm::vec3(0.14f, 0.3f, 0.14f), texJersey);
        glm::mat4 elbow = glm::translate(shoulder, glm::vec3(0, -0.3f, 0));
        drawCube(modelLoc, VAO, elbow, glm::vec3(0, -0.16f, 0),
            glm::vec3(0.11f, 0.32f, 0.11f), texSkin);
        // Leg: the knee is a child of the hip joint.
        glm::mat4 hip = glm::translate(modelTemp, glm::vec3(side * 0.13f, 0.90f, 0));
        if (side == 1) hip = glm::rotate(hip, glm::radians(-kickAngle), glm::vec3(1, 0, 0));
        drawCube(modelLoc, VAO, hip, glm::vec3(0, -0.19f, 0),
            glm::vec3(0.19f, 0.38f, 0.19f), texShorts);
        glm::mat4 knee = glm::translate(hip, glm::vec3(0, -0.38f, 0));
        drawCube(modelLoc, VAO, knee, glm::vec3(0, -0.19f, 0),
            glm::vec3(0.15f, 0.38f, 0.15f), texSkin);
        drawCube(modelLoc, VAO, knee, glm::vec3(0, -0.42f, 0.09f),
            glm::vec3(0.17f, 0.11f, 0.29f), texShoes);
    }
}

static glm::vec3 SpherePoint(float theta, float phi)
{
    return glm::vec3(sinf(theta) * cosf(phi), cosf(theta),
        sinf(theta) * sinf(phi));
}

static void AddVertex(std::vector<GLfloat>& v, glm::vec3 p)
{
    // Position (XYZ), normal (XYZ). The original vertex format is unchanged.
    v.push_back(p.x); v.push_back(p.y); v.push_back(p.z);
    v.push_back(p.x); v.push_back(p.y); v.push_back(p.z);
}

static bool IsBlackPatch(glm::vec3 normal)
{
    // Approximation of the black pentagons on a soccer ball.
    const float d = 1.61803398875f;
    const glm::vec3 centers[] = {
        {0, 1, d}, {0, -1, d}, {0, 1, -d}, {0, -1, -d},
        {1, d, 0}, {-1, d, 0}, {1, -d, 0}, {-1, -d, 0},
        {d, 0, 1}, {-d, 0, 1}, {d, 0, -1}, {-d, 0, -1}
    };
    for (int i = 0; i < 12; ++i)
        if (glm::dot(normal, glm::normalize(centers[i])) > 0.925f)
            return true;
    return false;
}

static void AddTriangle(std::vector<GLfloat>& whiteVertices,
    std::vector<GLfloat>& blackVertices,
    glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
    glm::vec3 direction = glm::normalize(a + b + c);
    std::vector<GLfloat>& target = IsBlackPatch(direction) ? blackVertices : whiteVertices;
    AddVertex(target, a);
    AddVertex(target, b);
    AddVertex(target, c);
}

static void CreateBallMesh(BallMesh& mesh, const std::vector<GLfloat>& vertexData)
{
    mesh.vertexCount = static_cast<GLsizei>(vertexData.size() / 6);
    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glBindVertexArray(mesh.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
        (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void createBall(BallMesh& whiteMesh, BallMesh& blackMesh)
{
    const float PI = 3.14159265358979323846f;
    const int latitudeSegments = 24, longitudeSegments = 48;
    std::vector<GLfloat> whiteVertices, blackVertices;
    for (int i = 0; i < latitudeSegments; ++i) {
        float t0 = PI * i / latitudeSegments;
        float t1 = PI * (i + 1) / latitudeSegments;
        for (int j = 0; j < longitudeSegments; ++j) {
            float p0 = 2.0f * PI * j / longitudeSegments;
            float p1 = 2.0f * PI * (j + 1) / longitudeSegments;
            glm::vec3 a = SpherePoint(t0, p0);
            glm::vec3 b = SpherePoint(t0, p1);
            glm::vec3 c = SpherePoint(t1, p0);
            glm::vec3 d = SpherePoint(t1, p1);
            AddTriangle(whiteVertices, blackVertices, a, d, c);
            AddTriangle(whiteVertices, blackVertices, a, b, d);
        }
    }
    CreateBallMesh(whiteMesh, whiteVertices);
    CreateBallMesh(blackMesh, blackVertices);
}

void drawBall(GLint modelLoc, const glm::mat4& scene,
    const BallMesh& whiteMesh, const BallMesh& blackMesh,
    GLuint texWhite, GLuint texBlack, float rotation, glm::vec3 position)
{
    glm::mat4 model = glm::translate(scene, position);
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 0, 1));
    model = glm::scale(model, glm::vec3(0.11f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texWhite);
    glBindVertexArray(whiteMesh.VAO);
    glDrawArrays(GL_TRIANGLES, 0, whiteMesh.vertexCount);
    glBindTexture(GL_TEXTURE_2D, texBlack);
    glBindVertexArray(blackMesh.VAO);
    glDrawArrays(GL_TRIANGLES, 0, blackMesh.vertexCount);
    glBindVertexArray(0);
}

void deleteBallMesh(BallMesh& mesh)
{
    glDeleteVertexArrays(1, &mesh.VAO);
    glDeleteBuffers(1, &mesh.VBO);
}

void drawSolidSphere(GLint modelLoc, const glm::mat4& parent, glm::vec3 position,
    glm::vec3 size, const BallMesh& whiteMesh, const BallMesh& blackMesh, GLuint texture)
{
    glm::mat4 model = glm::translate(parent, position);
    model = glm::scale(model, size);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(whiteMesh.VAO);
    glDrawArrays(GL_TRIANGLES, 0, whiteMesh.vertexCount);
    glBindVertexArray(blackMesh.VAO);
    glDrawArrays(GL_TRIANGLES, 0, blackMesh.vertexCount);
    glBindVertexArray(0);
}

void drawTrees(GLint modelLoc, GLint textureModeLoc, GLuint VAO, const glm::mat4& scene,
    const BallMesh& whiteMesh, const BallMesh& blackMesh,
    GLuint texBark, GLuint texLeavesDark, GLuint texLeavesLight)
{
    for (int side = -1; side <= 1; side += 2) {
        for (int i = -3; i <= 3; ++i) {
            if (i == 0) continue;
            float x = static_cast<float>(i) * 15.5f;
            float z = side * (55.0f + ((i + 3) % 2) * 2.2f);
            float height = 3.5f + static_cast<float>((i + 3) % 3) * 0.45f;
            glUniform1i(textureModeLoc, 2);
            drawCube(modelLoc, VAO, scene, glm::vec3(x, height * 0.5f, z),
                glm::vec3(0.5f, height, 0.5f), texBark);
            glUniform1i(textureModeLoc, 0);
            glUniform1i(textureModeLoc, 3);
            drawSolidSphere(modelLoc, scene, glm::vec3(x, height + 0.35f, z),
                glm::vec3(2.0f, 1.65f, 1.8f), whiteMesh, blackMesh, texLeavesDark);
            drawSolidSphere(modelLoc, scene, glm::vec3(x + 0.5f, height + 1.2f, z - 0.45f),
                glm::vec3(1.35f, 1.2f, 1.25f), whiteMesh, blackMesh, texLeavesLight);
            glUniform1i(textureModeLoc, 0);
        }
    }
}

void drawBird(GLint modelLoc, GLuint VAO, const glm::mat4& scene,
    const BallMesh& whiteMesh, const BallMesh& blackMesh,
    GLuint texBird, GLuint texBeak, float elapsed)
{
    float orbit = elapsed * 0.24f;
    float x = 58.0f * cosf(orbit);
    float z = 41.0f * sinf(orbit);
    float y = 11.5f + 1.8f * sinf(elapsed * 0.46f);

    glm::mat4 bird = glm::translate(scene, glm::vec3(x, y, z));
    glm::vec3 direction(-58.0f * sinf(orbit), 0.0f, 41.0f * cosf(orbit));
    float yaw = atan2f(-direction.z, direction.x);
    bird = glm::rotate(bird, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
    bird = glm::rotate(bird, glm::radians(4.0f * sinf(elapsed * 0.8f)),
        glm::vec3(0.0f, 0.0f, 1.0f));

    drawSolidSphere(modelLoc, bird, glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.55f, 0.20f, 0.24f), whiteMesh, blackMesh, texBird);
    drawSolidSphere(modelLoc, bird, glm::vec3(0.48f, 0.12f, 0.0f),
        glm::vec3(0.23f, 0.23f, 0.20f), whiteMesh, blackMesh, texBird);
    drawCube(modelLoc, VAO, bird, glm::vec3(0.72f, 0.07f, 0.0f),
        glm::vec3(0.24f, 0.09f, 0.14f), texBeak);
    drawCube(modelLoc, VAO, bird, glm::vec3(-0.64f, 0.05f, 0.0f),
        glm::vec3(0.48f, 0.06f, 0.36f), texBird);

    float flap = glm::radians(35.0f * sinf(elapsed * 8.0f));
    for (int side = -1; side <= 1; side += 2) {
        glm::mat4 wing = glm::translate(bird, glm::vec3(-0.05f, 0.07f, side * 0.16f));
        wing = glm::rotate(wing, side * flap, glm::vec3(1.0f, 0.0f, 0.0f));
        drawCube(modelLoc, VAO, wing, glm::vec3(-0.03f, 0.0f, side * 0.55f),
            glm::vec3(0.55f, 0.075f, 1.0f), texBird);
        glm::mat4 outerWing = glm::translate(wing, glm::vec3(0.0f, 0.0f, side * 1.0f));
        outerWing = glm::rotate(outerWing, side * flap * 0.55f,
            glm::vec3(1.0f, 0.0f, 0.0f));
        drawCube(modelLoc, VAO, outerWing, glm::vec3(-0.12f, 0.0f, side * 0.32f),
            glm::vec3(0.40f, 0.05f, 0.66f), texBird);
    }
}

void drawFireworks(GLint modelLoc, GLuint VAO, const glm::mat4& scene,
    float elapsed, GLuint texGold, GLuint texBlue, GLuint texRed, GLuint texWhite)
{
    const float pi = 3.14159265358979323846f;
    for (int burst = 0; burst < 3; ++burst) {
        float local = elapsed - 0.48f * burst;
        if (local < 0.0f || local > 3.2f)
            continue;

        float x = 60.0f + (burst % 2 == 0 ? 1.0f : -3.0f);
        float z = -21.0f + burst * 20.0f;
        float explodeAt = 0.60f;
        float baseHeight = 10.0f + burst * 1.7f;

        if (local < explodeAt) {
            float y = baseHeight * local / explodeAt;
            drawCube(modelLoc, VAO, scene, glm::vec3(x, y, z),
                glm::vec3(0.13f, 0.45f, 0.13f), texGold);
            continue;
        }

        float t = local - explodeAt;
        float radius = 7.0f * t;
        for (int i = 0; i < 32; ++i) {
            float theta = pi * (static_cast<float>((i * 11) % 31) + 0.5f) / 31.0f;
            float phi = 2.0f * pi * (static_cast<float>(i) / 1.61803398875f);
            glm::vec3 unit(sinf(theta) * cosf(phi), cosf(theta), sinf(theta) * sinf(phi));
            glm::vec3 p(x + unit.x * radius,
                baseHeight + unit.y * radius - 1.35f * t * t,
                z + unit.z * radius);
            GLuint color = i % 4 == 0 ? texGold :
                (i % 4 == 1 ? texBlue : (i % 4 == 2 ? texRed : texWhite));
            float size = std::max(0.05f, 0.23f * (1.0f - t / 3.0f));
            drawCube(modelLoc, VAO, scene, p,
                glm::vec3(size, size * 1.5f, size), color);
        }
    }
}

void updateBusAnimation()
{
    if (!busAssetsReady || !busRunning)
        return;
    float frameStep = std::max(0.0f, std::min(deltaTime, 0.1f));
    busDistanceTraveled = std::min(busRouteLength, busDistanceTraveled + busSpeed * frameStep);
    float travel = busDistanceTraveled;
    if (travel <= busInitialDistance) {
        busState = BUS_STRAIGHT;
        busPosition = busStartPosition + glm::vec3(0.0f, 0.0f, travel);
        busHeading = 0.0f;
        busSteeringAngle = 0.0f;
    }
    else if (travel <= busInitialDistance + busTurnLength) {
        busState = BUS_TURNING;
        float turnAngle = (travel - busInitialDistance) / busTurnRadius;
        busPosition = busStartPosition + glm::vec3(
            busTurnRadius * (1.0f - cosf(turnAngle)),
            0.0f,
            busInitialDistance + busTurnRadius * sinf(turnAngle));
        busHeading = turnAngle;
        busSteeringAngle = glm::radians(18.0f) * sinf(2.0f * turnAngle);
    }
    else {
        busState = BUS_LEAVING;
        float finalTravel = travel - busInitialDistance - busTurnLength;
        busPosition = busStartPosition + glm::vec3(
            busTurnRadius + finalTravel,
            0.0f,
            busInitialDistance + busTurnRadius);
        busHeading = busPi * 0.5f;
        busSteeringAngle = 0.0f;
    }
    if (busDistanceTraveled >= busRouteLength) {
        busState = BUS_COMPLETED;
        busRunning = false;
    }
}

bool loadBusWheelData(const char* filePath)
{
    std::ifstream fileStream(filePath);
    if (!fileStream) {
        std::cerr << "Missing bus wheel data: " << filePath << std::endl;
        return false;
    }
    std::string line;
    int i = 0;
    while (std::getline(fileStream, line) && i < 6) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream lineStream(line);
        float x, y, z, radius;
        int frontWheelFlag;
        if (!(lineStream >> x >> y >> z >> radius >> frontWheelFlag) || radius <= 0.0f) {
            std::cerr << "Invalid bus wheel data: " << filePath << std::endl;
            return false;
        }
        busWheelData[i++] = { glm::vec3(x, y, z), radius, frontWheelFlag != 0 };
    }
    if (i != 6) {
        std::cerr << "Expected 6 wheels in: " << filePath << std::endl;
        return false;
    }
    return true;
}


glm::vec3 getBroadcastPosition()
{
    if (activeCamera == 9) return facadeCameraPosition;
    return activeCamera >= 1 && activeCamera <= 8
        ? broadcastCameraPositions[activeCamera - 1] : camera.GetPosition();
}

glm::mat4 getBroadcastView()
{
    if (activeCamera == 9)
        return glm::lookAt(facadeCameraPosition, glm::vec3(0.0f, 2.0f, -43.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));
    if (activeCamera >= 1 && activeCamera <= 8) {
        glm::vec3 eye = broadcastCameraPositions[activeCamera - 1];
        return glm::lookAt(eye, trackedBallPosition, glm::vec3(0.0f, 1.0f, 0.0f));
    }
    return camera.GetViewMatrix();
}

void drawFloodlightTowers(GLint modelLoc, GLuint VAO, const glm::mat4& scene, GLuint texMetal)
{
    for (int i = 0; i < 4; ++i) {
        const glm::vec3& p = floodlightPositions[i];
        drawCube(modelLoc, VAO, scene, p + glm::vec3(0.0f, 8.25f, 0.0f),
            glm::vec3(0.50f, 16.50f, 0.50f), texMetal);
        drawCube(modelLoc, VAO, scene, p + glm::vec3(0.0f, 17.45f, 0.0f),
            glm::vec3(4.70f, 1.50f, 0.45f), texMetal);
        for (int beam = 1; beam < 5; ++beam) {
            float y = beam * 3.2f;
            drawCube(modelLoc, VAO, scene, p + glm::vec3(0.0f, y, 0.0f),
                glm::vec3(1.0f, 0.12f, 0.9f), texMetal);
        }
    }
}

void drawFloodlightPanels(GLint modelLoc, GLuint VAO)
{
    glBindVertexArray(VAO);
    for (int i = 0; i < 4; ++i) {
        const glm::vec3& p = floodlightPositions[i];
        for (int row = 0; row < 2; ++row) {
            for (int col = 0; col < 6; ++col) {
                glm::vec3 lampPosition = p + glm::vec3((col - 2.5f) * 0.70f,
                    17.13f + row * 0.62f, p.z < 0 ? 0.27f : -0.27f);
                glm::mat4 m = glm::translate(glm::mat4(1.0f), lampPosition);
                m = glm::scale(m, glm::vec3(0.56f, 0.44f, 0.12f));
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m));
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
    }
    glBindVertexArray(0);
}

void drawBroadcastCameraModels(GLint modelLoc, Shader& lightingShader, Model* cameraBase, Model* cameraHead)
{
    if (!cameraBase || !cameraHead) return;
    glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
    for (int i = 0; i < 8; ++i) {
        const glm::vec3& position = broadcastCameraPositions[i];
        glm::mat4 baseMatrix = glm::translate(glm::mat4(1.0f), position);
        baseMatrix = glm::scale(baseMatrix, glm::vec3(cameraModelScale));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(baseMatrix));
        cameraBase->Draw(lightingShader);

        glm::vec3 pivotWorld = position + cameraModelScale * cameraHeadPivot;
        glm::vec3 direction = trackedBallPosition - pivotWorld;
        float horizontalDistance = glm::length(glm::vec2(direction.x, direction.z));
        float yaw = std::atan2(-direction.x, -direction.z) +
            glm::radians(cameraModelForwardYawDegrees);
        float pitch = std::atan2(direction.y, std::max(0.0001f, horizontalDistance));
        pitch = glm::clamp(pitch, glm::radians(-80.0f), glm::radians(80.0f));

        glm::mat4 headMatrix = glm::translate(baseMatrix, cameraHeadPivot);
        headMatrix = glm::rotate(headMatrix, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
        headMatrix = glm::rotate(headMatrix, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(headMatrix));
        cameraHead->Draw(lightingShader);
    }
}
