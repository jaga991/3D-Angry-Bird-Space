
#include "level.h"
#include "../src/Cube.h"

std::vector<Cube*> loadLevel(int level) {
    std::vector<Cube*> cubeList;


    for (int i = 0; i < 2; i++) {
        float x = 0.0f + 1.0f * i;
        Cube* cube = new Cube();
        cube->SetPosition(glm::vec3(0.0f, x, 0.0f));
        cube->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
        cube->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
        cube->SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
        cubeList.push_back(cube);
    }

    for (int i = 0; i < 2; i++) {
        float x = 0.0f + 1.0f * i;
        Cube* cube = new Cube();
        cube->SetPosition(glm::vec3(2.0f, x, 0.0f));
        cube->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
        cube->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
        cube->SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
        cubeList.push_back(cube);
    }

    Cube* longCube = new Cube();
    longCube->SetPosition(glm::vec3(1.0f, 2.0f, 0.0f));
    longCube->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
    longCube->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    longCube->SetScale(glm::vec3(3.0f, 1.0f, 1.0f));
    longCube->SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
    cubeList.push_back(longCube);

    Cube* longCube2 = new Cube();
    longCube2->SetPosition(glm::vec3(1.0f, 4.0f, 0.0f));
    longCube2->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
    longCube2->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    longCube2->SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
    longCube2->SetColor(glm::vec3(5.0f, 5.0f, 5.0f));
    cubeList.push_back(longCube2);

    return cubeList;
}