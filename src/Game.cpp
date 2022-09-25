#include <iostream>

#include "Game.hpp"

#include "Map.hpp"
#include "Constants.hpp"
#include "AssetManager.hpp"
#include "./Components/TransformComponent.hpp"
#include "./Components/SpriteComponent.hpp"
#include "./Components/KeyboardControlComponent.hpp"
#include "./Components/ColliderComponent.hpp"
#include "./Components/TextLabelComponent.hpp"
#include "./Components/ProjectileEmitterComponent.hpp"

//! struct to close the lua_state as the documentation specification.
//! 	TODO: make callable on a unique_ptr lua_State and pass as second parameters
// struct closer {
// 	void operator()(lua_State* L){ lua_close(L); }
// };

EntityManager manager;
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Renderer* Game::renderer;
SDL_Event Game::event;
SDL_Rect Game::camera = {0, 0, GAEN::SCREEN::WIDTH, GAEN::SCREEN::HEIGHT};
Entity* mainPlayer = NULL;
Map* map;

Game::Game(){
	this->isRunning = false;
}

Game::~Game(){
	Destroy();
}

void Game::LoadLevel(int levelNumber){
	sol::state lua;
	lua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::math);

	std::string levelName = "Level" + std::to_string(levelNumber);
	lua.script_file("./assets/scripts/" + levelName + ".lua");

	// ---------------------------------------------
	// * LOAD ASSETS FROM LUA SCRIPT
	// ---------------------------------------------

	sol::table levelData = lua[levelName];
	sol::table levelAssets = levelData["assets"];

	unsigned int assetIndex = 0;

	while(true) {
		sol::optional<sol::table> existsAssetIndexNode = levelAssets[assetIndex];
		if(existsAssetIndexNode == sol::nullopt){
			break;
		}
		else {
			sol::table asset = levelAssets[assetIndex];
			std::string assetType = asset["type"];
			if(assetType.compare("texture") == 0) {
				std::string assetId = asset["id"];
				std::string assetFile = asset["file"];
				assetManager->AddTexture(assetId, assetFile.c_str());
			}
			if(assetType.compare("font") == 0) {
				std::string assetId = asset["id"];
				std::string assetFile = asset["file"];
				int fontSize = static_cast<int>(asset["fontSize"]);
				assetManager->AddFont(assetId, assetFile.c_str(), fontSize);
			}
		}
		++assetIndex;
	}

	//  ---------------------------------------------
	// * LOAD MAP FROM LUA SCRIPT
	//  ---------------------------------------------

	sol::table levelMap = levelData["map"];
	std::string mapTextureId = levelMap["textureAssetId"];
	std::string mapFile = levelMap["file"];

	map = new Map(
		mapTextureId,
		static_cast<int>(levelMap["scale"]),
		static_cast<int>(levelMap["tileSize"])
	);

	map->LoadMap(
		mapFile,
		static_cast<int>(levelMap["mapSizeX"]),
		static_cast<int>(levelMap["mapSizeY"])
	);

	//  ---------------------------------------------
	// * LOAD ENTITY FROM LUA SCRIPT
	//  ---------------------------------------------
	sol::table levelEntities = levelData["entities"];

	unsigned int entityIndex = 0;

	while (true) {
		sol::optional<sol::table> existsEntityIndexNode = levelEntities[entityIndex];
		if (existsEntityIndexNode == sol::nullopt) {
			break;
		}
		else {
			sol::table entity = levelEntities[entityIndex];

			std::string entityName = entity["name"];

			GAEN::LAYER::LayerType entityLayerType = static_cast<GAEN::LAYER::LayerType>( static_cast<int>(entity["layer"]) );

			//! new entity
			auto& newEntity(manager.AddEntity(entityName, entityLayerType));

			// ! add transform component
			sol::optional<sol::table> existsTransformComponent = entity["components"]["transform"];
			if (existsTransformComponent != sol::nullopt) {
				newEntity.AddComponent<TransformComponent>(
					static_cast<int>(entity["components"]["transform"]["position"]["x"]),
					static_cast<int>(entity["components"]["transform"]["position"]["y"]),
					static_cast<int>(entity["components"]["transform"]["velocity"]["x"]),
					static_cast<int>(entity["components"]["transform"]["velocity"]["y"]),
					static_cast<int>(entity["components"]["transform"]["width"]),
					static_cast<int>(entity["components"]["transform"]["height"]),
					static_cast<int>(entity["components"]["transform"]["scale"])
				);
			}

			//! add sprite component
			sol::optional<sol::table> existsSpriteComponent = entity["components"]["sprite"];
			if (existsSpriteComponent != sol::nullopt) {
				std::string textureId = entity["components"]["sprite"]["textureAssetId"];
				bool isAnimated = entity["components"]["sprite"]["animated"];
				if (isAnimated) {
					newEntity.AddComponent<SpriteComponent>(
						textureId,
						static_cast<int>(entity["components"]["sprite"]["frameCount"]),
						static_cast<int>(entity["components"]["sprite"]["animationSpeed"]),
						static_cast<bool>(entity["components"]["sprite"]["hasDirections"]),
						static_cast<bool>(entity["components"]["sprite"]["fixed"])
					);
				}
				else {
          newEntity.AddComponent<SpriteComponent>(textureId, false);
				}
			}

			//! add keyboard component
			//! 	expected a map< string PATTERN_KEY_NAME, string PATTERN_KEY_CODE >
			sol::optional<sol::table> existsInputComonent = entity["components"]["input"];
			if (existsInputComonent != sol::nullopt){
				sol::optional<sol::table> existsKeyboardInputComponent = entity["components"]["input"]["keyboard"];
				if (existsKeyboardInputComponent != sol::nullopt) {

					std::map<std::string, std::string> temp_keys;

					std::string up = entity["components"]["input"]["keyboard"]["up"];
					std::string right = entity["components"]["input"]["keyboard"]["right"];
					std::string left = entity["components"]["input"]["keyboard"]["left"];
					std::string down = entity["components"]["input"]["keyboard"]["down"];
					std::string shoot = entity["components"]["input"]["keyboard"]["shoot"];
					std::string border = entity["components"]["input"]["keyboard"]["border"];

					temp_keys["upKey"] = up;
					temp_keys["rightKey"] = right;
					temp_keys["leftKey"] = left;
					temp_keys["downKey"] = down;
					temp_keys["shootKey"] = shoot;
					temp_keys["borderKey"] = border;

					//! merge the input in default
					GAEN::KEYMAP::DEFAULT_KEYS.insert( temp_keys.begin(), temp_keys.end() );

					newEntity.AddComponent<KeyboardControlComponent>(GAEN::KEYMAP::DEFAULT_KEYS);
				}
			}

			//! add collider component
			sol::optional<sol::table> existsColliderComponent = entity["components"]["collider"];
			if (existsColliderComponent != sol::nullopt){
				std::string colliderTag = entity["components"]["collider"]["tag"];
				newEntity.AddComponent<ColliderComponent>(
					colliderTag,
					static_cast<int>(entity["components"]["transform"]["position"]["x"]),
					static_cast<int>(entity["components"]["transform"]["position"]["y"]),
					static_cast<int>(entity["components"]["transform"]["width"]),
					static_cast<int>(entity["components"]["transform"]["height"])
				);
			}

			//! add projectile component
			sol::optional<sol::table> existsProjectileEmitterComponent = entity["components"]["projectileEmitter"];
			if (existsProjectileEmitterComponent != sol::nullopt) {
				int parentEntityPosX = static_cast<int>(entity["components"]["transform"]["position"]["x"]);
				int parentEntityPosY = static_cast<int>(entity["components"]["transform"]["position"]["y"]);
				int parentEntityWidth = static_cast<int>(entity["components"]["transform"]["width"]);
				int parentEntityHeight = static_cast<int>(entity["components"]["transform"]["height"]);
				int projectileWidth = static_cast<int>(entity["components"]["projectileEmitter"]["width"]);
				int projectileHeight = static_cast<int>(entity["components"]["projectileEmitter"]["height"]);
				int projectileSpeed = static_cast<int>(entity["components"]["projectileEmitter"]["speed"]);
				int projectileRange = static_cast<int>(entity["components"]["projectileEmitter"]["range"]);
				int projectileAngle = static_cast<int>(entity["components"]["projectileEmitter"]["angle"]);
				bool projectileKeepShooting = static_cast<bool>(entity["components"]["projectileEmitter"]["keepShooting"]);

				std::string textureAssetId = entity["components"]["projectileEmitter"]["textureAssetId"];
				Entity& projectile(manager.AddEntity("projectile", GAEN::LAYER::LayerType::PROJECTILE_LAYER));

				projectile.AddComponent<TransformComponent>(
					parentEntityPosX + (parentEntityWidth / 2),
					parentEntityPosY + (parentEntityHeight / 2),
					0,
					0,
					projectileWidth,
					projectileHeight,
					1
				);

				projectile.AddComponent<SpriteComponent>(textureAssetId);

				projectile.AddComponent<ColliderComponent>(
					"PROJECTILE",
					parentEntityPosX,
					parentEntityPosY,
					projectileWidth,
					projectileHeight
				);

				projectile.AddComponent<ProjectileEmitterComponent>(
					projectileSpeed,
					projectileAngle,
					projectileRange,
					projectileKeepShooting
				);
			}

			//! add text label component
			sol::optional<sol::table> existsTextLabelComponent = entity["components"]["textLabel"];
			if (existsTextLabelComponent != sol::nullopt){
				std::string labelId = entity["name"];
				GAEN::LAYER::LayerType layerType = static_cast<GAEN::LAYER::LayerType>(entity["layer"]);
				
				int posX = static_cast<int>(entity["components"]["textLabel"]["position"]["x"]);
				int posY = static_cast<int>(entity["components"]["textLabel"]["position"]["y"]);

				std::string fontFamily = entity["components"]["textLabel"]["fontFamily"];

				std::string labelText = entity["components"]["textLabel"]["text"];

				SDL_Color color {
					static_cast<Uint8>(entity["components"]["textLabel"]["color"]["r"]), static_cast<Uint8>(entity["components"]["textLabel"]["color"]["g"]),
					static_cast<Uint8>(entity["components"]["textLabel"]["color"]["b"]), static_cast<Uint8>(entity["components"]["textLabel"]["color"]["a"])
				};

				Entity& label(manager.AddEntity(labelId, layerType));
				label.AddComponent<TextLabelComponent>(posX, posY, labelText, fontFamily, color);
			}
		}
		++entityIndex;
	}

	mainPlayer = manager.GetEntityByName("player");
}

