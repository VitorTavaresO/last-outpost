#include <last-outpost/game_control.h>
#include <last-outpost/globals.h>
#include <last-outpost/map.h>
#include <iostream>
#include <SDL_image.h>

namespace Game
{
	GameControl::GameControl()
		: window(nullptr), renderer(nullptr), gameWorld(nullptr),
		  currentState(GameState::MainMenu), currentLevelIndex(0)
	{
	}

	GameControl::~GameControl()
	{
		cleanup();
	}

	bool GameControl::initialize()
	{
		if (!initializeSDL())
			return false;

		createLevels();
		return true;
	}

	void GameControl::run()
	{
		while (currentState != GameState::Quitting)
		{
			switch (currentState)
			{
			case GameState::MainMenu:
				handleMainMenu();
				break;
			case GameState::Playing:
				handleGamePlay();
				break;
			case GameState::Paused:
				handlePauseMenu();
				break;
			case GameState::LevelComplete:
				handleLevelComplete();
				break;
			case GameState::GameOver:
				handleGameOver();
				break;
			default:
				break;
			}
		}
	}

	void GameControl::cleanup()
	{
		if (renderer)
		{
			SDL_DestroyRenderer(renderer);
			renderer = nullptr;
		}

		if (window)
		{
			SDL_DestroyWindow(window);
			window = nullptr;
		}

		IMG_Quit();
		SDL_Quit();
	}

	bool GameControl::initializeSDL()
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		{
			std::cerr << "Error on start SDL: " << SDL_GetError() << std::endl;
			return false;
		}

		if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
		{
			std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
			return false;
		}

		window = SDL_CreateWindow("Last Outpost",
								  SDL_WINDOWPOS_UNDEFINED,
								  SDL_WINDOWPOS_UNDEFINED,
								  SCREEN_WIDTH, SCREEN_HEIGHT,
								  SDL_WINDOW_SHOWN);

		if (window == nullptr)
		{
			std::cerr << "Error on Window Creation " << SDL_GetError() << std::endl;
			SDL_Quit();
			return false;
		}

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer == nullptr)
		{
			std::cerr << "Error on render creation: " << SDL_GetError() << std::endl;
			SDL_DestroyWindow(window);
			SDL_Quit();
			return false;
		}

		return true;
	}

	void GameControl::createLevels()
	{
		Map map(TILES_X, TILES_Y, rawStringMap);
		auto path = map.extractPath();

		std::vector<Enemy> enemyTypes;
		enemyTypes.reserve(3);

		enemyTypes.emplace_back(100, 20, 5.0f, "Fireball", path);
		enemyTypes.emplace_back(150, 30, 0.8f, "Ice Spike", path);
		enemyTypes.emplace_back(200, 40, 0.5f, "Lightning Bolt", path);

		levels.emplace_back(rawStringMap, std::move(enemyTypes), 20);
	}

	void GameControl::handleMainMenu()
	{
		changeState(GameState::Playing);
	}

	void GameControl::handleGamePlay()
	{
		if (!gameWorld)
		{
			gameWorld = std::make_unique<GameWorld>(
				renderer,
				SCREEN_WIDTH, SCREEN_HEIGHT,
				std::move(getCurrentLevel()));
		}

		bool gameCompleted = gameWorld->run();

		if (!gameCompleted)
		{
			changeState(GameState::Quitting);
			return;
		}

		/*if (gameCompleted)
		{
			gameWorld.reset();

			if (++currentLevelIndex < static_cast<int>(levels.size()))
			{
				changeState(GameState::LevelComplete);
			}
			else
			{
				changeState(GameState::GameOver);
			}
		}*/
	}

	void GameControl::handlePauseMenu()
	{
		changeState(GameState::Playing);
	}

	void GameControl::handleLevelComplete()
	{
		changeState(GameState::Playing);
	}

	void GameControl::handleGameOver()
	{
		changeState(GameState::Quitting);
	}

	void GameControl::changeState(GameState newState)
	{
		currentState = newState;
	}

	Level &GameControl::getCurrentLevel()
	{
		return levels[currentLevelIndex];
	}
}