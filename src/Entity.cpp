#include "Entity.hpp"

Entity::Entity(EntityManager& manager)
	: manager(manager) {
	isActive = true;
}

Entity::Entity(EntityManager& manager, const std::string& name, GAEN::LAYER::LayerType layer)
	: manager(manager), name(name), layer(layer) {
	isActive = true;
}

bool Entity::IsActive() const {
	return this->isActive;
}

void Entity::Update(float deltaTime){
	for(auto& component : components){
		component->Update(deltaTime);
	}
}

void Entity::Render(){
	for(auto& component : components){
		component->Render();
	}
}

void Entity::Destroy(){
	isActive = false;
}

void Entity::ListAllComponents() const {
	for(auto& [key, value] : componentTypeMap){
		std::cout << "  Component<" << key->name() << '\n';
	}
}