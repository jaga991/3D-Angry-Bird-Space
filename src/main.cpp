#define NOMINMAX
#include <windows.h>

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


//store a list of cubes
std::vector<Cube> cubes;






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
    


    Cube cube1;
    cube1.SetPosition(glm::vec3(2.0f, 0.0f,0.9f));
    cube1.SetAngularVelocity(glm::vec3(0.1f,0.1f,0.1f));
    cube1.SetVelocity(glm::vec3(-0.0005f, 0.0f,0.0f));
 
    cubes.push_back(cube1);

    Cube cube2;
    cube2.SetPosition(glm::vec3(0.0f, 0.0, 0.0f));

    cubes.push_back(cube2);

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
        // glDrawArrays(GL_TRIANGLES, 0, 6);


        for (int i = 0; i < cubes.size(); i++)
        {
            if (i == 0) {
                std::cout<<glm::to_string(cubes[i].GetAngularVelocity())<<std::endl;
            }
            cubes[i].SetPosition(cubes[i].GetPosition() + cubes[i].GetVelocity());
            //print angular velocity

            cubes[i].SetRotation(cubes[i].GetRotation() + cubes[i].GetAngularVelocity());

            // Draw the cube
            cubes[i].Draw(ourShader);


        }
        // Check for collisions between all pairs of cubes
        for (int i = 0; i < cubes.size(); i++) {
            for (int j = i + 1; j < cubes.size(); j++) {
                std::pair<bool, std::pair<glm::vec3, float>> result = areCubesColliding(cubes[i], cubes[j]);
                //if SAT collision returns true
                //resolve the collision immediately
                //put the 2 cubes through a collision point checker, get all points of collision into a list
                // run the list through a collision resolution function
                // when list is empty, end the collision resolution
                // continue with the next pair of collision

                if (result.first) {
                    glm::vec3 overlapAxis = result.second.first;
                    float overlap = result.second.second;

                    // Project the cubes onto the overlap axis
                    float minProj1, maxProj1, minProj2, maxProj2;
                    projectCubeOntoAxis(cubes[i], overlapAxis, minProj1, maxProj1);
                    projectCubeOntoAxis(cubes[j], overlapAxis, minProj2, maxProj2);

                    // Calculate the minimum and maximum overlap values
                    float minOverlap = std::max(minProj1, minProj2);
                    float maxOverlap = std::min(maxProj1, maxProj2);

                    std::vector<glm::vec3> collidingVertices1 = getCollidingVertices(cubes[i], overlapAxis, minOverlap, maxOverlap);
                    std::vector<glm::vec3> collidingVertices2 = getCollidingVertices(cubes[j], overlapAxis, minOverlap, maxOverlap);


                    //get the average collision point
                    glm::vec3 collidePointAvg;
                    for (glm::vec3& vertex : collidingVertices1) {
                        std::cout<< glm::to_string(vertex)<<std::endl;
                        collidePointAvg += vertex;
                    }
                    for (glm::vec3& vertex : collidingVertices2) {
                        std::cout<< glm::to_string(vertex)<<std::endl;
                        collidePointAvg += vertex;
                    }
                    collidePointAvg = collidePointAvg / (float)(collidingVertices1.size() + collidingVertices2.size());
                    
                    // get the average of the two vertices
                    




                    
                    // Collision resolution
                    
                     // Collision response
                    float totalMass = cubes[i].GetMass() + cubes[j].GetMass();
                    cubes[i].SetPosition(cubes[i].GetPosition() + overlapAxis * (overlap * (cubes[j].GetMass() / totalMass)));
                    cubes[j].SetPosition(cubes[j].GetPosition() - overlapAxis * (overlap * (cubes[i].GetMass() / totalMass)));

                    // Impulse resolution
                    float restitution = 0.5f; // Coefficient of restitution - change this to suit your needs
                    glm::vec3 relativeVelocity = cubes[i].GetVelocity() - cubes[j].GetVelocity();
                    float impulseMagnitude = -(1 + restitution) * glm::dot(relativeVelocity, overlapAxis) / (1 / cubes[i].GetMass() + 1 / cubes[j].GetMass());
                    glm::vec3 impulse = impulseMagnitude * overlapAxis;


                    cubes[i].SetVelocity(cubes[i].GetVelocity() + impulse / cubes[i].GetMass());
                    cubes[j].SetVelocity(cubes[j].GetVelocity() - impulse / cubes[j].GetMass());

                    // Torque and angular velocity adjustment

                    glm::vec3 r1 = collidePointAvg - cubes[i].GetPosition(); 
                    glm::vec3 r2 = collidePointAvg - cubes[j].GetPosition(); 

                    glm::vec3 torque1 = glm::cross(r1, impulse);
                    glm::vec3 torque2 = glm::cross(r2, -impulse);

                    cubes[i].SetAngularVelocity(cubes[i].GetAngularVelocity() + torque1);
                    cubes[j].SetAngularVelocity(cubes[j].GetAngularVelocity() + torque2);

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
    // ------------------------------------------------------------------
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
        Cube newCube;
        newCube.SetPosition(camera.Position);
        cubes.push_back(newCube);
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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}