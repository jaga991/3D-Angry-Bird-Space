#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "irrKlang.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "glad.c"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include "shader_s.h"
#include "camera.h"

#include "Cube.h"
#include <vector>

#include "../collision/sat.h"
#include "../level/level.h"

#include <set>
#include <tuple>

#include <map>
#include <string>
using namespace irrklang;

ISoundEngine* SoundEngine = createIrrKlangDevice();

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void setShaderAndTexture();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool wasRightMouseButtonPressed = false;


bool rerenderText = true;

//physics constants
float gravity = -0.0f; // Acceleration due to gravity - change this to suit your needs
float friction = 0.5f; // Coefficient of friction - change this to suit your needs
float airResistance = 0.1f; // Coefficient of air resistance - change this to suit your needs

int frameCounter = 0;
double startSecond = glfwGetTime();

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//list of cubes
std::vector<Cube*> cubeList;

//frame rate
const float desiredFrameTime = 1.0f / 200.0; // 60 FPS

//game variables
int shotFired = 0;
int score = 0;


struct pair_comparator {
    bool operator() (const std::pair<Cube*, Cube*>& a, const std::pair<Cube*, Cube*>& b) const {
        // Order the pairs so that the smaller pointer is always first
        Cube* a1 = a.first < a.second ? a.first : a.second;
        Cube* a2 = a.first < a.second ? a.second : a.first;
        Cube* b1 = b.first < b.second ? b.first : b.second;
        Cube* b2 = b.first < b.second ? b.second : b.first;

        return std::tie(a1, a2) < std::tie(b1, b2);
    }
};



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

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    SoundEngine->play2D("audio/bgm.mp3", true);


    cubeList = loadLevel(3, score);

    std::set<std::pair<Cube*, Cube*>, pair_comparator> collidingCubes;


    std::cout << "score is: " << score << std::endl;
    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("Linking\\shader\\shader.vs", "Linking\\shader\\shader.fs");
    ourShader.use();

    // load and create textures
    // -------------------------
    unsigned int texture1, texture2, texture3, texture4, texture5;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load("Linking\\texture\\stone.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture 1" << std::endl;
    }
    stbi_image_free(data);
    // texture 2
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("Linking\\texture\\bird.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture 2" << std::endl;
    }
    stbi_image_free(data);

    // texture 3 (text3 saved for background)
    // ---------
    glGenTextures(1, &texture3);
    glBindTexture(GL_TEXTURE_2D, texture3);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("Linking\\texture\\background.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture 3" << std::endl;
    }
    stbi_image_free(data);

    //texture 4
    glGenTextures(1, &texture4);
    glBindTexture(GL_TEXTURE_2D, texture4);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // set texture filtering parameters
    // load image, create texture and generate mipmaps
    data = stbi_load("Linking\\texture\\pig.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture 4" << std::endl;
    }
    stbi_image_free(data);
    //texture 5
    glGenTextures(1, &texture5);
    glBindTexture(GL_TEXTURE_2D, texture5);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // set texture filtering parameters
    // load image, create texture and generate mipmaps
    data = stbi_load("Linking\\texture\\wood.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture 4" << std::endl;
    }
    stbi_image_free(data);
    // load image, create texture and generate mipmaps

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture3);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, texture4);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, texture5);
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);
    ourShader.setInt("texture3", 2);
    ourShader.setInt("texture4", 3);
    ourShader.setInt("texture5", 4);

    // -----------

    while (!glfwWindowShouldClose(window))
    {
        //for camera movement
        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        // -----
        processInput(window);

        //frame rate
        double frameStart = glfwGetTime();
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // bind textures on corresponding texture units
                // activate shader



        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);
        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        ourShader.setMat4("model", model);
        

        std::pair<bool, std::pair<glm::vec3, float>> result;



        for (int i = 0; i < cubeList.size(); i++)
        {

            ourShader.setInt("cubeType", cubeList[i]->GetType());
            cubeList[i]->Draw(ourShader);
            if (isnan(cubeList[i]->GetPosition().x) || isnan(cubeList[i]->GetPosition().y) || isnan(cubeList[i]->GetPosition().z)) {
                cubeList[i]->SetPosition(cubeList[i]->GetPrevPos());
            }

            if (glm::length(cubeList[i]->GetPosition()) > 10000) {
                cubeList[i]->SetPosition(cubeList[i]->GetPrevPos());

            }

            //apply air resistance
            cubeList[i]->SetVelocity(cubeList[i]->GetVelocity() * (1.0f - airResistance * deltaTime));
            cubeList[i]->SetAngularVelocity(cubeList[i]->GetAngularVelocity() * (1.0f - airResistance * deltaTime));

            // Update the position and rotation of the cube
            cubeList[i]->SetPosition(cubeList[i]->GetPosition() + cubeList[i]->GetVelocity() * deltaTime);
            cubeList[i]->SetRotation(cubeList[i]->GetRotation() + cubeList[i]->GetAngularVelocity() * deltaTime);
            
            //check if pig destroyed
            if (cubeList[i]->GetType() == 4) {
                if (glm::length(cubeList[i]->GetVelocity()) > 0.2f) {
                    SoundEngine->play2D("audio/pigDeath.mp3", false);
                   cubeList[i]->SetPosition(glm::vec3(200.0f, 200.0f, 200.0f));
                   cubeList[i]->SetVelocity(glm::vec3(0, 0, 0));
                   score--;
                   std::cout << "score is: " << score << std::endl;
                       if (score == 0) {
                       SoundEngine->play2D("audio/levelComplete.mp3", false);
				   }
				}
            }

        }
        




        //check each pair of cube for collision
        for (int i = 0; i < cubeList.size(); i++) {
            if (cubeList[i]->GetType() == 3)
            {
                continue;
            }
            for (int j = i + 1; j < cubeList.size(); j++)
            {
                if (cubeList[j]->GetType() == 3)
                {
                    continue;
                }
                result = areCubesColliding(*cubeList[i], *cubeList[j]);
                if (result.first)
                {
                    //get minimum translation vector and magnitude
                    glm::vec3 mtv = result.second.first;
                    float mtvMagnitude = result.second.second;

                    //face normals of cube1
                    std::vector<glm::vec3> normals1 = cubeList[i]->GetFaceNormals();
                    //face normals of cube2
                    std::vector<glm::vec3> normals2 = cubeList[j]->GetFaceNormals();

                    //check collision type
                    int collisionType = detectCollisionType(*cubeList[i], *cubeList[j], mtv, mtvMagnitude);
                    //print collision type

                    //get contact point
                    glm::vec3 contactPoint = getContactPoint(*cubeList[i], *cubeList[j], collisionType, mtv, mtvMagnitude);

                    // Collision resolution 1

                    float totalMass = cubeList[i]->GetMass() + cubeList[j]->GetMass();
                    cubeList[i]->SetPosition(cubeList[i]->GetPosition() + mtv * (mtvMagnitude * (cubeList[j]->GetMass() / totalMass)));
                    cubeList[j]->SetPosition(cubeList[j]->GetPosition() - mtv * (mtvMagnitude * (cubeList[i]->GetMass() / totalMass)));

                    // Impulse resolution
                    glm::vec3 relativeVelocity = cubeList[i]->GetVelocity() - cubeList[j]->GetVelocity();
                    float impulseMagnitude = -(1 + ((cubeList[i]->GetRestitution() + cubeList[j]->GetRestitution()) / 2)) * glm::dot(relativeVelocity, mtv) / (1 / cubeList[i]->GetMass() + 1 / cubeList[j]->GetMass());
                    glm::vec3 impulse = impulseMagnitude * mtv * 0.95f;

                    cubeList[i]->SetPrevPos(cubeList[i]->GetPosition());
                    cubeList[j]->SetPrevPos(cubeList[j]->GetPosition());
                    cubeList[i]->SetVelocity(cubeList[i]->GetVelocity() + impulse / cubeList[i]->GetMass());
                    cubeList[j]->SetVelocity(cubeList[j]->GetVelocity() - impulse / cubeList[j]->GetMass());

                    /* Torque and angular velocity adjustment*/

                    glm::vec3 r1 = contactPoint - cubeList[i]->GetPosition();
                    glm::vec3 r2 = contactPoint - cubeList[j]->GetPosition();


                    glm::vec3 angularImpulse = glm::cross(r1, impulse) / cubeList[i]->GetInertia() + glm::cross(r2, -impulse) / cubeList[j]->GetInertia();

                    cubeList[i]->SetAngularVelocity(cubeList[i]->GetAngularVelocity() + angularImpulse);
                    cubeList[j]->SetAngularVelocity(cubeList[j]->GetAngularVelocity() - angularImpulse);// Calculate the vectors from the centers of mass to the point of contact


                }
            }
                   
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // 
    // ------------------------------------------------------------------
    for (int i = 0; i < cubeList.size(); i++)
    {   
        cubeList[i]->deleteBuffers();
        delete cubeList[i];
    }
    cubeList.clear();

    glfwTerminate();
    return 0;
}




























// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    bool isRightMouseButtonPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

    if (!wasRightMouseButtonPressed && isRightMouseButtonPressed)
    {
        // The right mouse button was just clicked
        Cube* newCube = new Cube();
        newCube->SetType(2);
        newCube->SetPosition(camera.Position);
        newCube->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
        // Set the cube's velocity to make it move in the direction the camera is facing
        float initialSpeed = 15.0f; // Change this to the speed you want
        newCube->SetVelocity(initialSpeed * camera.Front);
        newCube->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
        newCube->SetAngularVelocity(glm::vec3(60.0f, 61.0f, 0.0f)); // Change this to the angular velocity you want
        cubeList.push_back(newCube);
        SoundEngine->play2D("audio/fire.mp3", false);
        //increase shot fired count
        shotFired++;
    }

    wasRightMouseButtonPressed = isRightMouseButtonPressed;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
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
    else
    {
        firstMouse = true;
    }


}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));

}

void setShaderAndTexture()
{
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
