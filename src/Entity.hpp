#pragma once

#include <vector>
#include <string>
#include <map>
#include <iostream>

#include "EntityManager.hpp"
#include "Component.hpp"
#include "Constants.hpp"

class Component;
class EntityManager;

class Entity{
	private:
		EntityManager& manager;
		bool isActive;
		std::vector<Component*> components;
		std::map<const std::type_info*, Component*> componentTypeMap;
	public:
		std::string name;
		GAEN::LAYER::LayerType layer;
		Entity(EntityManager& manager);
		Entity(EntityManager& manager, const std::string& name, GAEN::LAYER::LayerType layer);
		void Update(float deltaTime);
		void Render();
		void Destroy();
		bool IsActive() const;
		void ListAllComponents() const;

		template <typename T, typename... TArgs>
		T& AddComponent(TArgs&&... args){
			T* newComponent(new T(std::forward<TArgs>(args)...));
			newComponent->owner = this;
			components.emplace_back(newComponent);
			componentTypeMap[&typeid(*newComponent)] = newComponent;
			newComponent->Initialize();
			return *newComponent;
		}

		template <typename T>
		bool HasComponent() const {
			return componentTypeMap.count(&typeid(T));
		}

		template <typename T>
		T* GetComponent(){
			return static_cast<T*>(componentTypeMap[&typeid(T)]);
		}
};