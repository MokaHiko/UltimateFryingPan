#pragma once
#include "Types.h"
#include "ComponentArray.h"
class ComponentManager
{
public:
	template<typename T>
	void RegisterComponent()
	{
		const char* typeName = typeid(T).name(); // a pointer to a character or a number we can use as signature of the type

		assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Component already registed");

		// Add this component type to the component type map
		mComponentTypes.insert({ typeName, mNextComponentType});

		// Create a component array pointer and add it to the component arrays map
		mComponentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

		++mNextComponentType; // increase the count
	}

	template<typename T>
	ComponentType GetComponentType()
	{
		const char* typeName = typeid(T).name();
		assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component types has not been registed");

		// return this component tpye - used for making the signature
		return mComponentTypes[typeName];
	}

	template<typename T>
	void AddComponent(Entity entity, T component)
	{
		// Add component to the array for an entity
		GetComponentArray<T>()->InsertData(entity, component);
	}

	template<typename T>
	void RemoveComponent(Entity entity)
	{
		// remove a component from the array for an entity;
		GetComponentArray<T>()->RemoveData(entity);
	}

	template<typename T>
	T& GetComponent(Entity entity)
	{
		return GetComponentArray<T>()->GetData(entity);
	}

	void EntityDestroyed(Entity entity)
	{
		for (auto const& pair : mComponentArrays)
		{
			auto const& component = pair.second;
			component->EntityDestroyed(entity);
		}
	}

private:
	// Map fron type string pointer to a component type
	std::unordered_map<const char*, ComponentType> mComponentTypes{};

	// Map from type string pointer toa  component array
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> mComponentArrays{};

	// The component type to be assigned to the nex registed component;
	ComponentType mNextComponentType{};

	// helper function to get the statically casted pointer to the ComponentArray of type T
	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray()
	{
		const char* typeName = typeid(T).name();

		assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component type does not exist");
		return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
	}
};

