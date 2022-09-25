#pragma once

#include <map>
#include <string>

#include <SDL2/SDL.h>

namespace GAEN {

	constexpr std::size_t FPS = 60;
	constexpr int FRAME_TARGET_TIME = 1000.0f / FPS;

	namespace SCREEN {
		//! pixels are 32/32
		constexpr int WIDTH = 800;
		constexpr int HEIGHT = 600;
	};

	namespace COLLISION {
		enum CollisionType {
      NO_COLLISION                    = 0,
      PLAYER_ENEMY_COLLISION          = 1,
      PLAYER_PROJECTILE_COLLISION     = 2,
      ENEMY_PROJECTILE_COLLISION      = 3,
      PLAYER_VEGETATION_COLLIDER      = 4,
      PLAYER_LEVEL_COMPLETE_COLLISION = 5,
		};

		constexpr unsigned int NUM_COLLISIONS = 6;
	};

	namespace LAYER {
		enum LayerType {
      TILEMAP_LAYER    = 0,
      VEGETATION_LAYER = 1,
      ENEMY_LAYER      = 2,
      OBSTACLE_LAYER   = 3,
      PLAYER_LAYER     = 4,
      PROJECTILE_LAYER = 6,
      UI_LAYER         = 7,
		};

		constexpr unsigned int NUM_LAYERS = 7;
	};

	namespace KEYMAP {
		static inline std::map<std::string, std::string> DEFAULT_KEYS = {
			{"upKey",	   "up"	},
			{"downKey",	 "down"	},
			{"rightKey", "right"},
			{"leftKey",	 "left"	},
			{"shootKey", "space"},
			{"borderKey","f"},
		};
	};

	namespace FONT {
		namespace COLOR {
	    const SDL_Color WHITE = {255, 255, 255, 255};
	    const SDL_Color RED   = {255, 0, 0, 255};
	    const SDL_Color GREEN = {0, 255, 0, 255};
	    const SDL_Color BLUE  = {0, 0, 255, 255};
	  };
	};
};