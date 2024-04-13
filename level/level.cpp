#include "../src/Cube.h"
#include <cstdlib> // for rand() and srand()
#include <ctime> // for time()
std::vector<Cube*> loadLevel(int level) {
    std::vector<Cube*> cubeList;

    if (level == 1) {
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

    }

    else if (level == 2) {
        // Initialize random seed
        srand(static_cast<unsigned>(time(0)));

        for (int i = 0; i < 50; i++) {
            // Generate random position
            float x = static_cast<float>(rand() % 20); // Random float between -50 and 50
            float y = static_cast<float>(rand() % 20);
            float z = static_cast<float>(rand() % 20);

            // Generate random scale
            float scale = static_cast<float>(rand() % 2 + 1); // Random float between 1 and 10

            Cube* cube = new Cube();
            cube->SetPosition(glm::vec3(x, y, z));
            cube->SetScale(glm::vec3(scale, scale, scale));
            cube->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
            cube->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
            cube->SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
            cubeList.push_back(cube);
        }
    }

    return cubeList;
}