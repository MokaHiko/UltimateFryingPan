#pragma once
#include "System.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"
class SpriteRendererSystem : public System
{
public:
	void Init();
	void Update();
private:
	std::unique_ptr<SpriteRenderer> renderer;
};

