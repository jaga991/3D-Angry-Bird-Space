#include "sat.h"

std::pair<bool, std::pair<glm::vec3, float>> areCubesColliding(const Cube& cube1, const Cube& cube2) {
    std::vector<glm::vec3> edges1 = cube1.GetEdges();
    std::vector<glm::vec3> edges2 = cube2.GetEdges();
    std::vector<glm::vec3> normals1 = cube1.GetFaceNormals();
    std::vector<glm::vec3> normals2 = cube2.GetFaceNormals();

    std::vector<glm::vec3> axes = {
        glm::vec3(1, 0, 0), // x-axis
        glm::vec3(0, 1, 0), // y-axis
        glm::vec3(0, 0, 1)  // z-axis
    };

    axes.insert(axes.end(), normals1.begin(), normals1.end());
    axes.insert(axes.end(), normals2.begin(), normals2.end());

    for (glm::vec3& edge1 : edges1) {
        for (glm::vec3& edge2 : edges2) {
            glm::vec3 crossProduct = glm::cross(edge1, edge2);
            if (glm::length(crossProduct) > 0.0001f) { // Avoid adding zero vectors
                axes.push_back(glm::normalize(crossProduct));
            }
        }
    }

    float smallestOverlap = std::numeric_limits<float>::max();
    glm::vec3 smallestOverlapAxis;

    for (glm::vec3& axis : axes) {
        float minProj1, maxProj1, minProj2, maxProj2;
        projectCubeOntoAxis(cube1, axis, minProj1, maxProj1);
        projectCubeOntoAxis(cube2, axis, minProj2, maxProj2);

        float overlap = maxProj1 < minProj2 ? maxProj1 - minProj2 : maxProj2 - minProj1;
        if (overlap < 0) {
            return { false, {glm::vec3(0.0f), 0.0f} };
        }
        else if (overlap < smallestOverlap) {
            smallestOverlap = overlap;
            smallestOverlapAxis = axis;
        }
    }

    return { true, {smallestOverlapAxis, smallestOverlap} };
}

void projectCubeOntoAxis(const Cube& cube, const glm::vec3& axis, float& minProj, float& maxProj) {
    std::vector<glm::vec3> vertices = cube.GetVertices();

    minProj = maxProj = glm::dot(vertices[0], axis);

    for (int i = 1; i < vertices.size(); i++) {
        float proj = glm::dot(vertices[i], axis);

        if (proj < minProj) {
            minProj = proj;
        }
        else if (proj > maxProj) {
            maxProj = proj;
        }
    }
}
std::vector<glm::vec3> getCollidingVertices(const Cube& cube, const glm::vec3& overlapAxis, float minOverlap, float maxOverlap) {
    std::vector<glm::vec3> vertices = cube.GetVertices();
    std::vector<glm::vec3> collidingVertices;

    for (const glm::vec3& vertex : vertices) {
        float projection = glm::dot(vertex, overlapAxis);
        if (projection >= minOverlap && projection <= maxOverlap) {
            collidingVertices.push_back(vertex);
        }
    }

    return collidingVertices;
}