bool Game::IsRunning() const {
	return this->isRunning;
}

void Game::Initialize(const std::string& title, int width, int height) {
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cerr << "Error on trying to initializing SDL." << '\n';
		return;
	}

	if(TTF_Init() != 0){
		std::cerr << "Error on trying to initializing SDL TTF." << '\n';
		return;
	}

	const char* windowTitle = !title.size() ? "Window" : title.c_str();
	this->window = SDL_CreateWindow(
		windowTitle, /* window title */
		SDL_WINDOWPOS_CENTERED, /* window pos x */
		SDL_WINDOWPOS_CENTERED, /* window pos y */
		width, /* window width */
		height, /* window height */
		SDL_WINDOW_BORDERLESS /* window style, 1 -> fullscreen, 2 -> border screen, borderless no border*/
	);

	if(!this->window){
		std::cerr << "Error on create SDL Window." << '\n';
		return;
	}

	this->renderer = SDL_CreateRenderer(
		window, /* window */
		-1, /* index of screen to renderer, -1 means to get default */
		0 /* no flags */
	);

	if(!this->renderer){
		std::cerr << "Error on create SDL renderer." << '\n';
		return;
	}

	LoadLevel(1);

	this->isRunning = true;
	return;
}

void Game::ProcessInput(){
	SDL_PollEvent(&event);
	switch(event.type) {
		case SDL_QUIT: {
			this->isRunning = false;
			break;
		}
		case SDL_KEYDOWN: {
			if(event.key.keysym.sym == SDLK_ESCAPE){
				this->isRunning = false;
			} else if(event.key.keysym.sym == SDLK_F5){
				this->isRunning = false;
			} else if(event.key.keysym.sym == SDLK_f){
				std::cout << " PRESSED KEY F \n";
			}
			break;
		}
		default: {
			break;
		}
	}
}

