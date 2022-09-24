#include "AssetManager.hpp"
#include "Components/TransformComponent.hpp"

AssetManager::AssetManager(EntityManager* manager): manager(manager) {
}

void AssetManager::ClearData(){
	textures.clear();
	fonts.clear();
}

void AssetManager::AddTexture(const std::string& textureId, const char* filePath){
	textures.emplace(textureId, TextureManager::LoadTexture(filePath));
}

void AssetManager::AddFont(std::string fontId, const char* filePath, int fontSize){
	fonts.emplace( fontId, FontManager::LoadFont(filePath, fontSize) );
}

SDL_Texture* AssetManager::GetTexture(const std::string& textureId){
	return textures[textureId];
}

TTF_Font* AssetManager::GetFont(std::string fontId){
	return fonts[fontId];
}