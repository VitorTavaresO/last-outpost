#ifndef LAST_OUTPOST_GAME_CONTROL_H
#define LAST_OUTPOST_GAME_CONTROL_H

#include <SDL.h>
#include <memory>
#include <vector>
#include <last-outpost/game_world.h>
#include <last-outpost/level.h>
#include <last-outpost/audio.h>
#include <last-outpost/ui_system.h>
#include <last-outpost/save_manager.h>

struct ImFont;

namespace Game
{
	enum class GameState
	{
		MainMenu,
		Playing,
		Paused,
		LevelComplete,
		Victory,
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
		std::unique_ptr<UISystem> uiSystem;
		GameState currentState;
		std::vector<Level> levels;
		int currentLevelIndex;

		SDL_Texture *menuBackgroundTexture;
		SDL_Texture *gameTitleTexture;
		int screenWidth;
		int screenHeight;

		ImFont *gumelaFont;
		ImFont *gumelaFontLarge;
		ImFont *gumelaFontTitle;

		bool showCreateSaveMenu;
		char saveNameBuffer[256];
		bool isNewGameMenuOpen;
		bool showLoadSaveMenu;

		std::unique_ptr<SaveManager> saveManager;
		std::vector<SaveData> availableSaves;
		int selectedSaveIndex;
		std::string currentSaveName;

		int goldCollected;
		int goldSpent;
		int levelStartGold;
		int currentGold;
		int finalTotalScoreToShow;
		int calculateTotalScore() const { return goldCollected + goldSpent; }

		bool initializeSDL();
		void createLevels();
		void handleMainMenu();
		void handleGamePlay();
		void handlePauseMenu();
		void handleLevelComplete();
		void handleVictory();
		void handleGameOver();

		void renderMainMenu();
		void renderVictoryScreen();
		void handleMainMenuEvents();
		void loadMenuAssets();
		void loadCustomFonts();
		void renderCreateSaveMenu();
		void renderLoadSaveMenu();
		bool startNewGame(const std::string &saveName);
		bool loadGame(const std::string &saveName);
		bool updateCurrentSaveProgress(int levelIndex, int newGold);
		std::string getCurrentSaveName() const;

		void changeState(GameState newState);
		Level &getCurrentLevel();

		Audio *getAudioSystem() const { return audioSystem.get(); }
	};
}

#endif