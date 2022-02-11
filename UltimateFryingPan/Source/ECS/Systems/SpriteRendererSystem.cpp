#include "pch.h"
#include "SpriteRendererSystem.h"
#include "Coordinator.h"

// Required Components
#include "RenderableSprite.h"
#include "Transform.h"

extern Coordinator gCoordinator;
void SpriteRendererSystem::Init()
{
	renderer = std::make_unique<SpriteRenderer>(ResourceManager::GetShader("sprite"));
}

void SpriteRendererSystem::Update()
{
	for (auto const& entity : mEntities)
	{
		auto const& transform = gCoordinator.GetComponent<Transform>(entity);
		auto const& renderable = gCoordinator.GetComponent<RenderableSprite>(entity);
		renderer->DrawSprite(renderable.texture, transform.position, transform.scale, transform.rotation.z, renderable.color);
	}
}
