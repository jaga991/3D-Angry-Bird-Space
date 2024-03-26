#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "shader_s.h"
#include <vector>

struct massData
{
    glm::mat3x3 inertia;
    glm::vec3 center;
    float mass;
};


class Cube {
    glm::vec3 rotation;
    glm::vec3 color;


public:
    Cube();
    void Draw(Shader& shader);
    void SetPosition(glm::vec3 newPosition);
    void SetVelocity(glm::vec3 newVelocity);
    void SetRotation(const glm::vec3& rotationIn);
    void SetColor(const glm::vec3& colorIn);

    glm::vec3 GetPosition();
    glm::vec3 GetVelocity();
    std::vector<glm::vec3> GetVertices() const; // Add this method
    std::vector<glm::vec3> GetEdges() const;
    std::vector<glm::vec3> GetFaceNormals() const;

    void deleteBuffers();

    void SetMassData(massData newMassData);
    massData GetMassData();

private:
    unsigned int VAO, VBO;
    glm::vec3 position;
    glm::vec3 velocity;
    massData m_data;
};