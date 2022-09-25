#include "EntityManager.hpp"
#include "Collision.hpp"
#include "Components/ColliderComponent.hpp"

void EntityManager::ClearData(){
	for(auto& entity : entities){
		entity->Destroy();
	}
}

bool EntityManager::HasNoEntities() const {
	return entities.size() == 0;
}

void EntityManager::Update(float deltaTime){
	for(auto& entity : entities){
		entity->Update(deltaTime);
	}
	DestroyInactiveEntities();
}

void EntityManager::DestroyInactiveEntities(){
	for(std::size_t i = 0; i < entities.size(); ++i){
		if(!entities[i]->IsActive()){
			entities.erase(entities.begin() + i);
		}
	}
}

void EntityManager::Render() {
	for(unsigned int layerNumber = {0}; layerNumber < GAEN::LAYER::NUM_LAYERS; ++layerNumber){
		for(auto& entity : GetEntitiesByLayer(static_cast<GAEN::LAYER::LayerType>(layerNumber))){
			entity->Render();
		}
	}
}

Entity& EntityManager::AddEntity(const std::string& entityName, GAEN::LAYER::LayerType layer){
	Entity* entity = new Entity(*this, entityName, layer);
	entities.emplace_back(entity);
	return *entity;
}

std::vector<Entity*> EntityManager::GetEntitiesByLayer(GAEN::LAYER::LayerType layer) const {
	std::vector<Entity*> selectedEntities;
	for(auto& entity : entities){
		if(entity->layer == layer){
			selectedEntities.emplace_back(entity);
		}
	}
	return selectedEntities;
}

Entity* EntityManager::GetEntityByName(std::string name) const {
	for(auto* entity : entities)
		if (entity->name.compare(name) == 0)
			return entity;
	return NULL;
}

std::vector<Entity*> EntityManager::GetEntities() const {
	return entities;
}

std::size_t EntityManager::GetEntityCount() {
	return entities.size();
}

void EntityManager::ListAllEntities() const {
	std::size_t i = {0};
	for(auto& entity : entities){
		std::cout << "Entity[" << i << "]: " << entity->name << '\n';
		entity->ListAllComponents();
		++i;
	}
}

GAEN::COLLISION::CollisionType EntityManager::CheckCollisions() const {
	for (std::size_t i = 0; i < entities.size() - 1; ++i) {
		auto& thisEntity = entities[i];
		if (thisEntity->HasComponent<ColliderComponent>()) {
			ColliderComponent* thisCollider = thisEntity->GetComponent<ColliderComponent>();
			for (std::size_t j = i + 1; j < entities.size(); ++j) {
				auto& thatEntity = entities[j];
				if (thisEntity->name.compare(thatEntity->name) != 0 && thatEntity->HasComponent<ColliderComponent>()) {
					ColliderComponent* thatCollider = thatEntity->GetComponent<ColliderComponent>();
					if (Collision::CheckRectangleCollision(thisCollider->collider, thatCollider->collider)) {
						if (thisCollider->colliderTag.compare("PLAYER") == 0 && thatCollider->colliderTag.compare("ENEMY") == 0) {
							return GAEN::COLLISION::CollisionType::PLAYER_ENEMY_COLLISION;
						}
						if (thisCollider->colliderTag.compare("PLAYER") == 0 && thatCollider->colliderTag.compare("PROJECTILE") == 0) {
							return GAEN::COLLISION::CollisionType::PLAYER_PROJECTILE_COLLISION;
						}
						if (thisCollider->colliderTag.compare("ENEMY") == 0 && thatCollider->colliderTag.compare("FRIENDLY_PROJECTILE") == 0) {
							return GAEN::COLLISION::CollisionType::ENEMY_PROJECTILE_COLLISION;
						}
						if (thisCollider->colliderTag.compare("PLAYER") == 0 && thatCollider->colliderTag.compare("LEVEL_COMPLETE") == 0) {
							return GAEN::COLLISION::CollisionType::PLAYER_LEVEL_COMPLETE_COLLISION;
						}
					}
				}
			}
		}
	}
	return GAEN::COLLISION::CollisionType::NO_COLLISION;
}