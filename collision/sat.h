#pragma once
#include <glm/glm.hpp>
#include "../src/Cube.h"
std::pair<bool, std::pair<glm::vec3, float>> areCubesColliding(const Cube& cube1, const Cube& cube2);
void projectCubeOntoAxis(const Cube& cube, const glm::vec3& axis, float& minProj, float& maxProj);
std::vector<glm::vec3> getCollidingVertices(const Cube& cube, const glm::vec3& overlapAxis, float minOverlap, float maxOverlap);