#include "pch.h"
#include "PhysicsSystem.h"
#include "Coordinator.cpp"

// Components
#include "Transform.h"
#include "Rigidbody.h"
#include "Gravity.h"

extern Coordinator gCoordinator;

void PhysicsSystem::Init()
{
}

void PhysicsSystem::Update(float dt)
{
	for (auto const& entity : mEntities)
	{
		auto& rigidBody = gCoordinator.GetComponent<Rigidbody>(entity);
		auto& transform = gCoordinator.GetComponent<Transform>(entity);

		// Forces
		auto const& gravity = gCoordinator.GetComponent<Gravity>(entity);

		transform.position += rigidBody.velocity * dt;
		rigidBody.velocity += gravity.force * dt;
	}
}
