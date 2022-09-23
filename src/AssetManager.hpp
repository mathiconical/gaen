#pragma once

#include <map>
#include <string>

#include "TextureManager.hpp"
#include "EntityManager.hpp"

class AssetManager{
	private:
		EntityManager* manager;
		std::map<std::string, SDL_Texture*> textures;
	public:
		AssetManager(EntityManager* manager);
		~AssetManager();
		void ClearData();
		void AddTexture(const std::string& textureId, const char* filePath);
		SDL_Texture* GetTexture(const std::string& textureId);
};