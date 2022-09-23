#include <fstream>

#include "Map.hpp"
#include "Game.hpp"
#include "EntityManager.hpp"
#include "Components/TileComponent.hpp"
#include "Constants.hpp"

extern EntityManager manager;

Map::Map(std::string textureId, int scale, int tileSize)
	: textureId(textureId), scale(scale), tileSize(tileSize) {
}

Map::~Map(){
	;
}

void Map::LoadMap(std::string filePath, int mapSizeX, int mapSizeY){
	std::fstream mapFile;
	mapFile.open(filePath);

	for(int y = 0; y < mapSizeY; ++y){
		for(int x = 0; x < mapSizeX; ++x){
			char ch;
			mapFile.get(ch);
			int sourceRectY = atoi(&ch) * tileSize;
			mapFile.get(ch);
			int sourceRectX = atoi(&ch) * tileSize;
			AddTile(sourceRectX, sourceRectY, x * (scale * tileSize), y * (scale * tileSize));
			mapFile.ignore();
		}
	}
	mapFile.close();
}

void Map::AddTile(int sourceRectX, int sourceRectY, int x, int y) {
	Entity& newTile(manager.AddEntity("Tile", GAEN::LAYER::LayerType::TILEMAP_LAYER));
	newTile.AddComponent<TileComponent>(sourceRectX, sourceRectY, x, y, tileSize, scale, textureId);
}