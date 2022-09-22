#pragma once

#include <iostream>

namespace GAEN {
	namespace SCREEN {
		//! pixels are 32/32
		constexpr int WIDTH = 768;
		constexpr int HEIGHT = 608;
	};

	constexpr std::size_t FPS = 60;
	constexpr int FRAME_TARGET_TIME = 1000.0f / FPS;
};