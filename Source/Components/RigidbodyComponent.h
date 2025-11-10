#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include <glm/glm.hpp>

struct RigidbodyComponent
{
	glm::vec2 velocity;

	RigidbodyComponent(glm::vec2 velocity = glm::vec2(0.0f, 0.0f))
	{
		this->velocity = velocity;
	}
};

#endif