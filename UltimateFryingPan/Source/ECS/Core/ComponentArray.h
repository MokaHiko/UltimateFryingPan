#pragma once
#include "Types.h"

class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray
{
public:
	void InsertData(Entity entity, T component)
	{
		assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() && "Cannot have the same entity twice");

		// put the new entry and the end and update the maps
		size_t newIndex = mSize;
		mEntityToIndexMap[entity] = newIndex;
		mIndexToEntityMap[newIndex] = entity;
		mComponentArray[newIndex] = component;
		++mSize;
	}
	void RemoveData(Entity entity)
	{
		assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Entity does not have component");
		
		// copy element at the end to the index of the element to be removed
		size_t indexOfRemovedEntity = mEntityToIndexMap[entity];
		size_t indexOfLastElement = mEntityToIndexMap[mSize] - 1;
		mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

		// update the maps to point to moved spot
		Entity entityofLastElement = mIndexToEntityMap[indexOfLastElement];
		mEntityToIndexMap[entityofLastElement] = indexOfRemovedEntity;
		mIndexToEntityMap[indexOfRemovedEntity] = entityofLastElement;

		// erase data of removed element
		mEntityToIndexMap.erase(entity);
		mIndexToEntityMap.erase(indexOfLastElement);
		--mSize;
	}
	T& GetData(Entity entity)
	{
		assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Entity does not have component");

		return mComponentArray[mEntityToIndexMap[entity]];
	}

	void EntityDestroyed(Entity entity) override
	{
		if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Entity does not have component")
		{
			RemoveData(entity);
		}
	}
private:
	// a packed array of components (of generic type T)
	// sized to the max number of entities allowed;
	std::array<T, MAX_ENTITIES> mComponentArray;

	// Map from an entity id to its index in the component array
	std::unordered_map<Entity, size_t> mEntityToIndexMap;

	// Map from index to entity id
	std::unordered_map<size_t, Entity> mIndexToEntityMap;

	// total size of valid entries in the array
	size_t mSize;
};

