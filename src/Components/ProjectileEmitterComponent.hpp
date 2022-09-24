#pragma once

#include <glm/glm.hpp>

#include "../EntityManager.hpp"
#include "TransformComponent.hpp"

class ProjectileEmitterComponent : public Component {
	private:
		TransformComponent* transform;
		glm::vec2 origin;
		int speed;
		int range;
		float angleRad;
		bool keepShooting;

	public:
		ProjectileEmitterComponent(int speed, float angleDeg, int range, bool keepShooting)
			: speed(speed), range(range), keepShooting(keepShooting)
			{
				angleRad = glm::radians(static_cast<float>(angleDeg));
			}

		void Initialize() override {
			transform = owner->GetComponent<TransformComponent>();
			origin = glm::vec2(transform->position.x, transform->position.y);

			//! polar coord convertion
			transform->velocity = glm::vec2(glm::cos(angleRad) * speed, glm::sin(angleRad) * speed);
		}

		void Update(float deltaTime) override {
			//! check the distance between origin and current position of projectile
			if(glm::distance(transform->position, origin) > range){
				if(keepShooting){ /* reset projectile to the origin and keep the cycle */
					transform->position.x = origin.x;
					transform->position.y = origin.y;
				}
				else { /* destroy the projectile */
					owner->Destroy();
				}
			}
		}
};