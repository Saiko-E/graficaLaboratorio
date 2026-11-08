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
void beginMatchScene(int scene, bool automatic);
void requestMatchScene(int scene);
void resetMatch();
void updateMatch(float step);
void updateFade(float step);
void drawMatchScoreboard(GLint modelLoc, GLuint VAO, const glm::mat4& scene,
    GLuint dark, GLuint white, GLuint gold, GLuint red);
void drawScoreDigit(GLint modelLoc, GLuint VAO, const glm::mat4& matrix,
    int digit, float x, float y, float z, float width, GLuint texture);
GLuint createFadeProgram();
void drawFadeOverlay(GLuint program, GLuint vao);
bool loadBusWheelData(const char* filePath);


//================= draw function =====================================
GLuint CreateSolidColorTexture(GLubyte r, GLubyte g, GLubyte b);
GLuint LoadDiffuseTexture(const char* path);
void VerifyModelTextures(const std::string& modelPath);
void drawCube(GLint modelLoc, GLuint VAO, const glm::mat4& parent, glm::vec3 position, glm::vec3 size, GLuint texture, float yaw = 0.0f);
void drawField(GLint modelLoc, GLint textureModeLoc, GLuint VAO, const glm::mat4& scene, GLuint texGrass, GLuint texWhite);
void drawPerimeter(GLint modelLoc, GLint textureModeLoc, GLuint VAO,
    const glm::mat4& scene, GLuint texWall, GLuint texFacade, GLuint texFloor, GLuint texAccent);
void drawGoal(GLint modelLoc, GLuint VAO, const glm::mat4& scene, float x, float direction, GLuint texPosts, GLuint texNet);
void drawPlayer(GLint modelLoc, GLuint VAO, GLuint torsoVAO, GLuint headVAO, const glm::mat4& scene,
    glm::vec3 position, float yaw, GLuint texJersey, GLuint texHead,
    GLuint texShorts, GLuint texSkin, GLuint texShoes,
    float kickAngle = 0.0f, float diveAngle = 0.0f, float walkAngle = 0.0f,
    int jerseyNumber = 0, GLuint numberColor = 0);

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
void drawFloodlightTowers(GLint modelLoc, GLint textureModeLoc, GLuint VAO,
    const glm::mat4& scene, GLuint texMetal);
void drawFloodlightPanels(GLint modelLoc, GLuint VAO);
void drawBroadcastCameraModels(GLint modelLoc, Shader& lightingShader, Model* cameraBase, Model* cameraHead);
void drawBleachers(GLint modelLoc, GLint textureModeLoc, GLuint VAO, const glm::mat4& scene,
    GLuint texConcrete, GLuint texSideWall, GLuint texRed, GLuint texWhite, GLuint texWalkway, GLuint texMetal);
void drawDrone(GLint modelLoc, GLuint VAO, const glm::mat4& scene,
    const BallMesh& whiteMesh, const BallMesh& blackMesh,
    glm::vec3 position, glm::vec3 target, float elapsed,
    GLuint texBody, GLuint texBlades, GLuint texMetal, GLuint texLens);
void drawTrophy(GLint modelLoc, GLuint VAO, const glm::mat4& scene,
    const BallMesh& whiteMesh, const BallMesh& blackMesh,
    float elapsed, GLuint texGold, GLuint texBase, GLuint texWhite);
glm::mat4 getBroadcastView();
glm::vec3 getBroadcastPosition();
void ResetPenalty();
float RandomFloat(float minValue, float maxValue);


