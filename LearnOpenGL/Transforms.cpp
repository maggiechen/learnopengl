// including these GLM header files will inject the GLM cpp code into this file

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Transforms.h"

void Transforms::SomeVectorShenanigans() {
	// Position vector <1, 0, 0>
	glm::vec4 vec(1.0, 0.0, 0.0, 1.0);
	glm::mat4 translateMatrix = glm::mat4(1.f); // creates matrix with 1s along diagonal, so the identity matrix

	// Transform with direction <1, 1, 0>
	translateMatrix = glm::translate(translateMatrix, glm::vec3(1.0, 1.0, 0.0));

	vec = translateMatrix * vec;
	std::cout << vec.x << vec.y << vec.z << vec.w << std::endl;
	// ^ shows the vector will be <2, 1, 0, 1> where the last 1 is w, which makes sense since it's also a position vector.

	std::string s;
	std::cin >> s;
}

