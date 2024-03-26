#include "sat.h"

bool areCubesColliding(const Cube& cube1, const Cube& cube2) {
    // Get the edges of the cubes
    std::vector<glm::vec3> edges1 = cube1.GetEdges();
    std::vector<glm::vec3> edges2 = cube2.GetEdges();
    std::vector<glm::vec3> normals1 = cube1.GetFaceNormals();
    std::vector<glm::vec3> normals2 = cube2.GetFaceNormals();

    // Define the axes to test
    std::vector<glm::vec3> axes = {
        glm::vec3(1, 0, 0), // x-axis
        glm::vec3(0, 1, 0), // y-axis
        glm::vec3(0, 0, 1)  // z-axis
    };
    // Add the face normals to the axes
    axes.insert(axes.end(), normals1.begin(), normals1.end());
    axes.insert(axes.end(), normals2.begin(), normals2.end());
    // Add the cross products of the edges to the axes
    for (glm::vec3& edge1 : edges1) {
        for (glm::vec3& edge2 : edges2) {
            glm::vec3 crossProduct = glm::cross(edge1, edge2);
            if (glm::length(crossProduct) > 0.0001f) { // Avoid adding zero vectors
                axes.push_back(glm::normalize(crossProduct));
            }
        }

    }

    // For each axis
    for (glm::vec3& axis : axes) {
        // Project the vertices of both cubes onto the axis
        //print out the axis
        float minProj1, maxProj1, minProj2, maxProj2;
        projectCubeOntoAxis(cube1, axis, minProj1, maxProj1);
        projectCubeOntoAxis(cube2, axis, minProj2, maxProj2);

        // Check if the projections overlap
        if (maxProj1 < minProj2 || maxProj2 < minProj1) {
            // The projections do not overlap, so the cubes are not colliding
            return false;
        }
    }

    // The projections overlap on all axes, so the cubes are colliding
    return true;
}

void projectCubeOntoAxis(const Cube& cube, const glm::vec3& axis, float& minProj, float& maxProj) {
    // Get the vertices of the cube
    std::vector<glm::vec3> vertices = cube.GetVertices();

    // Initialize minProj and maxProj to the projection of the first vertex
    minProj = maxProj = glm::dot(vertices[0], axis);

    // For each vertex of the cube
    for (int i = 1; i < vertices.size(); i++) {
        // Project the vertex onto the axis
        float proj = glm::dot(vertices[i], axis);

        // Update minProj and maxProj
        if (proj < minProj) {
            minProj = proj;
        }
        else if (proj > maxProj) {
            maxProj = proj;
        }
    }
}