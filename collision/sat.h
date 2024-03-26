#pragma once
#include <glm/glm.hpp>
#include "../src/Cube.h"
bool areCubesColliding(const Cube& cube1, const Cube& cube2);
void projectCubeOntoAxis(const Cube& cube, const glm::vec3& axis, float& minProj, float& maxProj);