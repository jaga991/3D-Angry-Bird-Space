#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "shader_s.h"
#include <vector>


class Cube {



public:
    Cube();
    void Draw(Shader& shader);
    void SetPosition(glm::vec3 newPosition);
    void SetVelocity(glm::vec3 newVelocity);
    void SetRotation(const glm::vec3& rotationIn);
    void SetColor(const glm::vec3& colorIn);
    void SetMass(float massIn);
	void SetInertia(const glm::mat3x3& inertiaIn);
    
    float GetMass();
    glm::mat3x3 GetInertia();
    glm::vec3 GetPosition();
    glm::vec3 GetVelocity();
    std::vector<glm::vec3> GetVertices() const; // Add this method
    std::vector<glm::vec3> GetEdges() const;
    std::vector<glm::vec3> GetFaceNormals() const;

    void deleteBuffers();


private:
    unsigned int VAO, VBO;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 rotation;
    glm::vec3 color;
    glm::mat3x3 inertia;
    glm::vec3 center;
    float mass;
};