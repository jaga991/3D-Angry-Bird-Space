#pragma once
#include <glm/glm.hpp>
#include "../src/Cube.h"
std::pair<bool, std::pair<glm::vec3, float>> areCubesColliding(const Cube& cube1, const Cube& cube2);
void projectCubeOntoAxis(const Cube& cube, const glm::vec3& axis, float& minProj, float& maxProj);
int detectCollisionType(const Cube& cube1, const Cube& cube2, glm::vec3 mtv, float mtvMagnitude);
glm::vec3 getContactPoint(const Cube& cube1, const Cube& cube2, int collisionType, glm::vec3 mtv, float mtvMagnitude);
std::pair<glm::vec3, glm::vec3> calculateShortestLine(std::pair<glm::vec3, glm::vec3> line1, std::pair<glm::vec3, glm::vec3> line2);