// Window dimensions
const GLuint WIDTH = 1280, HEIGHT = 720;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(32.0f, 2.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
glm::vec3 freeCameraPosition(32.0f, 2.0f, 0.0f);
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
int activeCamera = 9;
glm::vec3 broadcastCameraPositions[8] = {
    glm::vec3(20.0f, 22.0f, -14.0f),
    glm::vec3(20.0f, 22.0f, 14.0f),
    glm::vec3(30.0f, 0.0f, -40.0f),
    glm::vec3(59.0f, 0.0f, 0.0f),
    glm::vec3(30.0f, 0.0f, 40.0f),
    glm::vec3(49.0f, 0.0f, -40.0f),
    glm::vec3(24.0f, 4.32f, 62.0f),
    glm::vec3(-24.0f, 4.32f, -62.0f)
};
const glm::vec3 facadeCameraPosition(24.0f, 10.0f, -95.0f);
glm::vec3 trackedBallPosition(0.0f, 0.11f, 0.0f);
glm::vec3 cameraHeadPivot(0.0f, 8.837823f, 0.0f);
const float cameraModelScale = 0.20f;
float cameraModelForwardYawDegrees = 0.0f;
const std::string cameraAssetDirectory = "Models/Camaras/";
const glm::vec3 floodlightPositions[4] = {
    glm::vec3(-40.0f, 0.0f, -49.0f),
    glm::vec3(-40.0f, 0.0f, 49.0f),
    glm::vec3(40.0f, 0.0f, -49.0f),
    glm::vec3(40.0f, 0.0f, 49.0f)
};

enum BusState { BUS_WAITING, BUS_STRAIGHT, BUS_TURNING, BUS_COMPLETED };
struct WheelData {
    glm::vec3 position;
    float radius;
    bool isFrontWheel;
};
WheelData busWheelData[6];
const float busPi = 3.14159265358979323846f;
const float busSpeed = 3.0f;
const float busInitialDistance = 28.0f;
const float busTurnRadius = 8.0f;
const float busTurnLength = busTurnRadius * busPi * 0.5f;
const float busRouteLength = busInitialDistance + busTurnLength;
const float busModelScale = 1.85f;
struct TeamBus {
    glm::vec3 position;
    float heading;
    float distanceTraveled;
    float steeringAngle;
    bool moving;
    BusState state;
};
TeamBus teamBuses[2];
bool busAssetsReady = false;
bool arrivalStarted = false;
bool busHeadlightsOn = false;
float teamLineupTime[2] = { 0.0f, 0.0f };
const float playerExitDelay = 0.62f;
const float playerWalkDuration = 3.7f;
const int playersPerTeam = 11;
const float pumasCornerKickTime = 0.55f;
const float pumasShotTime = 4.3f;

const glm::vec3 busPivot(0.36695f, 0.0f, 3.16461f);
glm::mat4 getBusModelMatrix(const TeamBus& bus)
{
    glm::mat4 matrix(1.0f);
    matrix = glm::translate(matrix, bus.position);
    matrix = glm::rotate(matrix, bus.heading, glm::vec3(0.0f, 1.0f, 0.0f));
    matrix = glm::scale(matrix, glm::vec3(busModelScale));
    return glm::translate(matrix, -busPivot);
}

glm::vec3 getBusHeadlightPosition(const TeamBus& bus, int index)
{
    glm::vec3 local(index == 0 ? -0.14f : 0.87f, 1.95f, 6.75f);
    return glm::vec3(getBusModelMatrix(bus) * glm::vec4(local, 1.0f));
}

glm::vec3 getPlayerExitPosition(int team)
{
    float sign = team == 0 ? 1.0f : -1.0f;
    return glm::vec3(sign * 64.8f, 0.0f, -sign * 10.2f);
}

glm::vec3 getPlayerLineupPosition(int team, int player)
{
    float sign = team == 0 ? 1.0f : -1.0f;
    return glm::vec3(sign * 59.0f, 0.0f, -26.0f + 5.2f * player);
}

enum MatchScene {
    SCENE_IDLE, SCENE_ARRIVAL, SCENE_KICKOFF,
    SCENE_PUMAS_GOAL, SCENE_ATLANTE_GOAL, SCENE_PENALTIES, SCENE_FINAL
};
enum FadePhase { FADE_NONE, FADE_OUT, FADE_IN };
MatchScene matchScene = SCENE_IDLE;
MatchScene nextScene = SCENE_IDLE;
FadePhase fadePhase = FADE_NONE;
bool autoSequence = false;
bool sequencePaused = false;
float sceneTime = 0.0f;
float fadeAlpha = 0.0f;
float stadiumLightIntensity = 1.0f;
extern float animationClock;
int pumasScore = 0;
int atlanteScore = 0;
int matchMinute = 0;
int matchSecond = 0;
int penaltyTaken[2] = { 0, 0 };
int penaltyGoals[2] = { 0, 0 };
int penaltyKickerTeam = 0;
int penaltyWinner = -1;
int penaltyNextKickerTeam = -1;
bool penaltyShotRecorded = false;
float penaltyRestTime = 0.0f;
const float transitionDuration = 1.15f;
const glm::vec3 trophyFocusPosition(0.0f, 2.65f, 38.0f);

const char* getSceneLabel()
{
    switch (matchScene) {
    case SCENE_ARRIVAL: return "LLEGADA";
    case SCENE_KICKOFF: return "SAQUE INICIAL";
    case SCENE_PUMAS_GOAL: return "CORNER PUMAS";
    case SCENE_ATLANTE_GOAL: return "TIRO LIBRE ATLANTE";
    case SCENE_PENALTIES: return "PENALES";
    case SCENE_FINAL: return "FINAL";
    default: return "INICIO";
    }
}

bool isLineupComplete()
{
    float endTime = (playersPerTeam - 1) * playerExitDelay + playerWalkDuration;
    return arrivalStarted && teamLineupTime[0] >= endTime && teamLineupTime[1] >= endTime;
}

void startArrival()
{
    if (!busAssetsReady) {
        std::cerr << "Bus models missing; cannot start arrival." << std::endl;
        return;
    }
    for (int team = 0; team < 2; ++team) {
        TeamBus& bus = teamBuses[team];
        float sign = team == 0 ? 1.0f : -1.0f;
        float y = bus.position.y;
        bus.position = glm::vec3(sign * 102.0f, y, -sign * 21.0f);
        bus.heading = -sign * busPi * 0.5f;
        bus.distanceTraveled = 0.0f;
        bus.steeringAngle = 0.0f;
        bus.state = BUS_STRAIGHT;
        bus.moving = true;
        teamLineupTime[team] = 0.0f;
    }
    arrivalStarted = true;
    activeCamera = 9;
    ResetPenalty();
    goals = 0;
    saves = 0;
    misses = 0;
    std::cout << "Team arrival started." << std::endl;
}

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame
float animationClock = 0.0f;

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
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* screenMode = monitor ? glfwGetVideoMode(monitor) : nullptr;
    GLFWwindow* window = glfwCreateWindow(screenMode ? screenMode->width : WIDTH,
        screenMode ? screenMode->height : HEIGHT,
        "Pumas vs Atlante - Estadio", monitor, nullptr);

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
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int width, int height) {
        SCREEN_WIDTH = std::max(1, width);
        SCREEN_HEIGHT = std::max(1, height);
        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        });

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
    Shader skyShader("Shader/sky.vs", "Shader/sky.frag");

    const char* wheelFiles[6] = {
        "Models/autobus/llanta_1.obj",
        "Models/autobus/llanta_2.obj",
        "Models/autobus/llanta_3.obj",
        "Models/autobus/llanta_4obj.obj",
        "Models/autobus/llanta_5.obj",
        "Models/autobus/llanta_6.obj"
    };
    const char* bodyFiles[2] = {
        "Models/autobus/carroceria_atlante.obj",
        "Models/autobus/carroceria_pumas.obj"
    };
    busAssetsReady = loadBusWheelData("Models/autobus/ruedas.txt");
    float busGroundY = 0.0f;
    if (busAssetsReady) {
        float lowestWheelPoint = busWheelData[0].position.y - busWheelData[0].radius;
        for (int i = 1; i < 6; ++i)
            lowestWheelPoint = std::min(lowestWheelPoint, busWheelData[i].position.y - busWheelData[i].radius);
        busGroundY = -busModelScale * lowestWheelPoint;
    }
    teamBuses[0] = { glm::vec3(102.0f, busGroundY, -21.0f),
        -busPi * 0.5f, 0.0f, 0.0f, false, BUS_WAITING };
    teamBuses[1] = { glm::vec3(-102.0f, busGroundY, 21.0f),
        busPi * 0.5f, 0.0f, 0.0f, false, BUS_WAITING };
    for (int i = 0; i < 2; ++i) {
        std::ifstream file(bodyFiles[i]);
        if (!file) {
            std::cerr << "Missing team bus: " << bodyFiles[i] << std::endl;
            busAssetsReady = false;
        }
    }
    for (int i = 0; i < 6; ++i) {
        std::ifstream file(wheelFiles[i]);
        if (!file) {
            std::cerr << "Missing bus wheel: " << wheelFiles[i] << std::endl;
            busAssetsReady = false;
        }
    }
    std::unique_ptr<Model> busBodyModels[2];
    std::unique_ptr<Model> busWheelModels[6];
    if (busAssetsReady) {
        for (int i = 0; i < 2; ++i) {
            VerifyModelTextures(bodyFiles[i]);
            busBodyModels[i].reset(new Model(const_cast<char*>(bodyFiles[i])));
        }
        for (int i = 0; i < 6; ++i) {
            VerifyModelTextures(wheelFiles[i]);
            busWheelModels[i].reset(new Model(const_cast<char*>(wheelFiles[i])));
        }
    }

    std::unique_ptr<Model> broadcastCameraBase;
    std::unique_ptr<Model> broadcastCameraHead;
    const std::string cameraBasePath = cameraAssetDirectory + "cameraBase.obj";
    const std::string cameraHeadPath = cameraAssetDirectory + "cameraHead.obj";
    {
        std::ifstream baseFile(cameraBasePath.c_str());
        std::ifstream headFile(cameraHeadPath.c_str());
        if (baseFile.good() && headFile.good()) {
            VerifyModelTextures(cameraBasePath);
            VerifyModelTextures(cameraHeadPath);
            broadcastCameraBase.reset(new Model(const_cast<char*>(cameraBasePath.c_str())));
            broadcastCameraHead.reset(new Model(const_cast<char*>(cameraHeadPath.c_str())));
            std::cout << "Camera meshes loaded separately: fixed base and tracking head." << std::endl;
            std::ifstream pivotFile((cameraAssetDirectory + "camera_pivot.txt").c_str());
            if (pivotFile && !(pivotFile >> cameraHeadPivot.x >> cameraHeadPivot.y >> cameraHeadPivot.z)) {
                cameraHeadPivot = glm::vec3(0.0f, 8.837823f, 0.0f);
                std::cerr << "Invalid camera_pivot.txt; using default pivot." << std::endl;
            }
            std::cout << "Camera pivot Y: " << cameraHeadPivot.y
                << ", camera scale: " << cameraModelScale
                << ", forward correction: " << cameraModelForwardYawDegrees << std::endl;
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

    // Keep the original cube unchanged; only the jersey torso needs UVs.
    static_assert(sizeof(vertices) / sizeof(vertices[0]) == 36 * 6, "Expected 36 cube vertices");
    GLfloat jerseyVertices[36 * 8];
    for (int i = 0; i < 36; ++i) {
        for (int j = 0; j < 6; ++j)
            jerseyVertices[i * 8 + j] = vertices[i * 6 + j];
        float x = vertices[i * 6], y = vertices[i * 6 + 1], nz = vertices[i * 6 + 5];
        jerseyVertices[i * 8 + 6] = nz > 0.5f ? 0.02f + (x + 0.5f) * 0.46f :
            nz < -0.5f ? 0.52f + (0.5f - x) * 0.46f : 0.005f;
        jerseyVertices[i * 8 + 7] = std::fabs(nz) > 0.5f ? 0.96f - (y + 0.5f) * 0.92f : 0.005f;
    }
    GLuint jerseyVAO, jerseyVBO;
    glGenVertexArrays(1, &jerseyVAO);
    glGenBuffers(1, &jerseyVBO);
    glBindVertexArray(jerseyVAO);
    glBindBuffer(GL_ARRAY_BUFFER, jerseyVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(jerseyVertices), jerseyVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    // Each cube face maps to its own cell in the player's cross-shaped head atlas.
    GLfloat headVertices[36 * 8];
    for (int i = 0; i < 36; ++i) {
        for (int j = 0; j < 6; ++j)
            headVertices[i * 8 + j] = vertices[i * 6 + j];
        float x = vertices[i * 6], y = vertices[i * 6 + 1], z = vertices[i * 6 + 2];
        float nx = vertices[i * 6 + 3], ny = vertices[i * 6 + 4], nz = vertices[i * 6 + 5];
        float column = nz > 0.5f ? 1.0f : nz < -0.5f ? 3.0f :
            nx > 0.5f ? 2.0f : nx < -0.5f ? 0.0f : 1.0f;
        float row = ny > 0.5f ? 0.0f : ny < -0.5f ? 2.0f : 1.0f;
        float u = nz > 0.5f ? x + 0.5f : nz < -0.5f ? 0.5f - x :
            nx > 0.5f ? 0.5f - z : nx < -0.5f ? z + 0.5f : x + 0.5f;
        float v = ny > 0.5f ? z + 0.5f : ny < -0.5f ? 0.5f - z : 0.5f - y;
        headVertices[i * 8 + 6] = (column + 0.02f + 0.96f * u) / 4.0f;
        headVertices[i * 8 + 7] = (row + 0.02f + 0.96f * v) / 3.0f;
    }
    GLuint headVAO, headVBO;
    glGenVertexArrays(1, &headVAO);
    glGenBuffers(1, &headVBO);
    glBindVertexArray(headVAO);
    glBindBuffer(GL_ARRAY_BUFFER, headVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(headVertices), headVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);


    GLuint fadeProgram = createFadeProgram();
    GLuint fadeVAO, fadeVBO;
    const float fadeVertices[] = { -1.f,-1.f, 1.f,-1.f, 1.f,1.f,
                                   -1.f,-1.f, 1.f,1.f, -1.f,1.f };
    glGenVertexArrays(1, &fadeVAO);
    glGenBuffers(1, &fadeVBO);
    glBindVertexArray(fadeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, fadeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fadeVertices), fadeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindVertexArray(0);

    // Single pixel solid color textures 
    GLuint texGrass = LoadDiffuseTexture("Models/Textures/Grass005_Color.jpg");
    if (!texGrass) texGrass = CreateSolidColorTexture(35, 145, 55);
    GLuint texWhite = CreateSolidColorTexture(245, 245, 245);
    GLuint texBlack = CreateSolidColorTexture(20, 20, 20);
    GLuint texBlue = CreateSolidColorTexture(25, 70, 210);
    GLuint texPumas = LoadDiffuseTexture("Models/Textures/pumas_navy_jersey.png");
    if (!texPumas) texPumas = CreateSolidColorTexture(14, 30, 61);
    GLuint texRed = CreateSolidColorTexture(205, 45, 45);
    GLuint texAtlante = LoadDiffuseTexture("Models/Textures/atlante_white_jersey.png");
    if (!texAtlante) texAtlante = CreateSolidColorTexture(245, 245, 245);
    GLuint texShorts = CreateSolidColorTexture(35, 35, 55);
    GLuint texSkin = CreateSolidColorTexture(205, 150, 105);
    GLuint texHead = LoadDiffuseTexture("Models/Textures/player_head_textures/atlas_emociones/atlas_neutral.png");
    if (!texHead) texHead = CreateSolidColorTexture(205, 150, 105);
    glBindTexture(GL_TEXTURE_2D, texHead);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
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
    GLuint texTowerMetal = LoadDiffuseTexture("Models/Textures/worn_shutter_diff_1k.jpg");
    if (!texTowerMetal) texTowerMetal = CreateSolidColorTexture(55, 53, 51);
    GLuint texConcrete = CreateSolidColorTexture(183, 181, 175);
    GLuint texFacade = LoadDiffuseTexture("Models/Textures/concrete_diff_1k.jpg");
    if (!texFacade) texFacade = CreateSolidColorTexture(183, 181, 175);
    GLuint texRoughConcrete = LoadDiffuseTexture("Models/Textures/rough_concrete_diff_1k.jpg");
    if (!texRoughConcrete) texRoughConcrete = CreateSolidColorTexture(183, 181, 175);
    GLuint texWalkway = CreateSolidColorTexture(112, 108, 103);
    GLuint texAsphalt = LoadDiffuseTexture("Models/Textures/clean_asphalt_diff_1k.jpg");
    if (!texAsphalt) texAsphalt = CreateSolidColorTexture(112, 108, 103);

    // spherical ball 
    BallMesh whiteMesh, blackMesh;
    createBall(whiteMesh, blackMesh);

    // Set texture units
    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse1"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_specular1"), 1);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "useImportedModelTexture"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "cameraNeutralMaterial"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "proceduralTextureMode"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texSpecular);
    glActiveTexture(GL_TEXTURE0);

    GLint textureModeLoc = glGetUniformLocation(lightingShader.Program, "proceduralTextureMode");
    GLint importedTextureLoc = glGetUniformLocation(lightingShader.Program, "useImportedModelTexture");

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
        if (!sequencePaused) {
            animationClock += deltaTime;
            DoMovement();
            updateMatch(deltaTime);
            if (matchScene == SCENE_PENALTIES && fadePhase != FADE_OUT)
                Animation();
            if (matchScene == SCENE_ARRIVAL && fadePhase != FADE_OUT)
                updateBusAnimation();
            updateFade(deltaTime);
        }
        const bool ballVisible = matchScene == SCENE_KICKOFF || matchScene == SCENE_PUMAS_GOAL ||
            matchScene == SCENE_ATLANTE_GOAL || matchScene == SCENE_PENALTIES;
        float trackingAmount = 1.0f - std::exp(-4.0f * std::max(0.0f, sequencePaused ? 0.0f : deltaTime));
        glm::vec3 trackedTarget = matchScene == SCENE_FINAL ? trophyFocusPosition :
            ballVisible ? ballPosition : glm::vec3(0.0f, 0.11f, 0.0f);
        trackedBallPosition = glm::mix(trackedBallPosition, trackedTarget, trackingAmount);
        std::string title = "PUMAS " + std::to_string(pumasScore) + " - " +
            std::to_string(atlanteScore) + " ATLANTE | " + getSceneLabel() +
            (sequencePaused ? " [PAUSA]" : "") +
            " | PEN " + std::to_string(penaltyGoals[0]) + "-" +
            std::to_string(penaltyGoals[1]) + " | P: tirar penal | SPACE: pausa | R: reiniciar";
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
        glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse1"), 0);
        glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_specular1"), 1);
        glUniform1i(glGetUniformLocation(lightingShader.Program, "useImportedModelTexture"), 0);
        glUniform1i(glGetUniformLocation(lightingShader.Program, "cameraNeutralMaterial"), 0);
        glUniform1i(glGetUniformLocation(lightingShader.Program, "proceduralTextureMode"), 0);

        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glm::vec3 viewPosition = getBroadcastPosition();
        glUniform3f(viewPosLoc, viewPosition.x, viewPosition.y, viewPosition.z);


        // Directional light
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.23f, 0.25f, 0.32f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"),
            0.18f, 0.21f, 0.30f);
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
            stadiumDiffuse.x * stadiumLightIntensity,
            stadiumDiffuse.y * stadiumLightIntensity,
            stadiumDiffuse.z * stadiumLightIntensity);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.7f, 0.7f, 0.6f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.006f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.0005f);

        for (int team = 0; team < 2; ++team) {
            for (int lamp = 0; lamp < 2; ++lamp) {
                int lightIndex = 1 + team * 2 + lamp;
                std::string name = "pointLights[" + std::to_string(lightIndex) + "].";
                glm::vec3 p = getBusHeadlightPosition(teamBuses[team], lamp);
                glUniform3f(glGetUniformLocation(lightingShader.Program, (name + "position").c_str()), p.x, p.y, p.z);
                glUniform3f(glGetUniformLocation(lightingShader.Program, (name + "ambient").c_str()), 0.0f, 0.0f, 0.0f);
                float brightness = (busAssetsReady && busHeadlightsOn) ? 2.0f : 0.0f;
                glUniform3f(glGetUniformLocation(lightingShader.Program, (name + "diffuse").c_str()), brightness, brightness * 0.96f, brightness * 0.83f);
                glUniform3f(glGetUniformLocation(lightingShader.Program, (name + "specular").c_str()), brightness * 0.3f, brightness * 0.3f, brightness * 0.3f);
                glUniform1f(glGetUniformLocation(lightingShader.Program, (name + "constant").c_str()), 1.0f);
                glUniform1f(glGetUniformLocation(lightingShader.Program, (name + "linear").c_str()), 0.13f);
                glUniform1f(glGetUniformLocation(lightingShader.Program, (name + "quadratic").c_str()), 0.07f);
            }
        }

        // SpotLight
        glm::vec3 stadiumSpotPosition = floodlightPositions[2] + glm::vec3(0.0f, 17.5f, 0.0f);
        glm::vec3 stadiumSpotDirection = glm::normalize(glm::vec3(42.0f, 0.0f, 0.0f) - stadiumSpotPosition);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"),
            stadiumSpotPosition.x, stadiumSpotPosition.y, stadiumSpotPosition.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"),
            stadiumSpotDirection.x, stadiumSpotDirection.y, stadiumSpotDirection.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.1f, 0.1f, 0.09f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"),
            1.5f * stadiumLightIntensity, 1.4f * stadiumLightIntensity,
            1.2f * stadiumLightIntensity);
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
        projection = glm::perspective(glm::radians(activeCamera == 9 ? 62.0f :
            (activeCamera == 1 || activeCamera == 2 ? 54.0f : camera.GetZoom())),
            (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 300.0f);

        skyShader.Use();
        glm::mat4 inverseProjection = glm::inverse(projection);
        glm::mat3 inverseViewRotation = glm::mat3(glm::inverse(view));
        glUniformMatrix4fv(glGetUniformLocation(skyShader.Program, "inverseProjection"), 1, GL_FALSE,
            glm::value_ptr(inverseProjection));
        glUniformMatrix3fv(glGetUniformLocation(skyShader.Program, "inverseViewRotation"), 1, GL_FALSE,
            glm::value_ptr(inverseViewRotation));
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glBindVertexArray(fadeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        lightingShader.Use();

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
        glUniform1i(textureModeLoc, 0);
        glUniform1i(importedTextureLoc, 0);
        drawPerimeter(modelLoc, textureModeLoc, VAO, modelTemp, texConcrete, texFacade, texAsphalt, texWhite);
        drawField(modelLoc, textureModeLoc, VAO, modelTemp, texGrass, texWhite);
        drawGoal(modelLoc, VAO, modelTemp, -52.5f, -1.0f, texPosts, texNet);
        drawGoal(modelLoc, VAO, modelTemp, 52.5f, 1.0f, texPosts, texNet);

        const bool penaltyPlayersVisible = matchScene == SCENE_PENALTIES;
        if (penaltyPlayersVisible) {
            drawPlayer(modelLoc, VAO, jerseyVAO, headVAO, modelTemp,
                glm::vec3(40.4f, 0.0f, 0.0f), 90.0f,
                penaltyKickerTeam == 0 ? texPumas : texAtlante,
                texHead,
                penaltyKickerTeam == 0 ? texShorts : texRed,
                texSkin, texShoes, kickAngle, 0.0f, 0.0f, 10, texWhite);

            drawPlayer(modelLoc, VAO, jerseyVAO, headVAO, modelTemp,
                glm::vec3(51.2f, 0.0f, keeperPositionZ), -90.0f,
                penaltyKickerTeam == 0 ? texAtlante : texPumas,
                texHead,
                penaltyKickerTeam == 0 ? texRed : texShorts,
                texSkin, texShoes, 0.0f, keeperDive, 0.0f, 1, texWhite);
        }

        if (arrivalStarted && (matchScene == SCENE_ARRIVAL || matchScene == SCENE_IDLE)) {
            for (int team = 0; team < 2; ++team) {
                if (teamBuses[team].state != BUS_COMPLETED || teamLineupTime[team] <= 0.0f) continue;
                for (int player = 0; player < playersPerTeam; ++player) {
                    if (penaltyPlayersVisible && player == playersPerTeam - 1) continue;
                    float walkTime = teamLineupTime[team] - player * playerExitDelay;
                    if (walkTime < 0.0f) continue;
                    float t = glm::clamp(walkTime / playerWalkDuration, 0.0f, 1.0f);
                    float eased = t * t * (3.0f - 2.0f * t);
                    glm::vec3 start = getPlayerExitPosition(team);
                    glm::vec3 finish = getPlayerLineupPosition(team, player);
                    glm::vec3 playerPosition = glm::mix(start, finish, eased);
                    glm::vec3 movement = finish - start;
                    float yaw = t < 1.0f ? glm::degrees(std::atan2(movement.x, movement.z)) :
                        (team == 0 ? -90.0f : 90.0f);
                    float step = t < 1.0f ? 23.0f * sinf(walkTime * 9.0f) : 0.0f;
                    drawPlayer(modelLoc, VAO, jerseyVAO, headVAO, modelTemp,
                        playerPosition, yaw,
                        team == 0 ? texAtlante : texPumas,
                        texHead,
                        team == 0 ? texRed : texShorts, texSkin, texShoes,
                        0.0f, 0.0f, step, player + 1, texWhite);
                }
            }
        }

        if (matchScene == SCENE_FINAL) {
            int champion = penaltyWinner >= 0 ? penaltyWinner : (pumasScore >= atlanteScore ? 1 : 0);
            for (int team = 0; team < 2; ++team) {
                for (int player = 0; player < playersPerTeam; ++player) {
                    glm::vec3 place;
                    float yaw = team == champion ? 180.0f : 0.0f;
                    float walk = 3.0f * sinf(sceneTime * 2.0f + player);
                    if (team == champion) {
                        if (player == 5) {
                            place = glm::vec3(0.0f, 0.0f, 38.0f);
                            drawPlayer(modelLoc, VAO, jerseyVAO, headVAO, modelTemp, place, 180.0f,
                                team == 0 ? texAtlante : texPumas,
                                texHead,
                                team == 0 ? texRed : texShorts, texSkin, texShoes,
                                8.0f, 0.0f, 0.0f, player + 1, texWhite);
                            continue;
                        }
                        int idx = player < 5 ? player : player - 1;
                        float side = idx < 5 ? -1.0f : 1.0f;
                        int rank = idx < 5 ? idx : idx - 5;
                        place = glm::vec3(side * (4.5f + rank * 2.2f), 0.0f, 41.0f + (rank - 2.0f) * 1.6f);
                    }
                    else {
                        place = glm::vec3(-10.0f + player * 2.0f, 0.0f, 48.0f);
                    }
                    drawPlayer(modelLoc, VAO, jerseyVAO, headVAO, modelTemp,
                        place, yaw,
                        team == 0 ? texAtlante : texPumas,
                        texHead,
                        team == 0 ? texRed : texShorts, texSkin, texShoes,
                        0.0f, 0.0f, walk, player + 1, texWhite);
                }
            }
        }

        if (matchScene == SCENE_KICKOFF || matchScene == SCENE_PUMAS_GOAL || matchScene == SCENE_ATLANTE_GOAL) {
            for (int team = 0; team < 2; ++team) {
                float side = team == 0 ? 1.0f : -1.0f;
                for (int player = 0; player < playersPerTeam; ++player) {
                    float rowX = player == 0 ? 47.0f :
                        (player < 5 ? 31.0f : (player < 9 ? 16.0f : 5.0f));
                    float across = player == 0 ? 0.0f :
                        (player < 5 ? (player - 2.5f) * 10.0f :
                            (player < 9 ? (player - 6.5f) * 11.0f : (player - 9.5f) * 12.0f));
                    glm::vec3 place(side * rowX, 0.0f, across);
                    float step = 1.8f * sinf(sceneTime * 2.0f + player);
                    float kick = 0.0f;
                    float dive = 0.0f;
                    float yaw = team == 0 ? -90.0f : 90.0f;

                    if (matchScene == SCENE_KICKOFF) {
                        if (team == 1 && player == 9) {
                            place = glm::vec3(0.0f, 0.0f, 0.0f);
                            kick = sceneTime < 1.0f ? 42.0f * sinf(sceneTime * 3.14159f) : 0.0f;
                            step = 0.0f;
                        }
                        if (team == 1 && player == 10) {
                            float t = glm::clamp((sceneTime - 1.0f) / 2.3f, 0.0f, 1.0f);
                            glm::vec3 start(-2.0f, 0.0f, 7.0f), end(-16.0f, 0.0f, 14.0f);
                            place = glm::mix(start, end, t);
                            step = t < 1.0f ? 22.0f * sinf(sceneTime * 9.0f) : 0.0f;
                            yaw = -120.0f;
                        }
                    }
                    else if (matchScene == SCENE_PUMAS_GOAL) {
                        if (team == 1) {
                            if (player == 6) {
                                place = glm::vec3(50.9f, 0.0f, -33.5f);
                                kick = sceneTime < 2.0f * pumasCornerKickTime ?
                                    30.0f * sinf(sceneTime / (2.0f * pumasCornerKickTime) * busPi) : 0.0f;
                                yaw = -16.0f;
                                step = 0.0f;
                            }
                            if (player == 8) {
                                float t = glm::clamp(sceneTime / 2.5f, 0.0f, 1.0f);
                                place = glm::mix(glm::vec3(32.0f, 0.0f, -10.0f), glm::vec3(43.0f, 0.0f, -5.0f), t);
                                step = t < 1.0f ? 24.0f * sinf(sceneTime * 8.5f) : 0.0f;
                                if (sceneTime > 2.2f && sceneTime < 2.9f) kick = -15.0f;
                                yaw = 105.0f;
                            }
                            if (player == 9) {
                                float t = glm::clamp((sceneTime - 2.5f) / 1.5f, 0.0f, 1.0f);
                                place = glm::mix(glm::vec3(39.0f, 0.0f, 6.0f), glm::vec3(46.0f, 0.0f, 0.0f), t);
                                step = t < 1.0f ? 20.0f * sinf(sceneTime * 9.5f) : 0.0f;
                                if (sceneTime > pumasShotTime - 0.3f && sceneTime < pumasShotTime + 0.3f)
                                    kick = 36.0f * sinf((sceneTime - (pumasShotTime - 0.3f)) / 0.6f * busPi);
                                yaw = 90.0f;
                            }
                        }
                        if (team == 0 && player < 4) {
                            float t = glm::clamp(sceneTime / 3.2f, 0.0f, 1.0f);
                            place = glm::mix(place, glm::vec3(47.0f - player * 1.5f, 0.0f, -8.0f + player * 6.0f), t);
                            step = t < 1.0f ? 14.0f * sinf(sceneTime * 7.5f + player) : 0.0f;
                            yaw = -90.0f;
                        }
                    }
                    else if (matchScene == SCENE_ATLANTE_GOAL) {
                        if (team == 0) {
                            if (player == 9) {
                                place = glm::vec3(-28.0f, 0.0f, 0.0f);
                                kick = sceneTime > 0.7f && sceneTime < 1.5f ? 40.0f * sinf((sceneTime - 0.7f) / 0.8f * 3.14159f) : 0.0f;
                                yaw = -90.0f;
                            }
                            if (player == 8) {
                                float t = glm::clamp(sceneTime / 2.0f, 0.0f, 1.0f);
                                place = glm::mix(glm::vec3(-18.0f, 0.0f, -6.0f), glm::vec3(-23.0f, 0.0f, -10.0f), t);
                                step = 18.0f * sinf(sceneTime * 8.0f);
                            }
                        }
                        if (team == 1 && player < 3) {
                            place = glm::vec3(-40.0f, 0.0f, -4.0f + player * 4.0f);
                            if (sceneTime > 1.0f && sceneTime < 1.9f) kick = 8.0f * sinf((sceneTime - 1.0f) / 0.9f * 3.14159f);
                            yaw = 90.0f;
                        }
                        if (team == 1 && player == 0) {
                            float t = glm::clamp((sceneTime - 1.6f) / 1.2f, 0.0f, 1.0f);
                            place = glm::mix(glm::vec3(-50.0f, 0.0f, 0.0f), glm::vec3(-52.0f, 0.0f, -4.0f), t);
                            dive = -55.0f * t;
                            yaw = 90.0f;
                        }
                    }

                    drawPlayer(modelLoc, VAO, jerseyVAO, headVAO, modelTemp,
                        place, yaw,
                        team == 0 ? texAtlante : texPumas,
                        texHead,
                        team == 0 ? texRed : texShorts, texSkin, texShoes,
                        kick, dive, step, player + 1, texWhite);
                }
            }
        }
        if (ballVisible)
            drawBall(modelLoc, modelTemp, whiteMesh, blackMesh,
                texWhite, texBlack, rotBall, ballPosition);
        drawBleachers(modelLoc, textureModeLoc, VAO, modelTemp, texRoughConcrete, texFacade,
            texRed, texWhite, texWalkway, texFloodlight);
        drawTrees(modelLoc, textureModeLoc, VAO, modelTemp, whiteMesh, blackMesh,
            texBark, texLeavesDark, texLeavesLight);
        drawTrophy(modelLoc, VAO, modelTemp, whiteMesh, blackMesh,
            animationClock, texGold, texBlack, texWhite);
        for (int i = 0; i < 2; ++i) {
            if (activeCamera == i + 1) continue;
            drawDrone(modelLoc, VAO, modelTemp, whiteMesh, blackMesh,
                broadcastCameraPositions[i], trackedBallPosition, animationClock,
                texShorts, texBlack, texFloodlight, texBlue);
        }
        drawFloodlightTowers(modelLoc, textureModeLoc, VAO, modelTemp, texTowerMetal);
        glUniform1i(textureModeLoc, 0);
        glUniform1i(importedTextureLoc, 1);
        drawBroadcastCameraModels(modelLoc, lightingShader,
            broadcastCameraBase.get(), broadcastCameraHead.get());
        glUniform1i(importedTextureLoc, 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texSpecular);
        glActiveTexture(GL_TEXTURE0);
        drawBird(modelLoc, VAO, modelTemp, whiteMesh, blackMesh,
            texBird, texBeak, animationClock);
        if (fireworksActive) {
            float elapsed = animationClock - fireworksStartTime;
            if (elapsed < 4.8f)
                drawFireworks(modelLoc, VAO, modelTemp, elapsed,
                    texGold, texBlue, texRed, texWhite);
            else
                fireworksActive = false;
        }
        if (busAssetsReady) {
            glUniform1i(textureModeLoc, 0);
            glUniform1i(importedTextureLoc, 1);
            for (int team = 0; team < 2; ++team) {
                const TeamBus& bus = teamBuses[team];
                glm::mat4 busMatrix = getBusModelMatrix(bus);
                glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(busMatrix));
                busBodyModels[team]->Draw(lightingShader);
                for (int i = 0; i < 6; ++i) {
                    const glm::vec3 wheelCenter = busWheelData[i].position;
                    glm::mat4 wheelMatrix = glm::translate(busMatrix, wheelCenter);
                    if (busWheelData[i].isFrontWheel)
                        wheelMatrix = glm::rotate(wheelMatrix, bus.steeringAngle,
                            glm::vec3(0.0f, 1.0f, 0.0f));
                    float rollingAngle = -bus.distanceTraveled / (busModelScale * busWheelData[i].radius);
                    wheelMatrix = glm::rotate(wheelMatrix, rollingAngle,
                        glm::vec3(1.0f, 0.0f, 0.0f));
                    wheelMatrix = glm::translate(wheelMatrix, -wheelCenter);
                    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(wheelMatrix));
                    busWheelModels[i]->Draw(lightingShader);
                }
            }
            glUniform1i(importedTextureLoc, 0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texSpecular);
            glActiveTexture(GL_TEXTURE0);
        }

        glUniform1i(textureModeLoc, 0);
        glUniform1i(importedTextureLoc, 0);
        drawMatchScoreboard(modelLoc, VAO, modelTemp, texBlack, texWhite, texGold, texRed);

        lampShader.Use();
        GLint lampModelLoc = glGetUniformLocation(lampShader.Program, "model");
        GLint lampViewLoc = glGetUniformLocation(lampShader.Program, "view");
        GLint lampProjectionLoc = glGetUniformLocation(lampShader.Program, "projection");
        glUniformMatrix4fv(lampViewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(lampProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        drawFloodlightPanels(lampModelLoc, VAO);
        if (busAssetsReady && busHeadlightsOn) {
            glBindVertexArray(VAO);
            for (int team = 0; team < 2; ++team) {
                glm::mat4 busMatrix = getBusModelMatrix(teamBuses[team]);
                for (int lamp = 0; lamp < 2; ++lamp) {
                    glm::mat4 lightMatrix = glm::translate(busMatrix,
                        glm::vec3(lamp == 0 ? -0.14f : 0.87f, 1.95f, 6.75f));
                    lightMatrix = glm::scale(lightMatrix, glm::vec3(0.19f, 0.14f, 0.08f));
                    glUniformMatrix4fv(lampModelLoc, 1, GL_FALSE, glm::value_ptr(lightMatrix));
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }
            }
            glBindVertexArray(0);
        }
        glBindVertexArray(0);

        drawFadeOverlay(fadeProgram, fadeVAO);
        glfwSwapBuffers(window);
    }

    //DELETE BLOCK. THIS FREE MEMORY 
    deleteBallMesh(whiteMesh);
    deleteBallMesh(blackMesh);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &jerseyVAO);
    glDeleteBuffers(1, &jerseyVBO);
    glDeleteVertexArrays(1, &fadeVAO);
    glDeleteBuffers(1, &fadeVBO);
    glDeleteProgram(fadeProgram);
    glDeleteProgram(skyShader.Program);
    GLuint textures[] = { texGrass, texWhite, texBlack, texBlue, texPumas, texRed, texAtlante,
                         texShorts, texSkin, texShoes, texPosts, texNet,
                         texSpecular, texBark, texLeavesDark,
                         texLeavesLight, texBird, texBeak, texGold, texFloodlight, texTowerMetal, texConcrete, texWalkway, texAsphalt, texFacade, texRoughConcrete };
    glDeleteTextures(sizeof(textures) / sizeof(textures[0]), textures);
    for (int i = 0; i < 2; ++i) busBodyModels[i].reset();
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
    if (activeCamera == 1 || activeCamera == 2) {
        glm::vec3& dronePosition = broadcastCameraPositions[activeCamera - 1];
        glm::vec3 horizontal = trackedBallPosition - dronePosition;
        horizontal.y = 0.0f;
        if (glm::dot(horizontal, horizontal) < 0.001f)
            horizontal = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 forward = glm::normalize(horizontal);
        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
        float velocity = 11.0f * std::min(deltaTime, 0.1f);
        if (keys[GLFW_KEY_LEFT_SHIFT] || keys[GLFW_KEY_RIGHT_SHIFT])
            velocity *= 2.0f;
        if (keys[GLFW_KEY_W]) dronePosition += forward * velocity;
        if (keys[GLFW_KEY_S]) dronePosition -= forward * velocity;
        if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) dronePosition -= right * velocity;
        if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) dronePosition += right * velocity;
        if (keys[GLFW_KEY_Q]) dronePosition.y -= velocity;
        if (keys[GLFW_KEY_E]) dronePosition.y += velocity;
        dronePosition.x = glm::clamp(dronePosition.x, -64.0f, 64.0f);
        dronePosition.y = glm::clamp(dronePosition.y, 7.0f, 34.0f);
        dronePosition.z = glm::clamp(dronePosition.z, -47.0f, 47.0f);
        return;
    }
    if (activeCamera != 0)
        return;

    glm::vec3 forward = camera.GetFront();
    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
    float step = 7.5f * std::min(deltaTime, 0.1f);
    if (keys[GLFW_KEY_W]) freeCameraPosition += forward * step;
    if (keys[GLFW_KEY_S]) freeCameraPosition -= forward * step;
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) freeCameraPosition -= right * step;
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) freeCameraPosition += right * step;
    freeCameraPosition.x = glm::clamp(freeCameraPosition.x, -98.0f, 98.0f);
    freeCameraPosition.y = glm::clamp(freeCameraPosition.y, 1.0f, 32.0f);
    freeCameraPosition.z = glm::clamp(freeCameraPosition.z, -75.0f, 75.0f);

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
        firstMouse = true;
    }
    if (key == GLFW_KEY_LEFT_BRACKET)
        cameraModelForwardYawDegrees -= 90.0f;
    if (key == GLFW_KEY_RIGHT_BRACKET)
        cameraModelForwardYawDegrees += 90.0f;
    if (key == GLFW_KEY_UP)
        stadiumLightIntensity = std::min(2.5f, stadiumLightIntensity + 0.15f);
    if (key == GLFW_KEY_DOWN)
        stadiumLightIntensity = std::max(0.15f, stadiumLightIntensity - 0.15f);
    if (key == GLFW_KEY_H)
        busHeadlightsOn = !busHeadlightsOn;
    if (key == GLFW_KEY_R) {
        resetMatch();
        return;
    }
    if (key == GLFW_KEY_SPACE) {
        if (matchScene == SCENE_IDLE) {
            resetMatch();
            autoSequence = true;
            beginMatchScene(SCENE_ARRIVAL, true);
        }
        else {
            sequencePaused = !sequencePaused;
        }
        return;
    }
    if (key == GLFW_KEY_L) {
        resetMatch();
        autoSequence = true;
        beginMatchScene(SCENE_ARRIVAL, true);
        return;
    }
    MatchScene chosen = SCENE_IDLE;
    if (key == GLFW_KEY_Z) chosen = SCENE_ARRIVAL;
    if (key == GLFW_KEY_X) chosen = SCENE_KICKOFF;
    if (key == GLFW_KEY_C) chosen = SCENE_PUMAS_GOAL;
    if (key == GLFW_KEY_V) chosen = SCENE_ATLANTE_GOAL;
    if (key == GLFW_KEY_N) {
        chosen = SCENE_PENALTIES;
    }
    if (key == GLFW_KEY_M) chosen = SCENE_FINAL;
    if (chosen != SCENE_IDLE) {
        autoSequence = false;
        sequencePaused = false;
        requestMatchScene(static_cast<int>(chosen));
        return;
    }
    if (key == GLFW_KEY_P && matchScene == SCENE_PENALTIES && fadePhase == FADE_NONE) {
        autoSequence = false;
        if (penaltyWinner < 0) {
            if (penaltyState == READY) penaltyRequested = true;
        }
        return;
    }
    if (key == GLFW_KEY_F) {
        fireworksActive = true;
        fireworksStartTime = animationClock;
    }
}

