#pragma once

#include <vector>
#include <limits>
#include <functional>

#include "glm/glm.hpp"


struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 textco;
	glm::vec3 normal;
};
