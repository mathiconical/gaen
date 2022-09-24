#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../Component.hpp"

#include "../FontManager.hpp"
#include "../EntityManager.hpp"
#include "../AssetManager.hpp"
#include "../Game.hpp"

class TextLabelComponent : public Component {
	private:
		SDL_Rect position;
		SDL_Color color;
		SDL_Texture* texture;

		std::string text;
		std::string fontFamily;

	public:
		TextLabelComponent(int x, int y, std::string text, std::string fontFamily, const SDL_Color& color) {
			this->color = color;
			this->text = text;
			this->fontFamily = fontFamily;
			position.x = x;
			position.y = y;
			SetLabelText(text, fontFamily);
		}

		void SetLabelText(std::string text, std::string fontFamily){
			SDL_Surface* surface = TTF_RenderText_Blended(
				Game::assetManager->GetFont(fontFamily), /* font */
				text.c_str(), /* text */
				color /* color */
			);

			texture = SDL_CreateTextureFromSurface(Game::renderer, surface);

			SDL_FreeSurface(surface);

			SDL_QueryTexture(
				texture, /* texture */
				NULL, /* format Uint32 */
				NULL, /* access */
				&position.w, /* width */
				&position.h /* height */
			);
		}

		void Render() override {
			FontManager::Draw(texture, position);
		}
};