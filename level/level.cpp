#include "../src/Cube.h"
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
std::vector<Cube *> loadLevel(int level, int &score) {
  std::vector<Cube *> cubeList;

  Cube *skyBox = new Cube();
  skyBox->SetType(3);
  skyBox->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
  skyBox->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
  skyBox->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
  skyBox->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
  skyBox->SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
  skyBox->SetScale(glm::vec3(100.0f, 100.0f, 100.0f));
  cubeList.push_back(skyBox);

  if (level == 0) {
    Cube *cube = new Cube();
    cube->SetType(1);
    cube->SetPosition(glm::vec3(4.0f, 0.0f, 0.0f));
    cube->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
    cube->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    cube->SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
    cubeList.push_back(cube);

    Cube *cube2 = new Cube();
    cube2->SetType(BASE_BIRD_NUM);
    cube2->SetPosition(glm::vec3(1.0f, 0.0f, 0.0f));
    cube2->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
    cube2->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    cube2->SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
    cubeList.push_back(cube2);

    Cube *cube3 = new Cube();
    cube3->SetType(3);
    cube3->SetPosition(glm::vec3(2.0f, 0.0f, 0.0f));
    cube3->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
    cube3->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    cube3->SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
    cubeList.push_back(cube3);

    Cube *cube4 = new Cube();
    cube4->SetType(4);
    cube4->SetPosition(glm::vec3(3.0f, 0.0f, 0.0f));
    cube4->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
    cube4->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    cube4->SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
    cubeList.push_back(cube4);

    Cube *cube5 = new Cube();
    cube5->SetType(5);
    cube5->SetPosition(glm::vec3(4.0f, 0.0f, 0.0f));
    cube5->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
    cube5->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    cube5->SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
    cubeList.push_back(cube5);
  }

  if (level == 1) {

    Cube *longCube = new Cube();
    longCube->SetType(5);
    longCube->SetPosition(glm::vec3(1.0f, 2.0f, 0.0f));
    longCube->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
    longCube->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    longCube->SetScale(glm::vec3(3.0f, 1.0f, 1.0f));
    longCube->SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
    cubeList.push_back(longCube);

    Cube *longCube2 = new Cube();

    longCube2->SetType(5);
    longCube2->SetPosition(glm::vec3(1.0f, 4.0f, 0.0f));
    longCube2->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
    longCube2->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    longCube2->SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
    longCube2->SetColor(glm::vec3(5.0f, 5.0f, 5.0f));
    cubeList.push_back(longCube2);

  }

  else if (level == 2) {

    for (int i = 0; i < 3; i++) {
      float x = 0.0f + 1.0f * i;
      Cube *cube = new Cube();
      cube->SetType(1);
      cube->SetScale(glm::vec3(1.0f, 3.0f, 1.0f));
      cube->SetPosition(glm::vec3(x, 0.0f, 15.0f));
      cube->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
      cube->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
      cube->SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
      cubeList.push_back(cube);
    }

    for (int i = 0; i < 3; i++) {
      float x = 0.0f + 1.0f * i;
      Cube *cube = new Cube();
      cube->SetType(5);
      cube->SetScale(glm::vec3(1.0f, 3.0f, 1.0f));
      cube->SetPosition(glm::vec3(x, 7.0f, 13.0f));
      cube->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
      cube->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
      cube->SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
      cubeList.push_back(cube);
    }

    for (int i = 0; i < 3; i++) {
      float x = 0.0f + 1.0f * i;
      Cube *cube = new Cube();
      cube->SetType(1);
      cube->SetPosition(glm::vec3(x, 0.0f, 10.0f));
      cube->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
      cube->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
      cube->SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
      cubeList.push_back(cube);
    }
  }

  else if (level == 3) {
    for (int i = 0; i < 30; i++) {
      int types[] = {1, 4, 5}; // Array of desired types
      int index = rand() % 3;  // Generate a random index (0, 1, or 2)
      Cube *cube = new Cube();
      cube->SetType(types[index]); // Set the type of the cube

      // Generate a random position between -10.0 and 10.0 for each component
      float x =
          static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 20.0f -
          10.0f;
      float y =
          static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 20.0f -
          10.0f;
      float z =
          static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 20.0f -
          10.0f;
      cube->SetPosition(glm::vec3(x, y, z));

      cubeList.push_back(cube); // Add the cube to the list
    }
  }

  for (auto curr_cube : cubeList) {
    // stone
    if (curr_cube->GetType() == 1) {
      curr_cube->SetColor(glm::vec3(0.502f, 0.502f, 0.502f));
      curr_cube->SetMass(50.0f);
      curr_cube->SetRestitution(0.3f);
    }
    // bird
    else if (curr_cube->GetType() == BASE_BIRD_NUM) {
      curr_cube->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
      curr_cube->SetMass(10.0f);
      curr_cube->SetRestitution(0.8f);
    }
    // pig
    else if (curr_cube->GetType() == 4) {
      curr_cube->SetColor(glm::vec3(1.0f, 1.0f, 0.0f));
      curr_cube->SetMass(10.0f);
      curr_cube->SetRestitution(0.8f);
      score++;
    }
    // wood
    else if (curr_cube->GetType() == 5) {
      curr_cube->SetColor(glm::vec3(0.565f, 0.933f, 0.565f));
      curr_cube->SetMass(15.0f);
      curr_cube->SetRestitution(0.8f);
    }
  }

  return cubeList;
}
