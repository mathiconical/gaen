#include "TextureManager.hpp"

SDL_Texture* TextureManager::LoadTexture(const char* fileName){
	SDL_Surface* surface = IMG_Load(fileName);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}

void TextureManager::Draw(
		SDL_Texture* texture, /* texture */
		SDL_Rect sourceRectangle, /* src rect */
		SDL_Rect destinationRectangle, /* dest rect */
		SDL_RendererFlip flip /* flip */
	){
	SDL_RenderCopyEx(
		Game::renderer, /* renderer */
		texture, /* texture */
		&sourceRectangle, /* source rect */
		&destinationRectangle, /* destination rect */
		0.0, /* angle */
		NULL, /* center */
		flip /* flip */
	);
}