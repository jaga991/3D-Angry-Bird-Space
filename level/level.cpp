#include "../src/Cube.h"
#include <cstdlib> // for rand() and srand()
#include <ctime> // for time()
std::vector<Cube*> loadLevel(int level) {
    std::vector<Cube*> cubeList;

    Cube* skyBox = new Cube();
    skyBox->SetType(3);
    skyBox->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
skyBox->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
skyBox->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
skyBox->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
skyBox->SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
skyBox->SetScale(glm::vec3(100.0f, 100.0f, 100.0f));
cubeList.push_back(skyBox);

    if (level == 0) {
		Cube* cube = new Cube();
        cube->SetType(1);
		cube->SetPosition(glm::vec3(4.0f, 0.0f, 0.0f));
		cube->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
		cube->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
		cube->SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
		cubeList.push_back(cube);

		Cube* cube2 = new Cube();
        cube2->SetType(2);
		cube2->SetPosition(glm::vec3(1.0f, 0.0f, 0.0f));
		cube2->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
		cube2->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
		cube2->SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
		cubeList.push_back(cube2);

		Cube* cube3 = new Cube();
        cube3->SetType(3);
		cube3->SetPosition(glm::vec3(2.0f, 0.0f, 0.0f));
		cube3->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
		cube3->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
		cube3->SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
		cubeList.push_back(cube3);

		Cube* cube4 = new Cube();
		cube4->SetType(4);
		cube4->SetPosition(glm::vec3(3.0f, 0.0f, 0.0f));
		cube4->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
		cube4->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
        cube4->SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
        cubeList.push_back(cube4);

        Cube* cube5 = new Cube();
        cube5->SetType(5);
        cube5->SetPosition(glm::vec3(4.0f, 0.0f, 0.0f));
        cube5->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
        cube5->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
        cube5->SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
        cubeList.push_back(cube5);
    }

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

        for (int i = 0; i < 30; i++) {
            // Generate random position
            float x = static_cast<float>(rand() % 15); 
            float y = static_cast<float>(rand() % 15);
            float z = static_cast<float>(rand() % 15);


            Cube* cube = new Cube();
            cube->SetPosition(glm::vec3(x, y, z));
            cube->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
            cube->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
            cube->SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));


            cubeList.push_back(cube);
        }
    }

    else if (level == 3) {
        for (int i = 0; i < 4; i++) {

            Cube* cube = new Cube();
            if (i < 2) {
                cube->SetType(1);
                cube->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
            }
            else
            {
                cube->SetType(2);
            }
            cube->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
            cube->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
            cube->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
            cube->SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
            cubeList.push_back(cube);
        }

    }

    return cubeList;
}