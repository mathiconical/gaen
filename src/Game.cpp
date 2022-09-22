#include <iostream>

#include "Game.hpp"
#include "Constants.hpp"

SDL_Renderer* Game::renderer;
SDL_Event Game::event;

Game::Game(){
	this->isRunning = false;
}

Game::~Game(){
	;
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

	//! swap the buffer
	SDL_RenderPresent(this->renderer);
}

void Game::Destroy(){
	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->window);
	SDL_Quit();
}