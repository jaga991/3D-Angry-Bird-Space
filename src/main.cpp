// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "irrKlang.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "glad.c"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "camera.h"
#include "shader_s.h"
#include <iostream>

#include "Cube.h"
#include <vector>

#include "../collision/sat.h"
#include "../level/level.h"

#include <set>
#include <tuple>

#include <map>
#include <string>
using namespace irrklang;

ISoundEngine *SoundEngine = createIrrKlangDevice();

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

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

// physics constants
float gravity =
    -0.0f; // Acceleration due to gravity - change this to suit your needs
float friction =
    0.5f; // Coefficient of friction - change this to suit your needs
float airResistance =
    0.1f; // Coefficient of air resistance - change this to suit your needs

int frameCounter = 0;
double startSecond = glfwGetTime();

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

// list of cubes
std::vector<Cube *> cubeList;

// frame rate
const float desiredFrameTime = 1.0f / 200.0; // 60 FPS

// game variables
int shotFired = 0;
int score = 0;

struct pair_comparator {
  bool operator()(const std::pair<Cube *, Cube *> &a,
                  const std::pair<Cube *, Cube *> &b) const {
    // Order the pairs so that the smaller pointer is always first
    Cube *a1 = a.first < a.second ? a.first : a.second;
    Cube *a2 = a.first < a.second ? a.second : a.first;
    Cube *b1 = b.first < b.second ? b.first : b.second;
    Cube *b2 = b.first < b.second ? b.second : b.first;

    return std::tie(a1, a2) < std::tie(b1, b2);
  }
};

