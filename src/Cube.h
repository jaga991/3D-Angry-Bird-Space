#pragma once
// clang-format off
#include <glm/glm.hpp>
#include <glad/glad.h>
// clang-format on
#include "shader_s.h"
#include <vector>

const int BASE_BIRD_NUM = 11;

class Cube {

public:
  Cube();
  void Draw(Shader &shader);
  void SetPosition(glm::vec3 newPosition);
  void SetVelocity(glm::vec3 newVelocity);
  void SetAngularVelocity(glm::vec3 angularVelocityIn);
  void SetRotation(const glm::vec3 &rotationIn);
  void SetScale(const glm::vec3 &scaleIn);
  glm::vec3 GetScale() const;
  void SetColor(const glm::vec3 &colorIn);
  void SetMass(float massIn);
  void SetInertia(const glm::mat3x3 &inertiaIn);

  void SetRestitution(float restitutionIn);
  void SetType(int typeIn);
  void SetPrevPos(glm::vec3 prevPosIn);

  int GetType();
  float GetRestitution();
  float GetMass();
  glm::vec3 GetPrevPos();
  glm::vec3 GetRotation();
  glm::mat3x3 GetInertia();
  glm::vec3 GetPosition() const;
  glm::vec3 GetVelocity();
  glm::vec3 GetAngularVelocity();
  std::vector<glm::vec3> GetVertices() const; // Add this method
  std::vector<std::pair<glm::vec3, glm::vec3>> GetEdges() const;
  std::vector<glm::vec3> GetFaceNormals() const;

  void deleteBuffers();

private:
  unsigned int VAO, VBO;
  glm::vec3 position;
  glm::vec3 velocity;
  glm::vec3 rotation;
  glm::vec3 color;

  glm::vec3 center;
  glm::vec3 angularVelocity;
  glm::vec3 scale;
  glm::vec3 prevPos;
  glm::mat3x3 inertia;
  float mass;
  float restitution;
  int type;
};