void resetMatch()
{
    matchScene = SCENE_IDLE;
    nextScene = SCENE_IDLE;
    fadePhase = FADE_NONE;
    autoSequence = false;
    sequencePaused = false;
    fadeAlpha = 0.0f;
    sceneTime = 0.0f;
    pumasScore = atlanteScore = 0;
    matchMinute = matchSecond = 0;
    penaltyTaken[0] = penaltyTaken[1] = 0;
    penaltyGoals[0] = penaltyGoals[1] = 0;
    penaltyKickerTeam = 0;
    penaltyWinner = -1;
    penaltyNextKickerTeam = -1;
    penaltyShotRecorded = false;
    penaltyRestTime = 0.0f;
    arrivalStarted = false;
    fireworksActive = false;
    for (int team = 0; team < 2; ++team) {
        float sign = team == 0 ? 1.0f : -1.0f;
        TeamBus& bus = teamBuses[team];
        bus.position = glm::vec3(sign * 102.0f, bus.position.y, -sign * 21.0f);
        bus.heading = -sign * busPi * 0.5f;
        bus.distanceTraveled = 0.0f;
        bus.steeringAngle = 0.0f;
        bus.moving = false;
        bus.state = BUS_WAITING;
        teamLineupTime[team] = 0.0f;
    }
    ResetPenalty();
    goals = saves = misses = 0;
    activeCamera = 9;
}

