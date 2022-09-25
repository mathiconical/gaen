#pragma once

#include <string>
#include <map>

#include "../Game.hpp"
#include "../Map.hpp"
#include "../EntityManager.hpp"
#include "../Constants.hpp"
#include "TransformComponent.hpp"
#include "SpriteComponent.hpp"

class KeyboardControlComponent : public Component {
	private:
	public:
		std::map<std::string, std::string> keyMap;
		TransformComponent* transform;
		SpriteComponent* sprite;

		std::map<std::string, std::string> KEYS = {
			{"up",		"1073741906"},
			{"down",	"1073741905"},
			{"right",	"1073741903"},
			{"left",	"1073741904"},
			{"space",	"32"},
			{"f",     "102"},
		};

		KeyboardControlComponent(){
			keyMap = {
				{"upKey",		  GetSDLKeyStringCode(KEYS["up"	])},
				{"downKey",		GetSDLKeyStringCode(KEYS["down" ])},
				{"rightKey",	GetSDLKeyStringCode(KEYS["right"])},
				{"leftKey",		GetSDLKeyStringCode(KEYS["left" ])},
				{"shootKey",	GetSDLKeyStringCode(KEYS["space"])},
				{"borderKey",	GetSDLKeyStringCode(KEYS["f"])},
			};
		}

		KeyboardControlComponent(std::map<std::string, std::string>& keys){
			keyMap = {
				{"upKey",		  GetSDLKeyStringCode(keys["upKey"   ])},
				{"downKey",		GetSDLKeyStringCode(keys["downKey" ])},
				{"rightKey",	GetSDLKeyStringCode(keys["rightKey"])},
				{"leftKey",		GetSDLKeyStringCode(keys["leftKey" ])},
				{"shootKey",	GetSDLKeyStringCode(keys["shootKey"])},
				{"borderKey",	GetSDLKeyStringCode(keys["borderKey"])},
			};
		}

		std::string GetSDLKeyStringCode(const std::string& key) {
			return (KEYS.count(key)) ? KEYS[key] : std::to_string(static_cast<int>(key[0]));
		}

		void Initialize() override {
			transform = owner->GetComponent<TransformComponent>();
			sprite = owner->GetComponent<SpriteComponent>();
		}

		void Update(float deltaTime) override {
			//! Gather all events on the stack, and update keyboard state
			// SDL_PumpEvents();

			// const Uint8* state = SDL_GetKeyboardState(NULL);

			// if(state[SDL_SCANCODE_Q]){
			// 	std::cout << "Q PRESSED\n";
			// }

			if(Game::event.type == SDL_KEYDOWN){
				std::string keyCode = std::to_string(Game::event.key.keysym.sym);

				if(keyMap["upKey"].compare(keyCode) == 0){
					if(transform->position.y + transform->height <= 0){
						transform->position.y = 0;
						return;
					}
					transform->velocity.y = -200;
					transform->velocity.x = 0;
					sprite->Play("UpAnimation");
				}

				if(keyMap["downKey"].compare(keyCode) == 0){
					if(transform->position.y + transform->height >= GAEN::SCREEN::HEIGHT * Map::e_scale){
						transform->position.y = GAEN::SCREEN::HEIGHT * Map::e_scale - transform->height;
						return;
					}
					transform->velocity.y = 200;
					transform->velocity.x = 0;
					sprite->Play("DownAnimation");
				}

				if(keyMap["leftKey"].compare(keyCode) == 0){
					if(transform->position.x <= 0){
						transform->position.x = 2;
						return;
					}
					transform->velocity.y = 0;
					transform->velocity.x = -200;
					sprite->Play("LeftAnimation");
				}

				if(keyMap["rightKey"].compare(keyCode) == 0){
					if(transform->position.x + transform->width >= GAEN::SCREEN::WIDTH * Map::e_scale){
						transform->position.x = GAEN::SCREEN::WIDTH * Map::e_scale - transform->height;
						return;
					}
					transform->velocity.y = 0;
					transform->velocity.x = 200;
					sprite->Play("RightAnimation");
				}

				if(keyMap["shoot"].compare(keyCode) == 0){
					//! TODO: shoot projectiles when pressed shoot key
				}
			}
			if(Game::event.type == SDL_KEYUP){
				std::string keyCode = std::to_string(Game::event.key.keysym.sym);

				if(keyMap["upKey"].compare(keyCode) == 0){
					transform->velocity.y = 0;
				}

				if(keyMap["downKey"].compare(keyCode) == 0){
					transform->velocity.y = 0;
				}

				if(keyMap["leftKey"].compare(keyCode) == 0){
					transform->velocity.x = 0;
				}

				if(keyMap["rightKey"].compare(keyCode) == 0){
					transform->velocity.x = 0;
				}
			}
		}
};