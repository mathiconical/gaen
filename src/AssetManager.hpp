#pragma once

#include <map>
#include <string>

#include <SDL2/SDL_ttf.h>

#include "TextureManager.hpp"
#include "FontManager.hpp"
#include "EntityManager.hpp"
#include "FontManager.hpp"

class AssetManager{
	private:
		EntityManager* manager;
		std::map<std::string, SDL_Texture*> textures;
		std::map<std::string, TTF_Font*> fonts;
	public:
		AssetManager(EntityManager* manager);
		~AssetManager();
		void ClearData();
		
		void AddTexture(const std::string& textureId, const char* filePath);
		void AddFont(std::string fontId, const char* filePath, int fontSize);

		SDL_Texture* GetTexture(const std::string& textureId);
		TTF_Font* GetFont(std::string fontId);
};