void beginMatchScene(int next, bool automatic)
{
    matchScene = static_cast<MatchScene>(next);
    autoSequence = automatic;
    sceneTime = 0.0f;
    fadePhase = FADE_IN;
    fadeAlpha = 1.0f;
    if (matchScene == SCENE_ARRIVAL) {
        startArrival();
    }
    if (matchScene == SCENE_KICKOFF) {
        activeCamera = 9;
        ballPosition = glm::vec3(0.0f, 0.11f, 0.0f);
        matchMinute = matchSecond = 0;
    }
    if (matchScene == SCENE_PUMAS_GOAL) {
        activeCamera = 9;
        pumasScore = 0;
        atlanteScore = 0;
        matchMinute = 43;
        matchSecond = 0;
        ballPosition = glm::vec3(50.8f, 0.11f, -33.0f);
    }
    if (matchScene == SCENE_ATLANTE_GOAL) {
        activeCamera = 9;
        pumasScore = 1;
        atlanteScore = 0;
        matchMinute = 87;
        matchSecond = 0;
        ballPosition = glm::vec3(-28.0f, 0.11f, 0.0f);
    }
    if (matchScene == SCENE_PENALTIES) {
        activeCamera = 4;
        pumasScore = 1;
        atlanteScore = 1;
        matchMinute = 90;
        matchSecond = 0;
        penaltyTaken[0] = penaltyTaken[1] = 0;
        penaltyGoals[0] = penaltyGoals[1] = 0;
        penaltyKickerTeam = 0;
        penaltyWinner = -1;
        penaltyNextKickerTeam = -1;
        penaltyShotRecorded = false;
        penaltyRestTime = 0.0f;
        ResetPenalty();
        goals = saves = misses = 0;
    }
    if (matchScene == SCENE_FINAL) {
        activeCamera = 8;
        matchMinute = 90;
        matchSecond = 0;
        fireworksActive = true;
        fireworksStartTime = animationClock;
    }
}

