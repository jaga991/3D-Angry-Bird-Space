#include "sat.h"

std::pair<bool, std::pair<glm::vec3, float>>areCubesColliding(const Cube& cube1, const Cube& cube2) {
    // Get the edges of the cubes
    std::vector<glm::vec3> normals1 = cube1.GetFaceNormals();
    std::vector<glm::vec3> normals2 = cube2.GetFaceNormals();

    std::vector<glm::vec3>cube1Normals;
    cube1Normals.push_back(normals1[0]);
    cube1Normals.push_back(normals1[2]);
    cube1Normals.push_back(normals1[4]);

    std::vector<glm::vec3>cube2Normals;
    cube2Normals.push_back(normals2[0]);
    cube2Normals.push_back(normals2[2]);
    cube2Normals.push_back(normals2[4]);

    // Define the axes to test
    std::vector<glm::vec3> axes;
    // Add the face normals to the axes
    axes.push_back(normals1[0]);
    axes.push_back(normals1[2]);
    axes.push_back(normals1[4]);
    axes.push_back(normals2[0]);
    axes.push_back(normals2[2]);
    axes.push_back(normals2[4]);
    // Add the cross products of the face nomal of each cubes
    for (glm::vec3& normal1 : cube1Normals) {
        for (glm::vec3& normal2 : cube2Normals) {
			glm::vec3 crossProduct = glm::cross(normal1, normal2);
			if (glm::length(crossProduct) > 0.0001f) { // Avoid adding zero vectors
            	axes.push_back(glm::normalize(crossProduct));
            }
		}
	}

    //minimum translation vector
    float smallestOverlap = std::numeric_limits<float>::max();
    glm::vec3 smallestOverlapAxis;
    //std::cout << "start of checking" << std::endl;
    // For each axis
    for (glm::vec3& axis : axes) {
        // Project the vertices of both cubes onto the axis
        //print out the axis
        float minProj1, maxProj1, minProj2, maxProj2;
        projectCubeOntoAxis(cube1, axis, minProj1, maxProj1);
        projectCubeOntoAxis(cube2, axis, minProj2, maxProj2);



        for (glm::vec3& axis : axes) {
            //std::cout << "axis: " << axis.x << " " << axis.y << " " << axis.z << std::endl;
            float minProj1, maxProj1, minProj2, maxProj2;
            projectCubeOntoAxis(cube1, axis, minProj1, maxProj1);
            projectCubeOntoAxis(cube2, axis, minProj2, maxProj2);

            float overlap = maxProj1 < minProj2 ? maxProj1 - minProj2 : maxProj2 - minProj1;
            if (overlap < 0) {
                //std::cout<<"end of checking"<<std::endl;
                return { false, {glm::vec3(0.0f), 0.0f} };
            }
            else if (overlap < smallestOverlap) {
                smallestOverlap = overlap;
                smallestOverlapAxis = axis;
            }
        }
    }
    //std::cout << "smallestOverlapAxis: " << smallestOverlapAxis.x << " " << smallestOverlapAxis.y << " " << smallestOverlapAxis.z << std::endl;
    //std::cout << "smallestOverlap: " << smallestOverlap << std::endl;
    //// The projections overlap on all axes, so the cubes are colliding
    //std::cout << "end of checking" << std::endl;

    return { true, { smallestOverlapAxis, smallestOverlap } };

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

int detectCollisionType(const Cube& cube1, const Cube& cube2, glm::vec3 mtv, float mtvMagnitude) {
    // Get the face normals of the cubes
    std::vector<glm::vec3> normals1 = cube1.GetFaceNormals();
    std::vector<glm::vec3> normals2 = cube2.GetFaceNormals();

    // Check if the MTV corresponds to one of the face normals
    for (glm::vec3& normal : normals1) {
        if (glm::length(normal - mtv) < 0.0001f) {
            return 1;  // Face-vertex collision
        }
    }
    for (glm::vec3& normal : normals2) {
        if (glm::length(normal - mtv) < 0.0001f) {
            return 1;  // Face-vertex collision
        }
    }

    // If the MTV does not correspond to a face normal, it's an edge-edge collision
    return 2;

}

glm::vec3 getContactPoint(const Cube& cube1, const Cube& cube2, int collisionType, glm::vec3 mtv, float mtvMagnitude) {
	// Get the vertices of the cubes
    //std::cout<<"start of getContactPoint"<<std::endl;
    if (collisionType == 1) {
        //std::cout<<"start of collisionType 1"<<std::endl;
        //find out which cube is the one that is face colliding
        Cube faceCollidingCube;
        Cube vertexCollidingCube;

        std::vector<glm::vec3>faceNormal1 = cube1.GetFaceNormals();
        std::vector<glm::vec3>faceNormal2 = cube2.GetFaceNormals();

        for (glm::vec3& normal : faceNormal1) {
            if (glm::length(glm::cross(normal, mtv)) < 0.001f) {
                faceCollidingCube = cube1;
                vertexCollidingCube = cube2;
                break;
            }
        }

        for (glm::vec3& normal : faceNormal2) {
            if (glm::length(glm::cross(normal, mtv)) < 0.001f) {
                faceCollidingCube = cube2;
                vertexCollidingCube = cube1;
                break;
            }
        }

        //find which vertex from vertexCollidingCube is closest to faceCollidingCube, and return that vertex
        std::vector<glm::vec3> vertices = faceCollidingCube.GetVertices();
        glm::vec3 closestVertex = vertices[0];
        float minDistance = glm::distance(vertices[0], vertexCollidingCube.GetVertices()[0]);
        for (int i = 1; i < vertices.size(); i++) {
            float distance = glm::distance(vertices[i], vertexCollidingCube.GetVertices()[0]);
            if (distance < minDistance) {
                closestVertex = vertices[i];
                minDistance = distance;
            }
            //std::cout << "closestVertex: " << closestVertex.x << " " << closestVertex.y << " " << closestVertex.z << std::endl;
            return closestVertex;
        }

    }

    // if its an edge edge collision
    if (collisionType == 2) {
        //std::cout << "start of collisionType 2" << std::endl;

        //get the face normals of both cubes
        std::vector<glm::vec3> faceNormal1;
        std::vector<glm::vec3> faceNormal2;

        glm::vec3 validNormal1;
        glm::vec3 validNormal2;

        std::vector<glm::vec3> normals1 = cube1.GetFaceNormals();
        std::vector<glm::vec3> normals2 = cube2.GetFaceNormals();

        faceNormal1.push_back(normals1[0]);
        faceNormal1.push_back(normals1[2]);
        faceNormal1.push_back(normals1[4]);

        faceNormal2.push_back(normals2[0]);
        faceNormal2.push_back(normals2[2]);
        faceNormal2.push_back(normals2[4]);


        //find the normal that is perpendicular to the mtv
        for (glm::vec3& normal : faceNormal1) {
            if (std::abs(glm::dot(normal, mtv)) < 0.0001f) {
                validNormal1 = normal;
                break;
            }
        }
        for (glm::vec3& normal : faceNormal2) {
            if (std::abs(glm::dot(normal, mtv)) < 0.0001f) {
                validNormal2 = normal;
                break;
            }
        }

        
        //find the edge of cube1 and 2 that is involved in the collision
        std::vector<std::pair<glm::vec3, glm::vec3>> edges1 = cube1.GetEdges();
        std::vector<std::pair<glm::vec3, glm::vec3>> edges2 = cube2.GetEdges();
        std::vector<std::pair<glm::vec3, glm::vec3>> validEdges1;
        std::vector<std::pair<glm::vec3, glm::vec3>> validEdges2;

        //find the edges that are parallel to the valid norm
        for (std::pair<glm::vec3, glm::vec3>& edge : edges1) {
            glm::vec3 edgeVector = edge.second - edge.first;
            if (glm::length(glm::cross(edgeVector, validNormal1)) < 0.0001f) {
                validEdges1.push_back(edge);
            }
        }
        for (std::pair<glm::vec3, glm::vec3>& edge : edges2) {
            glm::vec3 edgeVector = edge.second - edge.first;
            if (glm::length(glm::cross(edgeVector, validNormal2)) < 0.0001f) {
                validEdges2.push_back(edge);
            }
        }

        //std::cout<<"printing out valid edges"<<std::endl;
        for (std::pair<glm::vec3, glm::vec3>& edge : validEdges1) {
            //std::cout << "validEdges1: " << edge.first.x << " " << edge.first.y << " " << edge.first.z << " to " << edge.second.x << " " << edge.second.y << " " << edge.second.z << std::endl;
        }
        for (std::pair<glm::vec3, glm::vec3>& edge : validEdges2) {
            //std::cout << "validEdges2: " << edge.first.x << " " << edge.first.y << " " << edge.first.z << " to " << edge.second.x << " " << edge.second.y << " " << edge.second.z << std::endl;
        }



        //find the closest edge of cube1 from center of cube 2
        glm::vec3 center2 = cube2.GetPosition();
        std::pair<glm::vec3, glm::vec3> closestEdge1;
        float minDistance1 = std::numeric_limits<float>::max();
        //for each edge in validEdges1
        for (std::pair<glm::vec3, glm::vec3>& edge : validEdges1) {
            glm::vec3 edgeCenter = (edge.first + edge.second) / 2.0f;
            float distance = glm::length(center2 - edgeCenter);
            if (distance < minDistance1) {
                minDistance1 = distance;
                closestEdge1 = edge;
            }
        }

        //find the closest edge of cube2 from center of cube 1
        glm::vec3 center1 = cube1.GetPosition();
        std::pair<glm::vec3, glm::vec3> closestEdge2;
        float minDistance2 = std::numeric_limits<float>::max();
        //for each edge in validEdges2
        for (std::pair<glm::vec3, glm::vec3>& edge : validEdges2) {
			glm::vec3 edgeCenter = (edge.first + edge.second) / 2.0f;
			float distance = glm::length(center1 - edgeCenter);
            if (distance < minDistance2) {
				minDistance2 = distance;
				closestEdge2 = edge;
			}
		}

        // Calculate the line of shortest distance between the two edges
        std::pair<glm::vec3, glm::vec3> shortestLine = calculateShortestLine(closestEdge1, closestEdge2);

        // The contact point is the midpoint of the shortest line
        glm::vec3 contactPoint = (shortestLine.first + shortestLine.second) / 2.0f;

        // Now contactPoint is the contact point between the two cubes
        return contactPoint;


    }
}


std::pair<glm::vec3, glm::vec3> calculateShortestLine(std::pair<glm::vec3, glm::vec3> line1, std::pair<glm::vec3, glm::vec3> line2) {
    glm::vec3 u = line1.second - line1.first;
    glm::vec3 v = line2.second - line2.first;
    glm::vec3 w = line1.first - line2.first;

    float a = glm::dot(u, u);
    float b = glm::dot(u, v);
    float c = glm::dot(v, v);
    float d = glm::dot(u, w);
    float e = glm::dot(v, w);
    float D = a * c - b * b;
    float sc, tc;

    // compute the line parameters of the two closest points
    if (D < 0.0001f) {          // the lines are almost parallel
        sc = 0.0;
        tc = (b > c ? d / b : e / c);    // use the largest denominator
    }
    else {
        sc = (b * e - c * d) / D;
        tc = (a * e - b * d) / D;
    }

    // get the difference of the two closest points
    glm::vec3 dP = w + (sc * u) - (tc * v);  // = line1(sc) - line2(tc)

    glm::vec3 closestPointOnLine1 = line1.first + sc * u;
    glm::vec3 closestPointOnLine2 = line2.first + tc * v;

    return { closestPointOnLine1, closestPointOnLine2 };    // return the closest points
}
