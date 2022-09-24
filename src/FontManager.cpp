#include "FontManager.hpp"

TTF_Font* FontManager::LoadFont(const char* fileName, int fontSize){
	return TTF_OpenFont(fileName, fontSize);
}

void FontManager::Draw(SDL_Texture* texture, SDL_Rect position){
	//! no extra params, no need to use copyEx
	SDL_RenderCopy(
		Game::renderer, /* renderer */
		texture, /* texture */
		NULL, /* source rectangle */
		&position /* destination rectangle */
	);
}