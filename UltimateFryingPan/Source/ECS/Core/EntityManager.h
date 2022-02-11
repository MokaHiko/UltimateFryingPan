#pragma once
#include "Types.h"

class EntityManager
{
public:
	EntityManager();
	Entity CreateEntity();

	void DestroyEntity(Entity entity);
	void SetSignature(Entity entity, Signature signature);

	Signature GetSignature(Entity entity);
private:
	// Queue of unused entity IDS
	std::queue<Entity> mAvailableEntites{};

	// Array of signature where the index corresponds to the entityID
	std::array<Signature, MAX_ENTITIES> mSignatures{};

	// Total living entities - used to keep limit on how many exist at a time
	uint32_t mLivingEntityCount{};

};

