#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <map>

#include "TransformComponent.hpp"
#include "../TextureManager.hpp"
#include "../AssetManager.hpp"
#include "../Animation.hpp"
#include "../Constants.hpp"

class SpriteComponent : public Component {
	private:
		TransformComponent* transform;
		SDL_Texture* texture;
		SDL_Rect sourceRectangle;
		SDL_Rect destinationRectangle;
		bool isAnimated;
		unsigned int numFrames;
		unsigned int animationSpeed;
		bool isFixed;
		std::map<std::string, Animation> animations;
		std::string currentAnimationName;
		unsigned int animationIndex = 0;
		std::map<std::string, SDL_Color> stateColors;
	public:
		SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

		SpriteComponent(const std::string& assetTextureId)
			: isAnimated(false), isFixed(false) {
			SetTexture(assetTextureId);
		}

		SpriteComponent(const std::string& assetTextureId, bool isFixed)
			: isAnimated(false), isFixed(isFixed) {
			SetTexture(assetTextureId);
		}

		SpriteComponent(const std::string& assetTextureId, unsigned int numFrames, unsigned int animationSpeed, bool hasDirections, bool isFixed)
			: numFrames(numFrames), animationSpeed(animationSpeed), isFixed(isFixed){
				isAnimated = true;

				if(hasDirections){
					Animation downAnimation = Animation(0, numFrames, animationSpeed);
					Animation rightAnimation = Animation(1, numFrames, animationSpeed);
					Animation leftAnimation = Animation(2, numFrames, animationSpeed);
					Animation upAnimation = Animation(3, numFrames, animationSpeed);

					animations.emplace("DownAnimation", downAnimation);
					animations.emplace("RightAnimation", rightAnimation);
					animations.emplace("LeftAnimation", leftAnimation);
					animations.emplace("UpAnimation", upAnimation);

					animationIndex = 0;
					currentAnimationName = "DownAnimation";
				} else {
					Animation singleAnimation = Animation(0, numFrames, animationSpeed);
					animations.emplace("singleAnimation", singleAnimation);
					animationIndex = 0;
					currentAnimationName = "singleAnimation";
				}

				Play(currentAnimationName);

				SetTexture(assetTextureId);
			}

		void Play(const std::string& animationName){
			numFrames = animations[animationName].numFrames;
			animationIndex = animations[animationName].index;
			animationSpeed = animations[animationName].animationSpeed;
			currentAnimationName = animationName;
		}

		void Play(const std::string& animationName, unsigned int increaseAnimationSpeed){
			numFrames = animations[animationName].numFrames;
			animationIndex = animations[animationName].index;
			animationSpeed = animations[animationName].animationSpeed + increaseAnimationSpeed;
			currentAnimationName = animationName;
		}

		void SetTexture(std::string assetTextureId){
			texture = Game::assetManager->GetTexture(assetTextureId);
		}

		void Initialize() override {
			transform = owner->GetComponent<TransformComponent>();
			sourceRectangle.x = 0;
			sourceRectangle.y = 0;
			sourceRectangle.w = transform->width;
			sourceRectangle.h = transform->height;
		}

		void Update(float deltaTime) override {
			if(isAnimated){
				sourceRectangle.x = sourceRectangle.w * static_cast<int>((SDL_GetTicks() / animationSpeed) % numFrames);
			}

			sourceRectangle.y = animationIndex * transform->height;

			destinationRectangle.x = static_cast<int>(transform->position.x) - (isFixed ? 0 : Game::camera.x);
			destinationRectangle.y = static_cast<int>(transform->position.y) - (isFixed ? 0 : Game::camera.y);
			destinationRectangle.w = transform->width * transform->scale;
			destinationRectangle.h = transform->height * transform->scale;
		}

		void Render() override {
			TextureManager::Draw(texture, sourceRectangle, destinationRectangle, spriteFlip);
		}
};