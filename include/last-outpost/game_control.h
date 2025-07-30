#ifndef LAST_OUTPOST_GAME_CONTROL_H
#define LAST_OUTPOST_GAME_CONTROL_H

#include <SDL.h>
#include <memory>
#include <vector>
#include <last-outpost/game_world.h>
#include <last-outpost/level.h>
#include <last-outpost/audio.h>

namespace Game
{
	enum class GameState
	{
		MainMenu,
		Playing,
		Paused,
		LevelComplete,
		GameOver,
		Quitting
	};

	class GameControl
	{
	public:
		GameControl();
		~GameControl();

		bool initialize();
		void run();
		void cleanup();

	private:
		SDL_Window *window;
		SDL_Renderer *renderer;
		std::unique_ptr<GameWorld> gameWorld;
		std::unique_ptr<Audio> audioSystem;
		GameState currentState;
		std::vector<Level> levels;
		int currentLevelIndex;

		bool initializeSDL();
		void createLevels();
		void handleMainMenu();
		void handleGamePlay();
		void handlePauseMenu();
		void handleLevelComplete();
		void handleGameOver();

		void changeState(GameState newState);
		Level &getCurrentLevel();

		Audio *getAudioSystem() const { return audioSystem.get(); }
	};
}

#endif