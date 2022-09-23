#include "Game.hpp"
#include <iostream>

#include "Map.hpp"
#include "Constants.hpp"
#include "AssetManager.hpp"
#include "./Components/TransformComponent.hpp"
#include "./Components/SpriteComponent.hpp"
#include "./Components/KeyboardControlComponent.hpp"
#include "./Components/ColliderComponent.hpp"

EntityManager manager;
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Renderer* Game::renderer;
SDL_Event Game::event;
SDL_Rect Game::camera = {0, 0, GAEN::SCREEN::WIDTH, GAEN::SCREEN::HEIGHT};
Map* map;

Game::Game(){
	this->isRunning = false;
}

Game::~Game(){
	;
}

Entity& player(manager.AddEntity("chopper", GAEN::LAYER::LayerType::PLAYER_LAYER));

void Game::LoadLevel(int levelNumber){
	//! start including new assets to the assetmanager list
	assetManager->AddTexture("tank-image", std::string{"./assets/images/tank-big-right.png"}.c_str());
	assetManager->AddTexture("chopper-image", std::string{"./assets/images/chopper-spritesheet.png"}.c_str());
	assetManager->AddTexture("jungle-tiletexture", std::string("./assets/tilemaps/jungle.png").c_str());
	assetManager->AddTexture("radar-image", std::string("./assets/images/radar.png").c_str());
	assetManager->AddTexture("border-image", std::string("./assets/images/collision-texture.png").c_str());

	map = new Map("jungle-tiletexture", 1, 32);
	map->LoadMap("./assets/tilemaps/jungle.map", 25, 20);

	//! start including entities and also components to them
	player.AddComponent<TransformComponent>(240, 106, 0, 0, 32, 32, 1);
	player.AddComponent<SpriteComponent>("chopper-image", 2, 90, true, false);
	player.AddComponent<KeyboardControlComponent>(GAEN::KEYMAP::DEFAULT_KEYS);
	player.AddComponent<ColliderComponent>("player", 240, 106, 32, 32);

	Entity& tankEntity(manager.AddEntity("tank", GAEN::LAYER::LayerType::ENEMY_LAYER));
	tankEntity.AddComponent<TransformComponent>(150, 495, 20, 20, 32, 32, 1);
	tankEntity.AddComponent<SpriteComponent>("tank-image");
	tankEntity.AddComponent<ColliderComponent>("enemy", 150, 495, 32, 32);

  Entity& radarEntity(manager.AddEntity("radar", GAEN::LAYER::LayerType::UI_LAYER));
  radarEntity.AddComponent<TransformComponent>(GAEN::SCREEN::WIDTH - 80, 15, 0, 0, 64, 64, 1);
  radarEntity.AddComponent<SpriteComponent>("radar-image", 8, 150, false, true);
}

bool Game::IsRunning() const {
	return this->isRunning;
}

void Game::Initialize(const std::string& title, int width, int height) {
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cerr << "Error on trying to initializing SDL." << '\n';
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

	LoadLevel(0);

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
		SDL_Delay(timeToWait);
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
	std::string collisionTagType = manager.CheckEntityCollisions(player);
	if(collisionTagType.compare("enemy") == 0){
		//! TODO: do something when collision is identified as an enemy
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
	TransformComponent* mainPlayerTransform = player.GetComponent<TransformComponent>();

	camera.x = mainPlayerTransform->position.x - (GAEN::SCREEN::WIDTH / 2);
	camera.y = mainPlayerTransform->position.y - (GAEN::SCREEN::HEIGHT / 2);

	camera.x = camera.x < 0 ? 0 : camera.x;
	camera.y = camera.y < 0 ? 0 : camera.y;
	camera.x = camera.x > camera.w / 2 ? camera.w / 2 : camera.x;
	camera.y = camera.y > camera.h / 2 ? camera.h / 2 : camera.y;
}

void Game::Destroy(){
	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->window);
	SDL_Quit();
}