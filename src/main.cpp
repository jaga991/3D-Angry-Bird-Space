#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

#include <set>
#include <tuple>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool wasRightMouseButtonPressed = false;


//physics constants
float gravity = -0.0f; // Acceleration due to gravity - change this to suit your needs
float friction = 0.5f; // Coefficient of friction - change this to suit your needs
float airResistance = 0.1f; // Coefficient of air resistance - change this to suit your needs

int frameCounter = 0;
double startSecond = glfwGetTime();


float floorVertices[] = {
    // positions          // texture Coords 
     5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
    -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

     5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
     5.0f, -0.5f, -5.0f,  2.0f, 2.0f
};

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


std::vector<Cube*> cubeList;

//frame rate
const float desiredFrameTime = 1.0f / 200.0; // 60 FPS

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

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("Linking\\shader\\shader.vs", "Linking\\shader\\shader.fs");

    unsigned int floorVAO, floorVBO;
    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);
    glBindVertexArray(floorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // load and create a texture
    // -------------------------






    for (int i = 0; i < 4; i++) {

        float x = 0.0f + 1.0f * i;
        Cube* cube = new Cube();
        cube->SetPosition(glm::vec3(0.0f, x, 0.0f));
        cube->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
        cube->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
        cube->SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
        cubeList.push_back(cube);
    }

    unsigned int texture1, texture2;
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
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char* data = stbi_load("Linking\\texture\\wall.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("Linking\\texture\\awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
    // or set it via the texture class
    ourShader.setInt("texture2", 1);

    std::set<std::pair<Cube*, Cube*>, pair_comparator> collidingCubes;    // render loop
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
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // activate shader
        ourShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);
        glBindVertexArray(floorVAO);
        glm::mat4 model = glm::mat4(1.0f);
        ourShader.setMat4("model", model);

        //draw floor
        //glDrawArrays(GL_TRIANGLES, 0, 6);

        std::pair<bool, std::pair<glm::vec3, float>> result;

        for (int i = 0; i < cubeList.size(); i++)
        {
            //apply gravity
            cubeList[i]->SetVelocity(cubeList[i]->GetVelocity() + glm::vec3(0.0f, gravity, 0.0f) * deltaTime);

            ////apply air resistance
            //cubeList[i]->SetVelocity(cubeList[i]->GetVelocity() * (1.0f - airResistance * deltaTime));
            //cubeList[i]->SetAngularVelocity(cubeList[i]->GetAngularVelocity() * (1.0f - airResistance * deltaTime));

            // Update the position and rotation of the cube
            cubeList[i]->SetPosition(cubeList[i]->GetPosition() + cubeList[i]->GetVelocity() * deltaTime);
            cubeList[i]->SetRotation(cubeList[i]->GetRotation() + cubeList[i]->GetAngularVelocity() * deltaTime);

            // Draw the cube
            cubeList[i]->Draw(ourShader);
        }

        //floor collision checker
        for (Cube* cube : cubeList) {

            float floorY = -0.5f; // The y-coordinate of the floor

            if (cube->GetPosition().y - 0.5f < floorY) { // If the cube is below the floor

                //apply friction
                //cube->SetVelocity(cube->GetVelocity() * (1.0f - friction * deltaTime));
                //cube->SetAngularVelocity(cube->GetAngularVelocity() * (1.0f - friction * deltaTime));


                glm::vec3 pos = cube->GetPosition();
                pos.y = floorY + 0.5f; // Move the cube to the floor level
                cube->SetPosition(pos);

                glm::vec3 vel = cube->GetVelocity();
                vel.y = -vel.y * cube->GetRestitution(); // Reverse the y-velocity and apply restitution
                cube->SetVelocity(vel);
            }
        }

        //check each pair of cube for collision
        for (int i = 0; i < cubeList.size(); i++) {

            for (int j = i + 1; j < cubeList.size(); j++)
            {
                //if distance of cube center is more than 3, skip the collision check
                if (glm::distance(cubeList[i]->GetPosition(), cubeList[j]->GetPosition()) > 3.0f) {
                    continue;
                }

                result = areCubesColliding(*cubeList[i], *cubeList[j]);
                if (result.first)
                {
                    collidingCubes.insert(std::make_pair(cubeList[i], cubeList[j]));


                }


                for (const auto& pair : collidingCubes) {
                    Cube* cube1 = pair.first;
                    Cube* cube2 = pair.second;
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
                    float impulseMagnitude = -(1 + ((cubeList[i]->GetRestitution() + cubeList[j]->GetRestitution())/2)) * glm::dot(relativeVelocity, mtv) / (1 / cubeList[i]->GetMass() + 1 / cubeList[j]->GetMass());
                    glm::vec3 impulse = impulseMagnitude * mtv;


                    cubeList[i]->SetVelocity(cubeList[i]->GetVelocity() + impulse / cubeList[i]->GetMass());
                    cubeList[j]->SetVelocity(cubeList[j]->GetVelocity() - impulse / cubeList[j]->GetMass());

                    // Torque and angular velocity adjustment

                    glm::vec3 r1 = contactPoint - cubeList[i]->GetPosition();
                    glm::vec3 r2 = contactPoint - cubeList[j]->GetPosition();


                    glm::vec3 angularImpulse = glm::cross(r1, impulse) / cubeList[i]->GetInertia() + glm::cross(r2, -impulse) / cubeList[j]->GetInertia();

                    cubeList[i]->SetAngularVelocity(cubeList[i]->GetAngularVelocity() + angularImpulse);
                    cubeList[j]->SetAngularVelocity(cubeList[j]->GetAngularVelocity() - angularImpulse);
                }
            }
        }
        collidingCubes.clear();
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
        delete cubeList[i];
    }
    cubeList.clear();

    glfwTerminate();
    return 0;
}

    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
    void processInput(GLFWwindow * window)
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

            newCube->SetPosition(camera.Position);

            // Set the cube's velocity to make it move in the direction the camera is facing
            float initialSpeed = 5.0f; // Change this to the speed you want
            newCube->SetVelocity(initialSpeed * camera.Front);
            newCube->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
            newCube->SetAngularVelocity(glm::vec3(60.0f, 61.0f, 0.0f)); // Change this to the angular velocity you want
            cubeList.push_back(newCube);
        }

        wasRightMouseButtonPressed = isRightMouseButtonPressed;
    }

    void mouse_callback(GLFWwindow * window, double xposIn, double yposIn)
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
    void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
    {
        camera.ProcessMouseScroll(static_cast<float>(yoffset));

    }

    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // ---------------------------------------------------------------------------------------------
    void framebuffer_size_callback(GLFWwindow * window, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and 
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }