#pragma once

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <glm/glm.hpp>



class Game{
	private:
		bool isRunning;
		SDL_Window* window;
	public:
		static SDL_Renderer* renderer;
		static SDL_Event event;
		bool IsRunning() const;
		Game();
		int ticksLastFrame = {0};
		~Game();
		void Initialize(const std::string& title, int width, int height);
		void ProcessInput();
		void Update();
		void Render();
		void Destroy();
};