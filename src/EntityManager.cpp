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

std::string EntityManager::CheckEntityCollisions(Entity& myEntity) const {
	ColliderComponent* myCollider = myEntity.GetComponent<ColliderComponent>();
	for(auto& entity : entities){
		if(entity->name.compare(myEntity.name) != 0 && entity->name.compare("Tile") != 0){
			if(entity->HasComponent<ColliderComponent>()){
				ColliderComponent* otherCollider = entity->GetComponent<ColliderComponent>();
				if(Collision::CheckRectangleCollision(myCollider->collider, otherCollider->collider)){
					return otherCollider->colliderTag;
				}
			}
		}
	}
	return std::string();
}