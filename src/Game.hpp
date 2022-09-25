#pragma once

#include <iostream>

#include <LUA/sol.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <glm/glm.hpp>

#include "EntityManager.hpp"
#include "Component.hpp"
#include "Entity.hpp"
#include "EntityManager.hpp"

// #include "./Components/TransformComponent.hpp"

class AssetManager;

class Game{
	private:
		bool isRunning;
		SDL_Window* window;
	public:
		static SDL_Renderer* renderer;
		static AssetManager* assetManager;
		static SDL_Event event;
		static SDL_Rect camera;
		void LoadLevel(int levelNumber);
		int ticksLastFrame = {0};
		bool IsRunning() const;
		Game();
		~Game();
		void Initialize(const std::string& title, int width, int height);
		void ProcessInput();
		void Update();
		void Render();
		void Destroy();
		void HandleCameraMovement();
		void CheckCollisions();
};