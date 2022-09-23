#include "AssetManager.hpp"

AssetManager::AssetManager(EntityManager* manager): manager(manager) {
}

void AssetManager::ClearData(){
	textures.clear();
}

void AssetManager::AddTexture(const std::string& textureId, const char* filePath){
	textures.emplace(textureId, TextureManager::LoadTexture(filePath));
}

SDL_Texture* AssetManager::GetTexture(const std::string& textureId){
	return textures[textureId];
}