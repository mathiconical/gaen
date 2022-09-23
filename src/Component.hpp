#pragma once

#include "Constants.hpp"

class Entity;

class Component{
	private:
		;
	public:
		Entity* owner;
		virtual ~Component() {};
		virtual void Initialize() {};
		virtual void Update(float deltaTime) {};
		virtual void Render() {};
};