int main() {
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
  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
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
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  glEnable(GL_DEPTH_TEST);

  SoundEngine->play2D("audio/bgm.mp3", true);

  cubeList = loadLevel(3, score);

  std::set<std::pair<Cube *, Cube *>, pair_comparator> collidingCubes;

  std::cout << "score is: " << score << std::endl;
  // build and compile our shader zprogram
  // ------------------------------------
  Shader ourShader("Linking\\shader\\shader.vs", "Linking\\shader\\shader.fs");
  ourShader.use();

  // load and create textures
  // -------------------------
  unsigned int texture1, texture3, texture4, texture5;
  unsigned int bird1, bird2, bird3, bird4;
  // texture 1
  // ---------
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  // set the texture wrapping parameters
  glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
      GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(
      true); // tell stb_image.h to flip loaded texture's on the y-axis.
  unsigned char *data =
      stbi_load("Linking\\texture\\stone.png", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture 1" << std::endl;
  }
  stbi_image_free(data);

  // texture 3 (text3 saved for background)
  // ---------
  glGenTextures(1, &texture3);
  glBindTexture(GL_TEXTURE_2D, texture3);
  // set the texture wrapping parameters
  glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
      GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  data = stbi_load("Linking\\texture\\background.png", &width, &height,
                   &nrChannels, 0);
  if (data) {
    // note that the awesomeface.png has transparency and thus an alpha channel,
    // so make sure to tell OpenGL the data type is of GL_RGBA
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture 3" << std::endl;
  }
  stbi_image_free(data);

  // texture 4
  glGenTextures(1, &texture4);
  glBindTexture(GL_TEXTURE_2D, texture4);
  // set the texture wrapping parameters
  glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
      GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // set texture filtering parameters
  // load image, create texture and generate mipmaps
  data =
      stbi_load("Linking\\texture\\pig.png", &width, &height, &nrChannels, 0);
  if (data) {
    // note that the awesomeface.png has transparency and thus an alpha channel,
    // so make sure to tell OpenGL the data type is of GL_RGBA
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture 4" << std::endl;
  }
  stbi_image_free(data);
  // texture 5
  glGenTextures(1, &texture5);
  glBindTexture(GL_TEXTURE_2D, texture5);
  // set the texture wrapping parameters
  glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
      GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // set texture filtering parameters
  // load image, create texture and generate mipmaps
  data =
      stbi_load("Linking\\texture\\wood.png", &width, &height, &nrChannels, 0);
  if (data) {
    // note that the awesomeface.png has transparency and thus an alpha channel,
    // so make sure to tell OpenGL the data type is of GL_RGBA
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture 4" << std::endl;
  }
  stbi_image_free(data);

  // Birds texture

  auto bind_bird_textures = [&](unsigned int *texture_id_ptr,
                                const char *texture_file) {
    // texture 2
    // ---------
    glGenTextures(1, texture_id_ptr);
    glBindTexture(GL_TEXTURE_2D, *texture_id_ptr);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_REPEAT); // set texture wrapping to GL_REPEAT (default
                                // wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load(texture_file, &width, &height, &nrChannels, 0);
    if (data) {
      // note that the awesomeface.png has transparency and thus an alpha
      // channel, so make sure to tell OpenGL the data type is of GL_RGBA
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      std::cout << "Failed to load " << texture_file << std::endl;
    }
    stbi_image_free(data);
  };
  bind_bird_textures(&bird1, "Linking\\texture\\red_bird.png");
  bind_bird_textures(&bird2, "Linking\\texture\\yellow_bird.png");
  bind_bird_textures(&bird3, "Linking\\texture\\blue_bird.png");
  bind_bird_textures(&bird4, "Linking\\texture\\black_bird.png");

  // load image, create texture and generate mipmaps
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, texture3);
  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_2D, texture4);
  glActiveTexture(GL_TEXTURE5);
  glBindTexture(GL_TEXTURE_2D, texture5);

  glActiveTexture(GL_TEXTURE11);
  glBindTexture(GL_TEXTURE_2D, bird1);
  glActiveTexture(GL_TEXTURE12);
  glBindTexture(GL_TEXTURE_2D, bird2);
  glActiveTexture(GL_TEXTURE13);
  glBindTexture(GL_TEXTURE_2D, bird3);
  glActiveTexture(GL_TEXTURE14);
  glBindTexture(GL_TEXTURE_2D, bird4);

  ourShader.setInt("texture1", 1);
  ourShader.setInt("texture3", 3);
  ourShader.setInt("texture4", 4);
  ourShader.setInt("texture5", 5);
  ourShader.setInt("bird1", 11);
  ourShader.setInt("bird2", 12);
  ourShader.setInt("bird3", 13);
  ourShader.setInt("bird4", 14);

  // -----------

  while (!glfwWindowShouldClose(window)) {
    // for camera movement
    double currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    // input
    // -----
    processInput(window);

    // frame rate
    double frameStart = glfwGetTime();
    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // bind textures on corresponding texture units
    // activate shader

    // pass projection matrix to shader (note that in this case it could change
    // every frame)
    glm::mat4 projection =
        glm::perspective(glm::radians(camera.Zoom),
                         (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);
    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    ourShader.setMat4("view", view);
    glm::mat4 model = glm::mat4(1.0f);
    ourShader.setMat4("model", model);

    std::pair<bool, std::pair<glm::vec3, float>> result;

    for (auto curr_cube : cubeList) {

      ourShader.setInt("cubeType", curr_cube->GetType());
      curr_cube->Draw(ourShader);
      if (isnan(curr_cube->GetPosition().x) ||
          isnan(curr_cube->GetPosition().y) ||
          isnan(curr_cube->GetPosition().z)) {
        curr_cube->SetPosition(curr_cube->GetPrevPos());
      }

      if (glm::length(curr_cube->GetPosition()) > 10000) {
        curr_cube->SetPosition(curr_cube->GetPrevPos());
      }

      // apply air resistance
      curr_cube->SetVelocity(curr_cube->GetVelocity() *
                             (1.0f - airResistance * deltaTime));
      curr_cube->SetAngularVelocity(curr_cube->GetAngularVelocity() *
                                    (1.0f - airResistance * deltaTime));

      // Update the position and rotation of the cube
      curr_cube->SetPosition(curr_cube->GetPosition() +
                             curr_cube->GetVelocity() * deltaTime);
      curr_cube->SetRotation(curr_cube->GetRotation() +
                             curr_cube->GetAngularVelocity() * deltaTime);

      // check if pig destroyed
      if (curr_cube->GetType() == 4) {
        if (glm::length(curr_cube->GetVelocity()) > 0.2f) {
          SoundEngine->play2D("audio/pigDeath.mp3", false);
          curr_cube->SetPosition(glm::vec3(200.0f, 200.0f, 200.0f));
          curr_cube->SetVelocity(glm::vec3(0, 0, 0));
          score--;
          std::cout << "score is: " << score << std::endl;
          if (score == 0) {
            SoundEngine->play2D("audio/levelComplete.mp3", false);
          }
        }
      }
    }

    // check each pair of cube for collision
    for (int i = 0; i < cubeList.size(); i++) {
      if (cubeList[i]->GetType() == 3) {
        continue;
      }
      for (int j = i + 1; j < cubeList.size(); j++) {
        if (cubeList[j]->GetType() == 3) {
          continue;
        }
        result = areCubesColliding(*cubeList[i], *cubeList[j]);
        if (result.first) {
          // get minimum translation vector and magnitude
          glm::vec3 mtv = result.second.first;
          float mtvMagnitude = result.second.second;

          // face normals of cube1
          std::vector<glm::vec3> normals1 = cubeList[i]->GetFaceNormals();
          // face normals of cube2
          std::vector<glm::vec3> normals2 = cubeList[j]->GetFaceNormals();

          // check collision type
          int collisionType = detectCollisionType(*cubeList[i], *cubeList[j],
                                                  mtv, mtvMagnitude);
          // print collision type

          // get contact point
          glm::vec3 contactPoint = getContactPoint(
              *cubeList[i], *cubeList[j], collisionType, mtv, mtvMagnitude);

          // Collision resolution 1

          float totalMass = cubeList[i]->GetMass() + cubeList[j]->GetMass();
          cubeList[i]->SetPosition(
              cubeList[i]->GetPosition() +
              mtv * (mtvMagnitude * (cubeList[j]->GetMass() / totalMass)));
          cubeList[j]->SetPosition(
              cubeList[j]->GetPosition() -
              mtv * (mtvMagnitude * (cubeList[i]->GetMass() / totalMass)));

          // Impulse resolution
          glm::vec3 relativeVelocity =
              cubeList[i]->GetVelocity() - cubeList[j]->GetVelocity();
          float impulseMagnitude =
              -(1 + ((cubeList[i]->GetRestitution() +
                      cubeList[j]->GetRestitution()) /
                     2)) *
              glm::dot(relativeVelocity, mtv) /
              (1 / cubeList[i]->GetMass() + 1 / cubeList[j]->GetMass());
          glm::vec3 impulse = impulseMagnitude * mtv * 0.95f;

          cubeList[i]->SetPrevPos(cubeList[i]->GetPosition());
          cubeList[j]->SetPrevPos(cubeList[j]->GetPosition());
          cubeList[i]->SetVelocity(cubeList[i]->GetVelocity() +
                                   impulse / cubeList[i]->GetMass());
          cubeList[j]->SetVelocity(cubeList[j]->GetVelocity() -
                                   impulse / cubeList[j]->GetMass());

          /* Torque and angular velocity adjustment*/

          glm::vec3 r1 = contactPoint - cubeList[i]->GetPosition();
          glm::vec3 r2 = contactPoint - cubeList[j]->GetPosition();

          glm::vec3 angularImpulse =
              glm::cross(r1, impulse) / cubeList[i]->GetInertia() +
              glm::cross(r2, -impulse) / cubeList[j]->GetInertia();

          cubeList[i]->SetAngularVelocity(cubeList[i]->GetAngularVelocity() +
                                          angularImpulse);
          cubeList[j]->SetAngularVelocity(
              cubeList[j]->GetAngularVelocity() -
              angularImpulse); // Calculate the vectors from the centers of mass
                               // to the point of contact
        }
      }
    }

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
    // etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  // glfw: terminate, clearing all previously allocated GLFW resources.
  //
  // ------------------------------------------------------------------
  for (int i = 0; i < cubeList.size(); i++) {
    cubeList[i]->deleteBuffers();
    delete cubeList[i];
  }
  cubeList.clear();

  glfwTerminate();
  return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
  // Ugly code... but lazy make a new class
  static int next_bird_index = 0;
  static bool can_use_ability = false;
  static Cube *last_bird_ptr = NULL;
  static const std::vector<glm::vec3> bird_colors = {
      glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.8f, 0.8f, 0.0f),
      glm::vec3(0.4f, 0.6f, 0.8f), glm::vec3(0.6f, 0.6f, 0.6f)};
  static const float blue_side = 0.8;
  static const float blue_velocity_proportion = 0.8;

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

  bool isRightMouseButtonPressed =
      glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

  if (!wasRightMouseButtonPressed && isRightMouseButtonPressed) {

    if (can_use_ability) {
      // Do bird ability
      if (last_bird_ptr == NULL) {
        // Shouldn't happen.
        assert(false);
      }

      auto bird_index = last_bird_ptr->GetType() - BASE_BIRD_NUM;

      if (bird_index == 1) {
        // Yellow bird
        last_bird_ptr->SetVelocity(last_bird_ptr->GetVelocity() * glm::vec3{4});
      }
      if (bird_index == 2) {
        // Blue bird
        cubeList.pop_back();
        auto normal = last_bird_ptr->GetVelocity();
        auto position = last_bird_ptr->GetPosition();

        glm::vec3 upVector(0.0f, 1.0f, 0.0f); // Define an arbitrary up vector
        glm::vec3 y_axis = glm::normalize(glm::cross(normal, upVector));
        glm::vec3 x_axis = glm::normalize(glm::cross(normal, y_axis));

        // Calculate the vertices of the equilateral triangle based on the
        // position, orientation, and side length
        glm::vec3 vertex1 = position + y_axis * glm::vec3{blue_side};
        glm::vec3 vertex2 = position - glm::vec3{0.5 * blue_side} * y_axis -
                            glm::vec3{0.866 * blue_side} * x_axis;
        glm::vec3 vertex3 = position - glm::vec3{0.5 * blue_side} * y_axis +
                            glm::vec3{0.866 * blue_side} * x_axis;

        auto create_blue_bird = [&](glm::vec3 &position) {
          Cube *newCube = new Cube();
          newCube->SetType(BASE_BIRD_NUM + 2);
          newCube->SetPosition(position);
          newCube->SetColor(bird_colors[2]);
          newCube->SetRotation(last_bird_ptr->GetRotation());
          newCube->SetAngularVelocity(last_bird_ptr->GetAngularVelocity());
          newCube->SetVelocity(last_bird_ptr->GetVelocity() +
                               (position - last_bird_ptr->GetPosition()) *
                                   glm::vec3{blue_velocity_proportion});
          cubeList.push_back(newCube);
        };

        create_blue_bird(vertex1);
        create_blue_bird(vertex2);
        create_blue_bird(vertex3);

        delete (last_bird_ptr);
      }
      if (bird_index == 3) {
        last_bird_ptr->SetColor(glm::vec3{1, 0, 0});
        // Black bird
        for (auto obj_ptr : cubeList) {
          // Don't explode with itself. Don't explode background.
          if (obj_ptr == last_bird_ptr || obj_ptr->GetType() == 3)
            continue;

          auto obj_pos = obj_ptr->GetPosition();
          auto bird_pos = last_bird_ptr->GetPosition();
          if (glm::distance(obj_pos, bird_pos) < 7) {
            obj_ptr->SetVelocity(obj_ptr->GetVelocity() +
                                 glm::normalize(obj_pos - bird_pos) *
                                     glm::vec3{10});
          }
        }
      }

      can_use_ability = false;

      wasRightMouseButtonPressed = isRightMouseButtonPressed;
    } else {
      // spawnn bird
      // The right mouse button was just clicked
      Cube *newCube = new Cube();
      newCube->SetType(BASE_BIRD_NUM + next_bird_index);
      newCube->SetPosition(camera.Position);
      newCube->SetColor(bird_colors[next_bird_index]);
      // Set the cube's velocity to make it move in the direction the camera is
      // facing
      float initialSpeed = 8.0f; // Change this to the speed you want
      newCube->SetVelocity(initialSpeed * camera.Front);
      newCube->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
      newCube->SetAngularVelocity(glm::vec3(
          60.0f, 61.0f, 0.0f)); // Change this to the angular velocity you want
      cubeList.push_back(newCube);
      SoundEngine->play2D("audio/fire.mp3", false);
      // increase shot fired count
      shotFired++;

      if (next_bird_index != 0) {
        can_use_ability = true;
      }
      last_bird_ptr = newCube;

      next_bird_index = (next_bird_index + 1) % 4;
    }
  }
  wasRightMouseButtonPressed = isRightMouseButtonPressed;
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset =
        lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
  } else {
    firstMouse = true;
  }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void setShaderAndTexture() {}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}
