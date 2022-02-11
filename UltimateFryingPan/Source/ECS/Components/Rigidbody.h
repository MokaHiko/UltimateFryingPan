#pragma once
struct Rigidbody
{
	glm::vec3 velocity;
	glm::vec3 acceleration;
	Rigidbody(glm::vec3 _velocity = glm::vec3(0.0f), glm::vec3 _acceleration = glm::vec3(0.0f))
		:velocity(_velocity), acceleration(_acceleration){ }
};

