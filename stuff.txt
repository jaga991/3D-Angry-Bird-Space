
// Calculate the vectors from the centers of mass to the point of contact
glm::vec3 r1 = contactPoint - cubeList[i]->GetPosition();
glm::vec3 r2 = contactPoint - cubeList[j]->GetPosition();

// Calculate the relative velocities at the point of contact
glm::vec3 relativeVelocity1 = cubeList[i]->GetVelocity() + glm::cross(cubeList[i]->GetAngularVelocity(), r1);
glm::vec3 relativeVelocity2 = cubeList[j]->GetVelocity() + glm::cross(cubeList[j]->GetAngularVelocity(), r2);
glm::vec3 relativeVelocity = relativeVelocity1 - relativeVelocity2;

// Calculate the moments of inertia
float I1 = cubeList[i]->GetMass() * glm::length(cubeList[i]->GetScale()) * glm::length(cubeList[i]->GetScale()) / 6.0f;
float I2 = cubeList[j]->GetMass() * glm::length(cubeList[j]->GetScale()) * glm::length(cubeList[j]->GetScale()) / 6.0f;

// Calculate the impulse
float impulseMagnitude = -(1 + ((cubeList[i]->GetRestitution() + cubeList[j]->GetRestitution())/2)) * glm::dot(relativeVelocity, mtv) / (1 / cubeList[i]->GetMass() + 1 / cubeList[j]->GetMass() + glm::dot(glm::cross(r1, mtv), glm::cross(r1, mtv)) / I1 + glm::dot(glm::cross(r2, mtv), glm::cross(r2, mtv)) / I2);
glm::vec3 impulse = impulseMagnitude * mtv;

// Apply the impulse to the linear velocities
cubeList[i]->SetVelocity(cubeList[i]->GetVelocity() + impulse / cubeList[i]->GetMass());
cubeList[j]->SetVelocity(cubeList[j]->GetVelocity() - impulse / cubeList[j]->GetMass());

// Apply the impulse to the angular velocities
glm::vec3 angularImpulse1 = glm::cross(r1, impulse) / I1;
glm::vec3 angularImpulse2 = glm::cross(r2, -impulse) / I2;
cubeList[i]->SetAngularVelocity(cubeList[i]->GetAngularVelocity() + angularImpulse1);
cubeList[j]->SetAngularVelocity(cubeList[j]->GetAngularVelocity() + angularImpulse2);


type 0 default
type 1 pig
type 2 bird
type 3 wood
type 4 ice
type 5 stone