void requestMatchScene(int next)
{
    if (matchScene == SCENE_IDLE && fadePhase == FADE_NONE) {
        beginMatchScene(next, autoSequence);
        return;
    }
    nextScene = static_cast<MatchScene>(next);
    fadePhase = FADE_OUT;
}

void updateFade(float step)
{
    if (fadePhase == FADE_OUT) {
        fadeAlpha = std::min(1.0f, fadeAlpha + step / transitionDuration);
        if (fadeAlpha >= 1.0f) {
            int following = static_cast<int>(nextScene);
            beginMatchScene(following, autoSequence);
            fadeAlpha = 1.0f;
        }
    }
    else if (fadePhase == FADE_IN) {
        fadeAlpha = std::max(0.0f, fadeAlpha - step / transitionDuration);
        if (fadeAlpha <= 0.0f) fadePhase = FADE_NONE;
    }
}

void updateMatch(float step)
{
    if (matchScene == SCENE_IDLE || fadePhase == FADE_OUT) return;
    if (fadePhase == FADE_IN) return;
    sceneTime += step;
    if (matchScene == SCENE_ARRIVAL) {
        if (autoSequence && isLineupComplete()) requestMatchScene(SCENE_KICKOFF);
        return;
    }
    if (matchScene == SCENE_KICKOFF) {
        float t = sceneTime;
        if (t < 1.0f)
            ballPosition = glm::vec3(0.0f, 0.11f, 0.0f);
        else if (t < 2.3f) {
            float a = glm::clamp((t - 1.0f) / 1.3f, 0.0f, 1.0f);
            ballPosition = glm::mix(glm::vec3(0.0f, 0.11f, 0.0f), glm::vec3(-7.0f, 0.11f, 6.0f), a);
        }
        else if (t < 4.6f) {
            float a = glm::clamp((t - 2.3f) / 2.3f, 0.0f, 1.0f);
            ballPosition = glm::mix(glm::vec3(-7.0f, 0.11f, 6.0f), glm::vec3(-18.0f, 0.11f, 12.0f), a);
        }
        matchMinute = 0;
        matchSecond = std::min(59, static_cast<int>(sceneTime * 6.0f));
        if (autoSequence && sceneTime >= 6.2f) requestMatchScene(SCENE_PUMAS_GOAL);
        return;
    }
    if (matchScene == SCENE_PUMAS_GOAL) {
        float t = sceneTime;
        glm::vec3 start(50.8f, 0.11f, -33.0f);
        glm::vec3 header(43.0f, 2.0f, -5.0f);
        glm::vec3 scorer(46.5f, 0.35f, 0.0f);
        glm::vec3 net(53.0f, 1.6f, 0.0f);
        if (t < 2.5f) {
            float a = glm::clamp((t - pumasCornerKickTime) / (2.5f - pumasCornerKickTime), 0.0f, 1.0f);
            ballPosition = glm::mix(start, header, a) + glm::vec3(0, 3.2f * sinf(busPi * a), 0);
        }
        else if (t < 4.0f) {
            float a = glm::clamp((t - 2.5f) / 1.5f, 0.0f, 1.0f);
            ballPosition = glm::mix(header, scorer, a) + glm::vec3(0, 0.5f * sinf(busPi * a), 0);
        }
        else {
            float a = glm::clamp((t - pumasShotTime) / (5.2f - pumasShotTime), 0.0f, 1.0f);
            ballPosition = glm::mix(scorer, net, a);
        }
        matchSecond = std::min(59, static_cast<int>(t));
        if (t >= 5.2f) pumasScore = 1;
        if (autoSequence && t >= 7.0f) requestMatchScene(SCENE_ATLANTE_GOAL);
        return;
    }
    if (matchScene == SCENE_ATLANTE_GOAL) {
        float a = glm::clamp((sceneTime - 1.3f) / 2.5f, 0.0f, 1.0f);
        glm::vec3 from(-28.0f, 0.11f, 0.0f), to(-53.0f, 2.0f, -3.1f);
        ballPosition = glm::mix(from, to, a) + glm::vec3(0, 4.8f * sinf(busPi * a), -2.5f * sinf(busPi * a));
        matchSecond = std::min(59, static_cast<int>(sceneTime));
        if (sceneTime >= 4.1f) atlanteScore = 1;
        if (autoSequence && sceneTime >= 6.5f) requestMatchScene(SCENE_PENALTIES);
        return;
    }
    if (matchScene == SCENE_PENALTIES) {
        matchMinute = 90;
        if (penaltyState == FINISHED && !penaltyShotRecorded) {
            penaltyTaken[penaltyKickerTeam]++;
            if (penaltyResult == "GOAL") penaltyGoals[penaltyKickerTeam]++;
            penaltyShotRecorded = true;
            penaltyRestTime = 0.0f;
            penaltyNextKickerTeam = penaltyKickerTeam;
            int remainingPumas = std::max(0, 5 - penaltyTaken[0]);
            int remainingAtlante = std::max(0, 5 - penaltyTaken[1]);
            if (penaltyGoals[0] > penaltyGoals[1] + remainingAtlante)
                penaltyWinner = 0;
            else if (penaltyGoals[1] > penaltyGoals[0] + remainingPumas)
                penaltyWinner = 1;
            else if (penaltyTaken[0] == penaltyTaken[1] && penaltyTaken[0] >= 5 &&
                penaltyGoals[0] != penaltyGoals[1])
                penaltyWinner = penaltyGoals[0] > penaltyGoals[1] ? 0 : 1;
            if (penaltyWinner >= 0) {
                std::cout << "Winner: " << (penaltyWinner == 0 ? "Pumas" : "Atlante") << std::endl;
                if (autoSequence) requestMatchScene(SCENE_FINAL);
            }
            else {
                penaltyNextKickerTeam = 1 - penaltyKickerTeam;
            }
        }
        if (penaltyShotRecorded) {
            penaltyRestTime += step;
            if (penaltyRestTime > 1.1f && penaltyWinner < 0) {
                penaltyKickerTeam = penaltyNextKickerTeam;
                ResetPenalty();
                penaltyShotRecorded = false;
                penaltyRestTime = 0.0f;
                if (autoSequence) penaltyRequested = true;
            }
        }
        if (autoSequence && penaltyWinner < 0 && !penaltyShotRecorded && penaltyState == READY && sceneTime >= 1.3f) {
            penaltyRequested = true;
        }
        return;
    }
    if (matchScene == SCENE_FINAL) {
        pumasScore = std::max(1, pumasScore);
        atlanteScore = std::max(1, atlanteScore);
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
    if (activeCamera != 0) {
        firstMouse = true;
        return;
    }
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
    glActiveTexture(GL_TEXTURE0);
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

void VerifyModelTextures(const std::string& modelPath)
{
    std::ifstream objFile(modelPath.c_str());
    if (!objFile) {
        std::cerr << "OBJ missing: " << modelPath << std::endl;
        return;
    }
    const std::string::size_type slash = modelPath.find_last_of("/\\");
    const std::string folder = slash == std::string::npos ? "" : modelPath.substr(0, slash + 1);
    std::string line;
    bool foundUV = false;
    bool foundMaterials = false;
    while (std::getline(objFile, line)) {
        if (line.compare(0, 3, "vt ") == 0) foundUV = true;
        if (line.compare(0, 7, "mtllib ") != 0) continue;
        foundMaterials = true;
        std::string mtlName = line.substr(7);
        if (!mtlName.empty() && mtlName.back() == '\r') mtlName.pop_back();
        for (char& c : mtlName) if (c == '\\') c = '/';
        std::ifstream mtlFile((folder + mtlName).c_str());
        if (!mtlFile) {
            std::cerr << "MTL missing: " << folder + mtlName << std::endl;
            continue;
        }
        std::string mtlLine;
        while (std::getline(mtlFile, mtlLine)) {
            if (mtlLine.compare(0, 7, "map_Kd ") != 0) continue;
            std::string image = mtlLine.substr(7);
            while (!image.empty() && (image.back() == '\r' || image.back() == ' ')) image.pop_back();
            while (!image.empty() && image.front() == ' ') image.erase(image.begin());
            for (char& c : image) if (c == '\\') c = '/';
            if (!image.empty() && image.front() == '"' && image.back() == '"')
                image = image.substr(1, image.size() - 2);
            if (image.empty()) continue;
            std::ifstream imageFile((folder + image).c_str(), std::ios::binary);
            if (!imageFile) std::cerr << "Texture missing for " << modelPath << ": " << folder + image << std::endl;
        }
    }
    if (!foundMaterials) std::cerr << "OBJ without MTL reference: " << modelPath << std::endl;
    if (!foundUV) std::cerr << "OBJ without UV coordinates (vt): " << modelPath << std::endl;
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

void drawPerimeter(GLint modelLoc, GLint textureModeLoc, GLuint VAO,
    const glm::mat4& scene, GLuint texWall, GLuint texFacade, GLuint texFloor, GLuint texAccent)
{
    // Incluye los árboles (|z| <= 57.2) y el recorrido del autobús (x <= 85).
    constexpr float halfLength = 103.0f, halfWidth = 80.0f;
    static_assert(halfLength > 85.0f && halfWidth > 71.2f,
        "El perimetro debe contener el autobus y los arboles");
    glUniform1i(textureModeLoc, 4);
    drawCube(modelLoc, VAO, scene, glm::vec3(0.0f, -0.22f, 0.0f),
        glm::vec3(2.0f * halfLength, 0.22f, 2.0f * halfWidth), texFloor);
    glUniform1i(textureModeLoc, 0);

    glUniform1i(textureModeLoc, 5);
    for (int side = -1; side <= 1; side += 2) {
        // Muros de las cabeceras: unen los dos laterales sin cerrar la vista superior.
        float gateCenterZ = -side * 21.0f;
        float gateHalfWidth = 7.0f;
        float lowEnd = gateCenterZ - gateHalfWidth;
        float highBegin = gateCenterZ + gateHalfWidth;
        for (int part = 0; part < 2; ++part) {
            float from = (part == 0) ? -halfWidth : highBegin;
            float to = (part == 0) ? lowEnd : halfWidth;
            float center = (from + to) * 0.5f;
            float width = to - from;
            drawCube(modelLoc, VAO, scene, glm::vec3(side * halfLength, 2.0f, center),
                glm::vec3(0.5f, 4.0f, width), texFacade);
            drawCube(modelLoc, VAO, scene, glm::vec3(side * (halfLength + 0.30f), 3.65f, center),
                glm::vec3(0.10f, 0.22f, width), texAccent);
        }
        for (int edge = -1; edge <= 1; edge += 2)
            drawCube(modelLoc, VAO, scene,
                glm::vec3(side * halfLength, 2.5f, gateCenterZ + edge * gateHalfWidth),
                glm::vec3(0.85f, 5.0f, 0.85f), texWall);
        drawCube(modelLoc, VAO, scene,
            glm::vec3(side * halfLength, 5.1f, gateCenterZ),
            glm::vec3(0.9f, 0.35f, gateHalfWidth * 2.0f + 0.9f), texAccent);

        // Laterales con dos vanos alineados con los pasillos entre gradas.
        for (int section = -1; section <= 1; section += 2) {
            float x = section * (halfLength + 2.0f) * 0.5f;
            drawCube(modelLoc, VAO, scene, glm::vec3(x, 2.0f, side * halfWidth),
                glm::vec3(halfLength - 2.0f, 4.0f, 0.5f), texFacade);
            drawCube(modelLoc, VAO, scene, glm::vec3(x, 3.65f, side * (halfWidth + 0.30f)),
                glm::vec3(halfLength - 2.0f, 0.22f, 0.10f), texAccent);
        }
        drawCube(modelLoc, VAO, scene, glm::vec3(0.0f, -0.01f, side * 72.0f),
            glm::vec3(4.0f, 0.08f, 16.0f), texWall);
        for (int jamb = -1; jamb <= 1; jamb += 2)
            drawCube(modelLoc, VAO, scene, glm::vec3(jamb * 2.25f, 1.8f, side * halfWidth),
                glm::vec3(0.5f, 3.6f, 0.7f), texFacade);
        drawCube(modelLoc, VAO, scene, glm::vec3(0.0f, 3.95f, side * halfWidth),
            glm::vec3(5.0f, 0.7f, 0.7f), texFacade);
    }
    glUniform1i(textureModeLoc, 0);
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

void drawPlayer(GLint modelLoc, GLuint VAO, GLuint torsoVAO, GLuint headVAO, const glm::mat4& scene,
    glm::vec3 position, float yaw, GLuint texJersey, GLuint texHead,
    GLuint texShorts, GLuint texSkin, GLuint texShoes,
    float kickAngle, float diveAngle, float walkAngle, int jerseyNumber, GLuint numberColor)
{
    // Hierarchical modeling: shoulder -> elbow and hip -> knee.
    glm::mat4 model = glm::translate(scene, position);
    model = glm::rotate(model, glm::radians(diveAngle), glm::vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(yaw), glm::vec3(0, 1, 0));
    glm::mat4 modelTemp = model; // Same principle used in the original example.

    drawCube(modelLoc, torsoVAO, modelTemp, glm::vec3(0, 1.20f, 0),
        glm::vec3(0.45f, 0.64f, 0.25f), texJersey);
    if (jerseyNumber > 0 && numberColor != 0) {
        drawCube(modelLoc, VAO, modelTemp, glm::vec3(0, 1.20f, -0.131f),
            glm::vec3(0.31f, 0.32f, 0.014f), texShorts);
        float left = jerseyNumber >= 10 ? -0.12f : -0.06f;
        if (jerseyNumber >= 10)
            drawScoreDigit(modelLoc, VAO, modelTemp, jerseyNumber / 10,
                left, 1.10f, -0.146f, 0.11f, numberColor);
        drawScoreDigit(modelLoc, VAO, modelTemp, jerseyNumber % 10,
            jerseyNumber >= 10 ? 0.015f : left, 1.10f, -0.146f,
            0.11f, numberColor);
    }
    drawCube(modelLoc, VAO, modelTemp, glm::vec3(0, 1.55f, 0),
        glm::vec3(0.12f, 0.13f, 0.12f), texSkin);
    drawCube(modelLoc, headVAO, modelTemp, glm::vec3(0, 1.77f, 0),
        glm::vec3(0.27f, 0.29f, 0.27f), texHead);

    for (int side = -1; side <= 1; side += 2) {
        // Arm: both segments are positioned relative to their joint.
        glm::mat4 shoulder = glm::translate(modelTemp, glm::vec3(side * 0.3f, 1.43f, 0));
        shoulder = glm::rotate(shoulder, glm::radians(-20.0f - side * walkAngle * 0.6f), glm::vec3(1, 0, 0));
        shoulder = glm::rotate(shoulder, glm::radians(side * std::fabs(diveAngle) * 1.1f), glm::vec3(0, 0, 1));
        drawCube(modelLoc, VAO, shoulder, glm::vec3(0, -0.15f, 0),
            glm::vec3(0.14f, 0.3f, 0.14f), texJersey);
        glm::mat4 elbow = glm::translate(shoulder, glm::vec3(0, -0.3f, 0));
        drawCube(modelLoc, VAO, elbow, glm::vec3(0, -0.16f, 0),
            glm::vec3(0.11f, 0.32f, 0.11f), texSkin);
        // Leg: the knee is a child of the hip joint.
        glm::mat4 hip = glm::translate(modelTemp, glm::vec3(side * 0.13f, 0.90f, 0));
        if (std::fabs(walkAngle) > 0.01f)
            hip = glm::rotate(hip, glm::radians(side * walkAngle), glm::vec3(1, 0, 0));
        else if (side == 1)
            hip = glm::rotate(hip, glm::radians(-kickAngle), glm::vec3(1, 0, 0));
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
            float z = side * (69.0f + ((i + 3) % 2) * 2.2f);
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
    if (!busAssetsReady || !arrivalStarted) return;
    float frameStep = std::max(0.0f, std::min(deltaTime, 0.1f));
    for (int team = 0; team < 2; ++team) {
        TeamBus& bus = teamBuses[team];
        if (!bus.moving) {
            if (bus.state == BUS_COMPLETED)
                teamLineupTime[team] += frameStep;
            continue;
        }
        bus.distanceTraveled = std::min(busRouteLength,
            bus.distanceTraveled + busSpeed * frameStep);
        float distance = bus.distanceTraveled;
        float sign = team == 0 ? 1.0f : -1.0f;
        float y = bus.position.y;
        if (distance <= busInitialDistance) {
            bus.state = BUS_STRAIGHT;
            bus.position = glm::vec3(sign * (102.0f - distance), y, -sign * 21.0f);
            bus.heading = -sign * busPi * 0.5f;
            bus.steeringAngle = 0.0f;
        }
        else {
            bus.state = BUS_TURNING;
            float angle = glm::clamp((distance - busInitialDistance) / busTurnRadius,
                0.0f, busPi * 0.5f);
            bus.position = glm::vec3(sign * (74.0f - busTurnRadius * sinf(angle)),
                y, -sign * (21.0f - busTurnRadius * (1.0f - cosf(angle))));
            bus.heading = (team == 0 ? -busPi * 0.5f : busPi * 0.5f) + angle;
            bus.steeringAngle = glm::radians(18.0f) * sinf(2.0f * angle);
        }
        if (distance >= busRouteLength) {
            bus.position = glm::vec3(sign * 66.0f, y, -sign * 13.0f);
            bus.heading = team == 0 ? 0.0f : busPi;
            bus.steeringAngle = 0.0f;
            bus.state = BUS_COMPLETED;
            bus.moving = false;
            teamLineupTime[team] = 0.0f;
        }
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
    if (activeCamera == 9)
        return glm::vec3(0.0f, 92.0f, 135.0f);
    if (activeCamera >= 1 && activeCamera <= 2) {
        const glm::vec3& dronePosition = broadcastCameraPositions[activeCamera - 1];
        glm::vec3 forward = trackedBallPosition - dronePosition;
        forward.y = 0.0f;
        if (glm::length(forward) < 0.0001f)
            forward = glm::vec3(1.0f, 0.0f, 0.0f);
        forward = glm::normalize(forward);
        return dronePosition + forward * 2.0f + glm::vec3(0.0f, -0.72f, 0.0f);
    }
    if (activeCamera >= 3 && activeCamera <= 8) {
        const glm::vec3& cameraPosition = broadcastCameraPositions[activeCamera - 1];
        glm::vec3 pivot = cameraPosition + cameraModelScale * cameraHeadPivot;
        glm::vec3 forward = trackedBallPosition - pivot;
        if (glm::length(forward) < 0.0001f)
            forward = glm::vec3(1.0f, 0.0f, 0.0f);
        return pivot + glm::normalize(forward) * 1.5f;
    }
    return freeCameraPosition;
}

glm::mat4 getBroadcastView()
{
    if (activeCamera == 9)
        return glm::lookAt(getBroadcastPosition(), glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));
    if (activeCamera >= 1 && activeCamera <= 8)
        return glm::lookAt(getBroadcastPosition(), trackedBallPosition,
            glm::vec3(0.0f, 1.0f, 0.0f));
    return glm::lookAt(freeCameraPosition, freeCameraPosition + camera.GetFront(),
        glm::vec3(0.0f, 1.0f, 0.0f));
}

void drawFloodlightTowers(GLint modelLoc, GLint textureModeLoc, GLuint VAO,
    const glm::mat4& scene, GLuint texMetal)
{
    glUniform1i(textureModeLoc, 5);
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
    glUniform1i(textureModeLoc, 0);
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
    glUniform1i(glGetUniformLocation(lightingShader.Program, "proceduralTextureMode"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "useImportedModelTexture"), 1);
    GLint neutralCameraLoc = glGetUniformLocation(lightingShader.Program, "cameraNeutralMaterial");
    glUniform1i(neutralCameraLoc, 1);
    for (int i = 2; i < 8; ++i) {
        if (activeCamera == i + 1) continue;
        const glm::vec3& position = broadcastCameraPositions[i];
        glm::mat4 baseMatrix = glm::translate(glm::mat4(1.0f), position);
        baseMatrix = glm::scale(baseMatrix, glm::vec3(cameraModelScale));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(baseMatrix));
        cameraBase->Draw(lightingShader);

        glm::vec3 pivotWorld = position + cameraModelScale * cameraHeadPivot;
        glm::vec3 direction = trackedBallPosition - pivotWorld;
        float horizontalDistance = glm::length(glm::vec2(direction.x, direction.z));
        if (glm::length(direction) < 0.0001f)
            direction = glm::vec3(1.0f, 0.0f, 0.0f);
        float yaw = std::atan2(-direction.z, direction.x) + glm::radians(cameraModelForwardYawDegrees);
        float pitch = std::atan2(direction.y, std::max(0.0001f, horizontalDistance));
        pitch = glm::clamp(pitch, glm::radians(-65.0f), glm::radians(65.0f));

        glm::mat4 headMatrix = glm::translate(baseMatrix, cameraHeadPivot);
        headMatrix = glm::rotate(headMatrix, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
        headMatrix = glm::rotate(headMatrix, pitch, glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(headMatrix));
        cameraHead->Draw(lightingShader);
    }
    glUniform1i(neutralCameraLoc, 0);
}


void drawBleachers(GLint modelLoc, GLint textureModeLoc, GLuint VAO, const glm::mat4& scene,
    GLuint texConcrete, GLuint texSideWall, GLuint texRed, GLuint texWhite, GLuint texWalkway, GLuint texMetal)
{
    for (int side = -1; side <= 1; side += 2) {
        drawCube(modelLoc, VAO, scene, glm::vec3(0.0f, -0.025f, side * 42.0f),
            glm::vec3(106.0f, 0.045f, 15.5f), texWalkway);
        for (int p = -9; p <= 9; ++p) {
            float x = p * 5.0f;
            drawCube(modelLoc, VAO, scene, glm::vec3(x, 0.53f, side * 49.45f),
                glm::vec3(0.10f, 1.06f, 0.10f), texMetal);
        }
        drawCube(modelLoc, VAO, scene, glm::vec3(0.0f, 1.05f, side * 49.45f),
            glm::vec3(90.0f, 0.08f, 0.08f), texMetal);
        for (int row = 0; row < 8; ++row) {
            float z = side * (52.0f + 2.0f * row);
            float height = 0.72f * (row + 1);
            glUniform1i(textureModeLoc, 5);
            drawCube(modelLoc, VAO, scene, glm::vec3(0.0f, height * 0.5f, z),
                glm::vec3(90.0f, height, 2.0f), texConcrete);
            glUniform1i(textureModeLoc, 0);
            for (int section = 0; section < 11; ++section) {
                if (section == 5) continue;
                float x = (section - 5) * 7.55f;
                GLuint seatColor = ((section + row) % 2 == 0) ? texRed : texWhite;
                drawCube(modelLoc, VAO, scene,
                    glm::vec3(x, height + 0.12f, z - side * 0.15f),
                    glm::vec3(7.15f, 0.23f, 1.15f), seatColor);
                drawCube(modelLoc, VAO, scene,
                    glm::vec3(x, height + 0.40f, z + side * 0.48f),
                    glm::vec3(7.15f, 0.60f, 0.19f), seatColor);
            }
            drawCube(modelLoc, VAO, scene,
                glm::vec3(0.0f, height + 0.025f, z),
                glm::vec3(5.9f, 0.05f, 1.85f), texWalkway);
        }
        glUniform1i(textureModeLoc, 5);
        for (int i = -1; i <= 1; i += 2) {
            drawCube(modelLoc, VAO, scene,
                glm::vec3(i * 45.3f, 3.3f, side * 59.0f),
                glm::vec3(0.20f, 6.6f, 16.0f), texSideWall);
        }
        glUniform1i(textureModeLoc, 0);
    }
}

void drawDrone(GLint modelLoc, GLuint VAO, const glm::mat4& scene,
    const BallMesh& whiteMesh, const BallMesh& blackMesh,
    glm::vec3 position, glm::vec3 target, float elapsed,
    GLuint texBody, GLuint texBlades, GLuint texMetal, GLuint texLens)
{
    glm::vec3 horizontal = target - position;
    horizontal.y = 0.0f;
    float yaw = std::atan2(-horizontal.z, horizontal.x);
    glm::mat4 drone = glm::translate(scene, position);
    drone = glm::rotate(drone, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
    drone = glm::rotate(drone, glm::radians(1.8f * sinf(elapsed * 2.0f)),
        glm::vec3(0.0f, 0.0f, 1.0f));
    drawCube(modelLoc, VAO, drone, glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.45f, 0.32f, 0.95f), texBody);
    drawCube(modelLoc, VAO, drone, glm::vec3(0.1f, 0.22f, 0.0f),
        glm::vec3(0.65f, 0.13f, 0.63f), texMetal);
    for (int sx = -1; sx <= 1; sx += 2) {
        for (int sz = -1; sz <= 1; sz += 2) {
            glm::vec3 rotorCenter(sx * 1.0f, 0.10f, sz * 1.0f);
            float armYaw = std::atan2(-static_cast<float>(sz), static_cast<float>(sx));
            glm::mat4 arm = glm::rotate(drone, armYaw, glm::vec3(0.0f, 1.0f, 0.0f));
            drawCube(modelLoc, VAO, arm, glm::vec3(0.69f, 0.04f, 0.0f),
                glm::vec3(1.46f, 0.11f, 0.13f), texMetal);
            drawSolidSphere(modelLoc, drone, rotorCenter,
                glm::vec3(0.25f, 0.13f, 0.25f), whiteMesh, blackMesh, texBody);
            glm::mat4 rotor = glm::translate(drone, rotorCenter + glm::vec3(0.0f, 0.16f, 0.0f));
            rotor = glm::rotate(rotor, (sx * sz) * elapsed * 31.0f,
                glm::vec3(0.0f, 1.0f, 0.0f));
            drawCube(modelLoc, VAO, rotor, glm::vec3(0.0f),
                glm::vec3(1.08f, 0.035f, 0.11f), texBlades);
            drawCube(modelLoc, VAO, rotor, glm::vec3(0.0f),
                glm::vec3(0.11f, 0.035f, 1.08f), texBlades);
            drawCube(modelLoc, VAO, drone, glm::vec3(sx * 0.45f, -0.32f, sz * 0.32f),
                glm::vec3(0.10f, 0.55f, 0.10f), texMetal);
        }
    }
    drawCube(modelLoc, VAO, drone, glm::vec3(0.41f, -0.30f, 0.0f),
        glm::vec3(0.50f, 0.34f, 0.40f), texBody);
    drawSolidSphere(modelLoc, drone, glm::vec3(0.68f, -0.30f, 0.0f),
        glm::vec3(0.14f, 0.14f, 0.14f), whiteMesh, blackMesh, texLens);
}

void drawTrophy(GLint modelLoc, GLuint VAO, const glm::mat4& scene,
    const BallMesh& whiteMesh, const BallMesh& blackMesh,
    float elapsed, GLuint texGold, GLuint texBase, GLuint texWhite)
{
    bool finalLift = matchScene == SCENE_FINAL;
    glm::mat4 pedestal = glm::translate(scene, finalLift ? glm::vec3(0.0f, -10.0f, 44.0f) : glm::vec3(0.0f, 0.0f, 44.0f));
    if (!finalLift) {
        drawCube(modelLoc, VAO, pedestal, glm::vec3(0.0f, 0.25f, 0.0f),
            glm::vec3(3.6f, 0.5f, 3.1f), texBase);
        drawCube(modelLoc, VAO, pedestal, glm::vec3(0.0f, 0.55f, 0.0f),
            glm::vec3(2.0f, 0.1f, 1.8f), texWhite);
    }
    glm::vec3 liftedOffset = finalLift ? glm::vec3(0.0f, 2.45f + 0.18f * sinf(elapsed * 3.0f), 38.0f) : glm::vec3(0.0f, 0.65f, 44.0f);
    glm::mat4 trophy = glm::translate(scene, liftedOffset);
    trophy = glm::rotate(trophy, glm::radians(finalLift ? 10.0f * sinf(elapsed * 1.2f) : elapsed * 26.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));
    trophy = glm::scale(trophy, glm::vec3(0.72f));
    drawCube(modelLoc, VAO, trophy, glm::vec3(0.0f, 0.12f, 0.0f),
        glm::vec3(0.95f, 0.24f, 0.95f), texGold);
    drawCube(modelLoc, VAO, trophy, glm::vec3(0.0f, 0.44f, 0.0f),
        glm::vec3(0.22f, 0.55f, 0.22f), texGold);
    drawCube(modelLoc, VAO, trophy, glm::vec3(0.0f, 0.76f, 0.0f),
        glm::vec3(0.70f, 0.13f, 0.70f), texGold);
    drawSolidSphere(modelLoc, trophy, glm::vec3(0.0f, 1.23f, 0.0f),
        glm::vec3(0.63f, 0.56f, 0.55f), whiteMesh, blackMesh, texGold);
    drawCube(modelLoc, VAO, trophy, glm::vec3(0.0f, 1.69f, 0.0f),
        glm::vec3(1.23f, 0.12f, 1.04f), texGold);
    for (int side = -1; side <= 1; side += 2) {
        drawCube(modelLoc, VAO, trophy, glm::vec3(side * 0.80f, 1.36f, 0.0f),
            glm::vec3(0.11f, 0.57f, 0.11f), texGold);
        drawCube(modelLoc, VAO, trophy, glm::vec3(side * 0.61f, 1.61f, 0.0f),
            glm::vec3(0.44f, 0.11f, 0.12f), texGold);
        drawCube(modelLoc, VAO, trophy, glm::vec3(side * 0.61f, 1.10f, 0.0f),
            glm::vec3(0.43f, 0.10f, 0.12f), texGold);
    }
}


void drawScoreDigit(GLint modelLoc, GLuint VAO, const glm::mat4& matrix,
    int digit, float x, float y, float z, float width, GLuint texture)
{
    static const int mask[10] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F };
    if (digit < 0 || digit > 9) return;
    float thickness = width * 0.12f;
    float height = width * 1.85f;
    for (int s = 0; s < 7; ++s) {
        if (!(mask[digit] & (1 << s))) continue;
        bool horizontal = (s == 0 || s == 3 || s == 6);
        float sx = x + ((s == 1 || s == 2) ? width * 0.43f :
            ((s == 4 || s == 5) ? -width * 0.43f : 0.0f));
        float sy = y + ((s == 0) ? height : (s == 6 ? height * 0.5f :
            (s == 3 ? 0.0f : ((s == 1 || s == 5) ? height * 0.75f : height * 0.25f))));
        glm::vec3 size = horizontal ? glm::vec3(width, thickness, thickness) :
            glm::vec3(thickness, height * 0.47f, thickness);
        drawCube(modelLoc, VAO, matrix, glm::vec3(sx, sy, z), size, texture);
    }
}

const char* getScoreGlyph(char letter, int row)
{
    static const char* P[] = { "11110","10001","10001","11110","10000","10000","10000" };
    static const char* U[] = { "10001","10001","10001","10001","10001","10001","01110" };
    static const char* M[] = { "10001","11011","10101","10101","10001","10001","10001" };
    static const char* A[] = { "01110","10001","10001","11111","10001","10001","10001" };
    static const char* S[] = { "01111","10000","10000","01110","00001","00001","11110" };
    static const char* T[] = { "11111","00100","00100","00100","00100","00100","00100" };
    static const char* L[] = { "10000","10000","10000","10000","10000","10000","11111" };
    static const char* N[] = { "10001","11001","10101","10011","10001","10001","10001" };
    static const char* E[] = { "11111","10000","10000","11110","10000","10000","11111" };
    static const char* I[] = { "11111","00100","00100","00100","00100","00100","11111" };
    static const char* O[] = { "01110","10001","10001","10001","10001","10001","01110" };
    static const char* D[] = { "11110","10001","10001","10001","10001","10001","11110" };
    static const char* R[] = { "11110","10001","10001","11110","10100","10010","10001" };
    const char** data = nullptr;
    switch (letter) {
    case 'P': data = P;break; case 'U':data = U;break; case 'M':data = M;break;
    case 'A':data = A;break; case 'S':data = S;break; case 'T':data = T;break;
    case 'L':data = L;break; case 'N':data = N;break; case 'E':data = E;break;
    case 'I':data = I;break; case 'O':data = O;break; case 'D':data = D;break;
    case 'R':data = R;break;
    default:return "00000";
    }
    return data[row];
}

void drawScoreWord(GLint modelLoc, GLuint VAO, const glm::mat4& matrix,
    const std::string& word, float x, float y, float z, float dot, GLuint texture)
{
    for (size_t k = 0; k < word.length(); ++k) {
        for (int row = 0; row < 7; ++row) {
            const char* pattern = getScoreGlyph(word[k], row);
            for (int col = 0; col < 5; ++col) {
                if (pattern[col] != '1') continue;
                drawCube(modelLoc, VAO, matrix,
                    glm::vec3(x + (k * 6 + col) * dot, y - row * dot, z),
                    glm::vec3(dot * 0.85f, dot * 0.85f, 0.045f), texture);
            }
        }
    }
}

void drawMatchScoreboard(GLint modelLoc, GLuint VAO, const glm::mat4& scene,
    GLuint dark, GLuint white, GLuint gold, GLuint red)
{
    glm::mat4 board = glm::translate(scene, glm::vec3(0.0f, 0.0f, -70.0f));
    drawCube(modelLoc, VAO, board, glm::vec3(-7.5f, 5.8f, 0),
        glm::vec3(0.45f, 11.6f, 0.45f), dark);
    drawCube(modelLoc, VAO, board, glm::vec3(7.5f, 5.8f, 0),
        glm::vec3(0.45f, 11.6f, 0.45f), dark);
    drawCube(modelLoc, VAO, board, glm::vec3(0.0f, 11.8f, 0),
        glm::vec3(18.0f, 7.0f, 0.55f), dark);
    drawScoreWord(modelLoc, VAO, board, "PUMAS", -7.6f, 14.2f, 0.35f, 0.15f, gold);
    drawScoreWord(modelLoc, VAO, board, "ATLANTE", 1.4f, 14.2f, 0.35f, 0.15f, white);
    drawScoreDigit(modelLoc, VAO, board, std::min(9, pumasScore), -5.7f, 10.9f, 0.40f, 1.15f, white);
    drawScoreDigit(modelLoc, VAO, board, std::min(9, atlanteScore), 5.5f, 10.9f, 0.40f, 1.15f, white);
    drawCube(modelLoc, VAO, board, glm::vec3(-0.2f, 12.1f, 0.42f),
        glm::vec3(0.25f, 0.25f, 0.10f), red);
    drawCube(modelLoc, VAO, board, glm::vec3(-0.2f, 11.3f, 0.42f),
        glm::vec3(0.25f, 0.25f, 0.10f), red);
    drawScoreDigit(modelLoc, VAO, board, (matchMinute / 10) % 10, -2.2f, 9.6f, 0.39f, 0.42f, gold);
    drawScoreDigit(modelLoc, VAO, board, matchMinute % 10, -1.55f, 9.6f, 0.39f, 0.42f, gold);
    drawCube(modelLoc, VAO, board, glm::vec3(-0.80f, 10.0f, 0.39f),
        glm::vec3(0.10f, 0.10f, 0.08f), gold);
    drawCube(modelLoc, VAO, board, glm::vec3(-0.80f, 10.35f, 0.39f),
        glm::vec3(0.10f, 0.10f, 0.08f), gold);
    drawScoreDigit(modelLoc, VAO, board, (matchSecond / 10) % 10, 0.04f, 9.6f, 0.39f, 0.42f, gold);
    drawScoreDigit(modelLoc, VAO, board, matchSecond % 10, 0.68f, 9.6f, 0.39f, 0.42f, gold);
    if (matchScene == SCENE_PENALTIES || matchScene == SCENE_FINAL) {
        drawScoreWord(modelLoc, VAO, board, "PEN", -7.8f, 9.7f, 0.39f, 0.13f, red);
        drawScoreDigit(modelLoc, VAO, board, std::min(9, penaltyGoals[0]), -4.45f, 9.15f, 0.4f, 0.42f, white);
        drawScoreDigit(modelLoc, VAO, board, std::min(9, penaltyGoals[1]), 4.45f, 9.15f, 0.4f, 0.42f, white);
    }
}

GLuint createFadeProgram()
{
    const char* vs = "#version 330 core\nlayout(location=0) in vec2 p;\nvoid main(){gl_Position=vec4(p,0.0,1.0);}\n";
    const char* fs = "#version 330 core\nout vec4 c;\nuniform float fadeAlpha;\nvoid main(){c=vec4(0.0,0.0,0.0,fadeAlpha);}\n";
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertex, 1, &vs, nullptr);
    glShaderSource(fragment, 1, &fs, nullptr);
    glCompileShader(vertex);
    glCompileShader(fragment);
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return program;
}

void drawFadeOverlay(GLuint program, GLuint vao)
{
    if (fadeAlpha <= 0.0f) return;
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(program);
    glUniform1f(glGetUniformLocation(program, "fadeAlpha"), fadeAlpha);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}
