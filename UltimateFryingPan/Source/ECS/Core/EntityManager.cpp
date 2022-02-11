#include "pch.h"
#include "EntityManager.h"

EntityManager::EntityManager()
{
	for (Entity entity = 0; entity < MAX_ENTITIES; entity++)
	{
		mAvailableEntites.push(entity);
	}
}

Entity EntityManager::CreateEntity()
{
	assert(mLivingEntityCount < MAX_ENTITIES && "Reached the limmit of entities");

	// take an id from avaialble queue first-in-first-out
	Entity id = mAvailableEntites.front();
	mAvailableEntites.pop();
	++mLivingEntityCount;

	return id;
}

void EntityManager::DestroyEntity(Entity entity)
{
	assert(entity < MAX_ENTITIES && "Entity out of range");

	// clear the signature of entity
	mSignatures[entity].reset();

	// place entity id to the back of the queue
	mAvailableEntites.push(entity);
	mLivingEntityCount--;
}

void EntityManager::SetSignature(Entity entity, Signature signature)
{
	assert(entity < MAX_ENTITIES && "Entity id out of range");

	// put this entitys signature into the array
	mSignatures[entity] = signature;
}

Signature EntityManager::GetSignature(Entity entity)
{
	return mSignatures[entity];
}
