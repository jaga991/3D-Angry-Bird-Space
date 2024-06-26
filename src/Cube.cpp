#include "Cube.h"
#include <glm/gtc/matrix_transform.hpp> 
#include <vector>


Cube::Cube() : position(0.0f, 0.0f, 0.0f), velocity(0.0f, 0.0f, 0.0f), color(0.0f, 1.0f, 0.0f), mass(20.0f), angularVelocity(0.0f,0.0f,0.0f), scale(1.0f,1.0f,1.0f), restitution(0.5f), type(0), rotation(0.0f,0.0f,0.0f){
    float vertices[] = {
           -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
           -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
           -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

           -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
           -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
           -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

           -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
           -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
           -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
           -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
           -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
           -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

           -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
           -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
           -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

           -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
           -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
           -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };


    inertia = glm::mat3x3(1.0f);
    float Ix = (mass / 12.0f) * (this->GetScale().y * this->GetScale().y + this->GetScale().z * this->GetScale().z);
    float Iy = (mass / 12.0f) * (this->GetScale().x * this->GetScale().x + this->GetScale().z * this->GetScale().z);
    float Iz = (mass / 12.0f) * (this->GetScale().x * this->GetScale().x + this->GetScale().y * this->GetScale().y);

    inertia[0][0] = Ix;
    inertia[1][1] = Iy;
    inertia[2][2] = Iz;


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Cube::Draw(Shader& shader) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, scale);  // Apply scale to the model matrix

    shader.setVec3("color", color);
    shader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << "OpenGL error: " << error << std::endl;
    }
}

std::vector<glm::vec3> Cube::GetVertices() const {
    // Create the model matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position); // Apply the translation
    model = glm::scale(model, scale); // Apply the scale
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    // Define the vertices of the cube
    std::vector<glm::vec3> vertices = {
        glm::vec3(-0.5f, -0.5f, -0.5f), //lower left back
        glm::vec3(0.5f, -0.5f, -0.5f), //lower right back
        glm::vec3(0.5f, 0.5f, -0.5f), //upper right back
        glm::vec3(-0.5f, 0.5f, -0.5f), //upper left back
        glm::vec3(-0.5f, -0.5f, 0.5f), //lower left front
        glm::vec3(0.5f, -0.5f, 0.5f), //lower right front
        glm::vec3(0.5f, 0.5f, 0.5f), //upper right front
        glm::vec3(-0.5f, 0.5f, 0.5f) //upper left front
    };

    // Transform the vertices by the model matrix
    for (glm::vec3& vertex : vertices) {
        glm::vec4 transformedVertex = model * glm::vec4(vertex, 1.0f);
        vertex = glm::vec3(transformedVertex);
    }

    return vertices;
}


std::vector<std::pair<glm::vec3, glm::vec3>> Cube::GetEdges() const {
    // Get the vertices of the cube
    std::vector<glm::vec3> vertices = GetVertices();

    // Define the edges
    std::vector<std::pair<glm::vec3, glm::vec3>> edges = {
        {vertices[0], vertices[1]}, // Edge from vertex 0 to vertex 1
        {vertices[1], vertices[2]}, // Edge from vertex 1 to vertex 2
        {vertices[2], vertices[3]}, // Edge from vertex 2 to vertex 3
        {vertices[3], vertices[0]}, // Edge from vertex 3 to vertex 0
        {vertices[4], vertices[5]}, // Edge from vertex 4 to vertex 5
        {vertices[5], vertices[6]}, // Edge from vertex 5 to vertex 6
        {vertices[6], vertices[7]}, // Edge from vertex 6 to vertex 7
        {vertices[7], vertices[4]}, // Edge from vertex 7 to vertex 4
        {vertices[0], vertices[4]}, // Edge from vertex 0 to vertex 4
        {vertices[1], vertices[5]}, // Edge from vertex 1 to vertex 5
        {vertices[2], vertices[6]}, // Edge from vertex 2 to vertex 6
        {vertices[3], vertices[7]}  // Edge from vertex 3 to vertex 7
    };

    return edges;
}

std::vector<glm::vec3> Cube::GetFaceNormals() const {
    // Get the vertices of the cube
    std::vector<glm::vec3> vertices = GetVertices();

    // Define the face normals
    std::vector<glm::vec3> normals = {
        glm::normalize(glm::cross(vertices[1] - vertices[0], vertices[3] - vertices[0])), // Normal of face 0-1-3-2
        glm::normalize(glm::cross(vertices[5] - vertices[4], vertices[7] - vertices[4])), // Normal of face 4-5-7-6
        glm::normalize(glm::cross(vertices[4] - vertices[0], vertices[3] - vertices[0])), // Normal of face 0-4-7-3
        glm::normalize(glm::cross(vertices[5] - vertices[1], vertices[2] - vertices[1])), // Normal of face 1-5-6-2
        glm::normalize(glm::cross(vertices[4] - vertices[0], vertices[1] - vertices[0])), // Normal of face 0-4-5-1
        glm::normalize(glm::cross(vertices[7] - vertices[3], vertices[2] - vertices[3]))  // Normal of face 3-7-6-2
    };

    return normals;
}

void Cube::SetPosition(glm::vec3 newPosition) {
    position = newPosition;
}

void Cube::SetVelocity(glm::vec3 newVelocity) {
    velocity = newVelocity;
}

void Cube::SetAngularVelocity(glm::vec3 angularVelocityIn)
{
	angularVelocity = angularVelocityIn;
}

void Cube::SetRotation(const glm::vec3& rotationIn) {
    this->rotation = rotationIn;
}

void Cube::SetColor(const glm::vec3& colorIn) {
    this->color = colorIn;
}
void Cube::SetMass(float massIn)
{
    this->mass = massIn;
}

void Cube::SetInertia(const glm::mat3x3& inertiaIn)
{
    this->inertia = inertiaIn;
}

// Add a method to set the scale
void Cube::SetScale(const glm::vec3& scaleIn) {
    scale = scaleIn;
}

void Cube::SetRestitution(float restitutionIn) {
	restitution = restitutionIn;
}
// Add a method to get the scale
glm::vec3 Cube::GetScale() const {
    return scale;
}

void Cube::SetType(int typeIn) {
	type = typeIn;
}

void Cube::SetPrevPos(glm::vec3 prevPosIn) {
	prevPos = prevPosIn;
}

glm::vec3 Cube::GetPosition() const {
    return position;
}

glm::vec3 Cube::GetVelocity() {
    return velocity;
}

glm::vec3 Cube::GetAngularVelocity()
{
	return angularVelocity;
}

glm::vec3 Cube::GetRotation()
{
	return rotation;
}

float Cube::GetMass()
{
		return mass;
}

glm::vec3 Cube::GetPrevPos()
{
	return prevPos;
}

glm::mat3x3 Cube::GetInertia()
{
    return inertia;
}

int Cube::GetType() {
	return type;
}

float Cube::GetRestitution() {
	return restitution;
}

void Cube::deleteBuffers() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}



