#pragma once

#include "Game.hpp"

class TextureManager{
	private:
		;
	public:
		static SDL_Texture* LoadTexture(const char* fileName);
		static void Draw(
			SDL_Texture* texture,
			SDL_Rect sourceRectangle,
			SDL_Rect destinationRectangle,
			SDL_RendererFlip flip
		);
};