void Game::Update(){
	//! sleep the execution untill we reach the target fram time in ms
	int timeToWait = GAEN::FRAME_TARGET_TIME - (SDL_GetTicks() - this->ticksLastFrame);

	if(timeToWait > 0 && timeToWait <= GAEN::FRAME_TARGET_TIME){
		SDL_Delay(5);
	}

	//! delta time is the difference in ticks from last frame
	float deltaTime = (SDL_GetTicks() - this->ticksLastFrame) / 1000.0f;

	//! Clamp deltaTime to a max value
	deltaTime = (deltaTime > 0.05f) ? 0.0f : deltaTime;

	//! sets the new ticks for the current frame
	this->ticksLastFrame = SDL_GetTicks();

	manager.Update(deltaTime);

	HandleCameraMovement();
	CheckCollisions();
}

void Game::CheckCollisions() {
	GAEN::COLLISION::CollisionType collisionType = manager.CheckCollisions();
	if(collisionType == GAEN::COLLISION::CollisionType::PLAYER_ENEMY_COLLISION){
		//! TODO: game over?
		isRunning = false;
	}
	if(collisionType == GAEN::COLLISION::CollisionType::PLAYER_PROJECTILE_COLLISION){
		//! TODO: game over? decrease durability of helicopter?
		isRunning = false;
	}
	if(collisionType == GAEN::COLLISION::CollisionType::PLAYER_LEVEL_COMPLETE_COLLISION){
		//! TODO: process next level? end game?
		isRunning = false;
	}
}

void Game::Render(){
	SDL_SetRenderDrawColor(
		this->renderer, /* sdl renderer */
		0, /* uint8 r */
		0, /* uint8 g */
		0, /* uint8 b */
		255 /* uint8 a */
	);

	//! clear the back buffer
	SDL_RenderClear(this->renderer);

	if(manager.HasNoEntities()){
		return;
	}

	manager.Render();

	//! swap the buffer
	SDL_RenderPresent(this->renderer);
}

void Game::HandleCameraMovement(){
	if(mainPlayer){
		TransformComponent* mainPlayerTransform = mainPlayer->GetComponent<TransformComponent>();

		camera.x = mainPlayerTransform->position.x - (GAEN::SCREEN::WIDTH  / 2);
		camera.y = mainPlayerTransform->position.y - (GAEN::SCREEN::HEIGHT / 2);

    camera.x = camera.x < 0 ? 0 : camera.x;
    camera.y = camera.y < 0 ? 0 : camera.y;
    camera.x = camera.x > camera.w ? camera.w : camera.x;
    camera.y = camera.y > camera.h ? camera.h : camera.y;
	}
}

void Game::Destroy(){
	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->window);
	SDL_Quit();
}