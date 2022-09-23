#pragma once

#include <map>
#include <string>

namespace GAEN {
	namespace SCREEN {
		//! pixels are 32/32
		constexpr int WIDTH = 768;
		constexpr int HEIGHT = 608;
	};

	namespace LAYER {
		enum LayerType {
      TILEMAP_LAYER    = 0,
      VEGETATION_LAYER = 1,
      ENEMY_LAYER      = 2,
      PLAYER_LAYER     = 3,
      PROJECTILE_LAYER = 4,
      UI_LAYER         = 5,
		};

		constexpr unsigned int NUM_LAYERS = 6;
	};

	constexpr std::size_t FPS = 60;
	constexpr int FRAME_TARGET_TIME = 1000.0f / FPS;

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
};