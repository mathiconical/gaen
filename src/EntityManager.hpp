#pragma once

#include <vector>
#include <iostream>

#include "Entity.hpp"
#include "Component.hpp"
#include "Constants.hpp"

class EntityManager{
	private:
		std::vector<Entity*> entities;
	public:
		void ClearData();
		void Update(float deltaTime);
		void Render();
		void DestroyInactiveEntities();
		void ListAllEntities() const;
		bool HasNoEntities() const;
		Entity& AddEntity(const std::string& entityName, GAEN::LAYER::LayerType layer);
		std::vector<Entity*> GetEntitiesByLayer(GAEN::LAYER::LayerType layer) const;
		std::vector<Entity*> GetEntities() const;
		std::size_t GetEntityCount();
		GAEN::COLLISION::CollisionType CheckCollisions() const;
};