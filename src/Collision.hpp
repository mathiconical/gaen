#pragma once

#include <SDL2/SDL.h>

class Collision {
	private:
		;
	public:
		static bool CheckRectangleCollision(const SDL_Rect& rectangleA, const SDL_Rect& rectangleB);
};