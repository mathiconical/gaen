#include "Constants.hpp"
#include "Game.hpp"

int main(int argc, char* args[]){
	Game game;

	game.Initialize("Window", GAEN::SCREEN::WIDTH, GAEN::SCREEN::HEIGHT);

	while(game.IsRunning()){
		game.ProcessInput();
		game.Update();
		game.Render();
	}

	game.Destroy();

	return 0;
}