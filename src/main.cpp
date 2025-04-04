#include <SDL.h>
#include <iostream>
#include <utility>
#include <last-outpost/globals.h>
#include <last-outpost/game_world.h>
#include <last-outpost/level.h>
#include <last-outpost/globals.h>

namespace Game
{
	int main(int argc, char **argv)
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			std::cerr << "Error on start SDL: " << SDL_GetError() << std::endl;
			return 1;
		}

		SDL_Window *window = SDL_CreateWindow("View Map", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == nullptr)
		{
			std::cerr << "Error on Window Creation " << SDL_GetError() << std::endl;
			SDL_Quit();
			return 1;
		}

		SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer == nullptr)
		{
			std::cerr << "Error on render creation: " << SDL_GetError() << std::endl;
			SDL_DestroyWindow(window);
			SDL_Quit();
			return 1;
		}

		Map map(TILES_X, TILES_Y, rawStringMap);
		auto path = map.extractPath();

		std::vector<Enemy> enemyTypes = {Enemy(100, 20, 0.1f, "Fireball", path),
										 Enemy(150, 30, .5f, "Ice Spike", path),
										 Enemy(200, 40, 1.0f, "Lightning Bolt", path)};

		Level LEVEL_1(rawStringMap, std::move(enemyTypes), 100);
		GameWorld gameWorld(renderer, SCREEN_WIDTH, SCREEN_HEIGHT, std::move(LEVEL_1));
		gameWorld.run();

		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();

		return 0;
	}
}

//--------------------- end namespace Game ---------------------

int main(int argc, char **argv)
{
	return Game::main(argc, argv);
}