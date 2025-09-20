#include <last-outpost/game_control.h>
#include <last-outpost/globals.h>
#include <last-outpost/map.h>
#include <last-outpost/audio.h>
#include <iostream>
#include <fstream>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <imgui/imgui.h>

namespace Game
{
	GameControl::GameControl()
		: window(nullptr), renderer(nullptr), gameWorld(nullptr), audioSystem(nullptr), uiSystem(nullptr),
		  currentState(GameState::MainMenu), currentLevelIndex(0), menuBackgroundTexture(nullptr),
		  gameTitleTexture(nullptr), screenWidth(1024), screenHeight(768),
		  gumelaFont(nullptr), gumelaFontLarge(nullptr), gumelaFontTitle(nullptr),
		  showCreateSaveMenu(false), isNewGameMenuOpen(false), showLoadSaveMenu(false),
		  showScoreboard(false), showCredits(false),
		  saveManager(std::make_unique<SaveManager>()), selectedSaveIndex(-1), currentSaveName(""),
		  goldCollected(0), goldSpent(0), levelStartGold(0), currentGold(100), finalTotalScoreToShow(0)
	{
		memset(saveNameBuffer, 0, sizeof(saveNameBuffer));
	}

	GameControl::~GameControl()
	{
		cleanup();
	}

	bool GameControl::initialize()
	{
		initializeSDL();

		audioSystem = std::make_unique<Audio>();
		audioSystem->initialize();

		audioSystem->loadGameSounds();
		audioSystem->loadGameMusic();

		audioSystem->setMasterVolume(0.7f);
		audioSystem->setSoundVolume(0.5f);
		audioSystem->setMusicVolume(0.6f);

		uiSystem = std::make_unique<UISystem>();
		uiSystem->initialize(window, renderer);

		uiSystem->setOnTowerSelected([this](TowerType towerType) {});
		uiSystem->setOnScoreboardReturn([this]()
										{
		if (currentState == GameState::Victory)
		{
			if (audioSystem)
			{
				audioSystem->fadeOutMusic(500);
				audioSystem->stopMusic();
				audioSystem->playMusic(MusicType::MainMenu);
			}
			changeState(GameState::MainMenu);
		}
		else
		{
			showScoreboard = false;
		} });
		loadMenuAssets();
		loadCustomFonts();

		uiSystem->setCustomFonts(gumelaFont, gumelaFontLarge, gumelaFontTitle);

		saveManager->initialize();
		availableSaves = saveManager->getAllSaves();

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
			case GameState::Victory:
				handleVictory();
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
		if (menuBackgroundTexture)
		{
			SDL_DestroyTexture(menuBackgroundTexture);
			menuBackgroundTexture = nullptr;
		}

		if (gameTitleTexture)
		{
			SDL_DestroyTexture(gameTitleTexture);
			gameTitleTexture = nullptr;
		}

		if (uiSystem)
		{
			uiSystem->cleanup();
			uiSystem.reset();
		}

		if (audioSystem)
		{
			audioSystem->cleanup();
			audioSystem.reset();
		}

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

		Mix_Quit();
		IMG_Quit();
		SDL_Quit();
	}

	bool GameControl::initializeSDL()
	{
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
		IMG_Init(IMG_INIT_PNG);
		Mix_Init(MIX_INIT_MP3);

		window = SDL_CreateWindow("Last Outpost",
								  SDL_WINDOWPOS_UNDEFINED,
								  SDL_WINDOWPOS_UNDEFINED,
								  SCREEN_WIDTH, SCREEN_HEIGHT,
								  SDL_WINDOW_SHOWN);

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		return true;
	}

	void GameControl::createLevels()
	{
		Map map1(TILES_X, TILES_Y, rawStringMap);
		auto path1 = map1.extractPath();

		std::vector<Enemy> enemyTypes1;
		enemyTypes1.reserve(3);
		enemyTypes1.emplace_back(100, 20, 5.0f, "Fireball", path1);
		enemyTypes1.emplace_back(150, 30, 0.8f, "Ice Spike", path1);
		enemyTypes1.emplace_back(200, 40, 0.5f, "Lightning Bolt", path1);
		levels.emplace_back(rawStringMap, std::move(enemyTypes1), 15);

		Map map2(TILES_X, TILES_Y, rawStringMap2);
		auto path2 = map2.extractPath();

		std::vector<Enemy> enemyTypes2;
		enemyTypes2.reserve(3);
		enemyTypes2.emplace_back(120, 25, 6.0f, "Fireball", path2);
		enemyTypes2.emplace_back(180, 35, 1.0f, "Ice Spike", path2);
		enemyTypes2.emplace_back(250, 50, 0.6f, "Lightning Bolt", path2);
		levels.emplace_back(rawStringMap2, std::move(enemyTypes2), 25);
	}

	void GameControl::handleMainMenu()
	{
		if (audioSystem)
		{
			if (!audioSystem->isMusicPlaying() ||
				audioSystem->getCurrentMusicType() != MusicType::MainMenu)
			{
				audioSystem->stopMusic();
				audioSystem->playMusic(MusicType::MainMenu);
			}
		}

		handleMainMenuEvents();

		if (showCredits)
		{
			renderCredits();
		}
		else
		{
			renderMainMenu();
		}
	}

	void GameControl::handleGamePlay()
	{
		if (!gameWorld)
		{
			if (audioSystem)
			{
				audioSystem->fadeOutMusic(1000);
				audioSystem->playMusic(MusicType::GamePlay);
			}

			gameWorld = std::make_unique<GameWorld>(
				renderer,
				SCREEN_WIDTH, SCREEN_HEIGHT,
				std::move(levels),
				currentLevelIndex,
				audioSystem.get(),
				uiSystem.get(),
				currentGold);

			levelStartGold = gameWorld->getGold();

			createLevels();
		}

		GameWorldResult gameResult = gameWorld->run();

		switch (gameResult)
		{
		case GameWorldResult::Quit:
			changeState(GameState::Quitting);
			break;
		case GameWorldResult::ReturnToMainMenu:
			gameWorld.reset();
			if (audioSystem)
			{
				audioSystem->fadeOutMusic(1000);

				audioSystem->stopMusic();
				audioSystem->playMusic(MusicType::MainMenu);
			}

			isNewGameMenuOpen = false;
			changeState(GameState::MainMenu);
			break;
		case GameWorldResult::LevelComplete:
		{
			int earnedGold = 0;
			int endGold = 0;
			if (gameWorld)
			{
				endGold = gameWorld->getGold();
				earnedGold = endGold - levelStartGold;
				if (earnedGold < 0)
					earnedGold = 0;

				currentGold = endGold;
			}

			gameWorld.reset();

			if (++currentLevelIndex < static_cast<int>(levels.size()))
			{
				saveManager->updateSaveProgress(currentSaveName, currentLevelIndex, earnedGold);

				{
					SaveData tmp;
					saveManager->loadSave(currentSaveName, tmp);
				}

				changeState(GameState::LevelComplete);
			}
			else
			{
				SaveData currentSave;
				int previousGold = 0;
				if (saveManager->loadSave(currentSaveName, currentSave))
				{
					previousGold = currentSave.totalScore;
				}

				int totalFinalGold = previousGold + endGold;

				std::string filename = "saves/" + currentSaveName + ".save";
				std::ofstream file(filename);
				if (file.is_open())
				{
					file << currentSaveName << std::endl;
					file << currentLevelIndex << std::endl;
					file << totalFinalGold << std::endl;
					file.close();
				}

				SaveData updatedSave;
				if (saveManager->loadSave(currentSaveName, updatedSave))
				{
					updatedSave.totalScore = totalFinalGold;
					saveManager->loadSaves();
				}

				finalTotalScoreToShow = totalFinalGold;

				changeState(GameState::Victory);
			}
			break;
		}
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

	void GameControl::handleVictory()
	{
		renderVictoryScreen();
	}

	void GameControl::renderVictoryScreen()
	{
		handleMainMenuEvents();
		if (currentState != GameState::Victory)
			return;

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		if (menuBackgroundTexture)
		{
			SDL_RenderCopy(renderer, menuBackgroundTexture, nullptr, nullptr);
		}

		uiSystem->beginFrame();

		availableSaves = saveManager->getAllSaves();
		uiSystem->renderScoreboard(availableSaves, currentSaveName, true);

		uiSystem->endFrame();
		SDL_RenderPresent(renderer);
	}

	void GameControl::renderCredits()
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		if (menuBackgroundTexture)
		{
			SDL_RenderCopy(renderer, menuBackgroundTexture, nullptr, nullptr);
		}

		uiSystem->beginFrame();

		// Cores do tema
		ImVec4 darkBrown = ImVec4(0.25f, 0.17f, 0.12f, 1.0f);
		ImVec4 lightBrown = ImVec4(0.6f, 0.45f, 0.3f, 1.0f);
		ImVec4 parchment = ImVec4(0.92f, 0.85f, 0.7f, 1.0f);
		ImVec4 titleRed = ImVec4(0.8f, 0.2f, 0.1f, 1.0f);

		float windowWidth = 600.0f;
		float windowHeight = 650.0f;
		float windowX = (SCREEN_WIDTH - windowWidth) * 0.5f;
		float windowY = (SCREEN_HEIGHT - windowHeight) * 0.5f;

		ImGui::SetNextWindowPos(ImVec2(windowX, windowY), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Always);

		ImGui::PushStyleColor(ImGuiCol_WindowBg, darkBrown);
		ImGui::PushStyleColor(ImGuiCol_Border, lightBrown);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);

		ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
								 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;

		if (ImGui::Begin("Credits", nullptr, flags))
		{
			// Fundo decorativo
			ImDrawList *drawList = ImGui::GetWindowDrawList();
			ImVec2 windowPos = ImGui::GetWindowPos();
			ImVec2 windowSize = ImGui::GetWindowSize();

			drawList->AddRectFilled(
				ImVec2(windowPos.x + 10, windowPos.y + 10),
				ImVec2(windowPos.x + windowSize.x - 10, windowPos.y + windowSize.y - 10),
				IM_COL32(235, 217, 178, 180),
				8.0f);

			// Título principal
			if (gumelaFontTitle)
				ImGui::PushFont(gumelaFontTitle);

			ImGui::SetCursorPosY(40.0f);
			ImVec2 titleSize = ImGui::CalcTextSize("CRÉDITOS");
			ImGui::SetCursorPosX((windowWidth - titleSize.x) * 0.5f);
			ImGui::PushStyleColor(ImGuiCol_Text, titleRed);
			ImGui::Text("CRÉDITOS");
			ImGui::PopStyleColor();

			if (gumelaFontTitle)
				ImGui::PopFont();

			// Separador
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.0f);
			ImGui::SetCursorPosX(50.0f);
			ImGui::PushStyleColor(ImGuiCol_Separator, lightBrown);
			ImGui::Separator();
			ImGui::PopStyleColor();

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 30.0f);

			// Orientador
			if (gumelaFontLarge)
				ImGui::PushFont(gumelaFontLarge);

			ImGui::PushStyleColor(ImGuiCol_Text, titleRed);
			ImVec2 orientadorSize = ImGui::CalcTextSize("Orientador");
			ImGui::SetCursorPosX((windowWidth - orientadorSize.x) * 0.5f);
			ImGui::Text("Orientador");
			ImGui::PopStyleColor();

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
			ImGui::PushStyleColor(ImGuiCol_Text, parchment);
			ImVec2 profSize = ImGui::CalcTextSize("Professor Dr. Eduardo Henrique Molina Cruz");
			ImGui::SetCursorPosX((windowWidth - profSize.x) * 0.5f);
			ImGui::Text("Professor Dr. Eduardo Henrique Molina Cruz");
			ImGui::PopStyleColor();

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 30.0f);

			// Desenvolvedores
			ImGui::PushStyleColor(ImGuiCol_Text, titleRed);
			ImVec2 devSize = ImGui::CalcTextSize("Desenvolvedores");
			ImGui::SetCursorPosX((windowWidth - devSize.x) * 0.5f);
			ImGui::Text("Desenvolvedores");
			ImGui::PopStyleColor();

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
			ImGui::PushStyleColor(ImGuiCol_Text, parchment);

			ImVec2 eduSize = ImGui::CalcTextSize("Eduardo Albuquerque");
			ImGui::SetCursorPosX((windowWidth - eduSize.x) * 0.5f);
			ImGui::Text("Eduardo Albuquerque");

			ImVec2 alineSize = ImGui::CalcTextSize("Aline Yuka Noguti");
			ImGui::SetCursorPosX((windowWidth - alineSize.x) * 0.5f);
			ImGui::Text("Aline Yuka Noguti");

			ImVec2 vitorSize = ImGui::CalcTextSize("Vitor Tavares");
			ImGui::SetCursorPosX((windowWidth - vitorSize.x) * 0.5f);
			ImGui::Text("Vitor Tavares");
			ImGui::PopStyleColor();

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 30.0f);

			// Artes
			ImGui::PushStyleColor(ImGuiCol_Text, titleRed);
			ImVec2 artSize = ImGui::CalcTextSize("Artes");
			ImGui::SetCursorPosX((windowWidth - artSize.x) * 0.5f);
			ImGui::Text("Artes");
			ImGui::PopStyleColor();

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
			ImGui::PushStyleColor(ImGuiCol_Text, parchment);

			ImVec2 eduArtSize = ImGui::CalcTextSize("Eduardo Albuquerque");
			ImGui::SetCursorPosX((windowWidth - eduArtSize.x) * 0.5f);
			ImGui::Text("Eduardo Albuquerque");

			ImVec2 alineArtSize = ImGui::CalcTextSize("Aline Yuka");
			ImGui::SetCursorPosX((windowWidth - alineArtSize.x) * 0.5f);
			ImGui::Text("Aline Yuka");
			ImGui::PopStyleColor();

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 30.0f);

			// Áudios
			ImGui::PushStyleColor(ImGuiCol_Text, titleRed);
			ImVec2 audioSize = ImGui::CalcTextSize("Áudios");
			ImGui::SetCursorPosX((windowWidth - audioSize.x) * 0.5f);
			ImGui::Text("Áudios");
			ImGui::PopStyleColor();

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
			ImGui::PushStyleColor(ImGuiCol_Text, parchment);
			ImVec2 eduAudioSize = ImGui::CalcTextSize("Eduardo Albuquerque");
			ImGui::SetCursorPosX((windowWidth - eduAudioSize.x) * 0.5f);
			ImGui::Text("Eduardo Albuquerque");
			ImGui::PopStyleColor();

			if (gumelaFontLarge)
				ImGui::PopFont();

			// Botão voltar
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 40.0f);
			ImGui::SetCursorPosX((windowWidth - 200.0f) * 0.5f);

			ImGui::PushStyleColor(ImGuiCol_Button, lightBrown);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.55f, 0.4f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.35f, 0.25f, 1.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);

			if (gumelaFontLarge)
				ImGui::PushFont(gumelaFontLarge);

			if (ImGui::Button("Menu Principal", ImVec2(200, 50)))
			{
				showCredits = false;
			}

			if (gumelaFontLarge)
				ImGui::PopFont();

			ImGui::PopStyleVar();
			ImGui::PopStyleColor(3);
		}
		ImGui::End();

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(2);

		uiSystem->endFrame();
		SDL_RenderPresent(renderer);
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

	void GameControl::loadMenuAssets()
	{
		SDL_Surface *menuSurface = IMG_Load("assets/Menu.png");
		if (menuSurface)
		{
			menuBackgroundTexture = SDL_CreateTextureFromSurface(renderer, menuSurface);
			SDL_FreeSurface(menuSurface);
		}
	}

	void GameControl::handleMainMenuEvents()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				changeState(GameState::Quitting);
				return;
			}

			uiSystem->handleEvent(event);

			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					changeState(GameState::Quitting);
					break;
				}
			}
		}
	}

	void GameControl::renderMainMenu()
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		if (menuBackgroundTexture)
		{
			SDL_RenderCopy(renderer, menuBackgroundTexture, nullptr, nullptr);
		}

		uiSystem->beginFrame();

		ImVec4 darkBrown = ImVec4(0.25f, 0.17f, 0.12f, 1.0f);
		ImVec4 mediumBrown = ImVec4(0.4f, 0.3f, 0.2f, 1.0f);
		ImVec4 lightBrown = ImVec4(0.6f, 0.45f, 0.3f, 1.0f);
		ImVec4 parchment = ImVec4(0.92f, 0.85f, 0.7f, 1.0f);
		ImVec4 titleRed = ImVec4(0.8f, 0.2f, 0.1f, 1.0f);
		ImVec4 hoverBrown = ImVec4(0.5f, 0.35f, 0.25f, 1.0f);

		float menuWidth = 500.0f;
		float menuHeight = 600.0f;
		float menuX = (SCREEN_WIDTH - menuWidth) * 0.5f;
		float menuY = (SCREEN_HEIGHT - menuHeight) * 0.5f;

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(SCREEN_WIDTH, SCREEN_HEIGHT));

		ImGuiWindowFlags titleWindowFlags = ImGuiWindowFlags_NoResize |
											ImGuiWindowFlags_NoMove |
											ImGuiWindowFlags_NoCollapse |
											ImGuiWindowFlags_NoTitleBar |
											ImGuiWindowFlags_NoScrollbar |
											ImGuiWindowFlags_NoBackground |
											ImGuiWindowFlags_NoInputs;

		if (ImGui::Begin("Title", nullptr, titleWindowFlags))
		{
			ImGui::PushStyleColor(ImGuiCol_Text, titleRed);

			if (gumelaFontTitle)
			{
				ImGui::PushFont(gumelaFontTitle);
			}

			ImVec2 titleSize = ImGui::CalcTextSize("LAST-OUTPOST");
			float titleX = (SCREEN_WIDTH - titleSize.x) * 0.5f;
			float titleY = menuY - titleSize.y - 40.0f;

			ImGui::SetCursorPos(ImVec2(titleX, titleY));
			ImGui::Text("LAST-OUTPOST");

			if (gumelaFontTitle)
			{
				ImGui::PopFont();
			}

			ImGui::PopStyleColor();
		}
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(menuX, menuY), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(menuWidth, menuHeight), ImGuiCond_Always);

		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoResize |
									   ImGuiWindowFlags_NoMove |
									   ImGuiWindowFlags_NoCollapse |
									   ImGuiWindowFlags_NoTitleBar |
									   ImGuiWindowFlags_NoScrollbar;

		ImGui::PushStyleColor(ImGuiCol_WindowBg, darkBrown);
		ImGui::PushStyleColor(ImGuiCol_Border, lightBrown);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);

		if (ImGui::Begin("Main Menu", nullptr, windowFlags))
		{
			ImDrawList *drawList = ImGui::GetWindowDrawList();
			ImVec2 windowPos = ImGui::GetWindowPos();
			ImVec2 windowSize = ImGui::GetWindowSize();

			drawList->AddRectFilled(
				ImVec2(windowPos.x + 10, windowPos.y + 10),
				ImVec2(windowPos.x + windowSize.x - 10, windowPos.y + windowSize.y - 10),
				IM_COL32(235, 217, 178, 180),
				8.0f);

			ImGui::SetCursorPosY(80.0f);

			ImGui::PushStyleColor(ImGuiCol_Button, mediumBrown);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoverBrown);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, lightBrown);
			ImGui::PushStyleColor(ImGuiCol_Text, parchment);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);

			float buttonWidth = 300.0f;
			float buttonHeight = 60.0f;
			float buttonX = (menuWidth - buttonWidth) * 0.5f;
			ImVec2 buttonSize(buttonWidth, buttonHeight);

			if (!isNewGameMenuOpen && !showLoadSaveMenu && !showScoreboard && !showCredits)
			{
				ImGui::SetCursorPosX(buttonX);
				if (gumelaFontLarge)
				{
					ImGui::PushFont(gumelaFontLarge);
				}
				if (ImGui::Button("Novo Jogo", buttonSize))
				{
					isNewGameMenuOpen = true;
					memset(saveNameBuffer, 0, sizeof(saveNameBuffer));
				}
				if (gumelaFontLarge)
				{
					ImGui::PopFont();
				}

				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.0f);
				ImGui::SetCursorPosX(buttonX);
				if (gumelaFontLarge)
				{
					ImGui::PushFont(gumelaFontLarge);
				}
				if (ImGui::Button("Continuar Jogo", buttonSize))
				{
					availableSaves = saveManager->getAllSaves();
					showLoadSaveMenu = true;
				}
				if (gumelaFontLarge)
				{
					ImGui::PopFont();
				}

				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.0f);
				ImGui::SetCursorPosX(buttonX);
				if (gumelaFontLarge)
				{
					ImGui::PushFont(gumelaFontLarge);
				}
				if (ImGui::Button("Placar", buttonSize))
				{
					availableSaves = saveManager->getAllSaves();
					showScoreboard = true;
				}
				if (gumelaFontLarge)
				{
					ImGui::PopFont();
				}

				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.0f);
				ImGui::SetCursorPosX(buttonX);
				if (gumelaFontLarge)
				{
					ImGui::PushFont(gumelaFontLarge);
				}
				if (ImGui::Button("Créditos", buttonSize))
				{
					showCredits = true;
				}
				if (gumelaFontLarge)
				{
					ImGui::PopFont();
				}
			}
			else if (isNewGameMenuOpen)
			{
				ImGui::SetCursorPosX(buttonX);
				if (gumelaFontLarge)
				{
					ImGui::PushFont(gumelaFontLarge);
				}

				ImGui::Text("Nome do Save:");
				ImGui::SetCursorPosX(buttonX);
				ImGui::SetNextItemWidth(buttonWidth);
				ImGui::InputText("##SaveName", saveNameBuffer, sizeof(saveNameBuffer));

				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.0f);
				ImGui::SetCursorPosX(buttonX);
				if (ImGui::Button("Criar", buttonSize))
				{
					if (strlen(saveNameBuffer) > 0)
					{
						startNewGame(saveNameBuffer);
						showCreateSaveMenu = true;
						isNewGameMenuOpen = false;
					}
					else
					{
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
						ImGui::SetCursorPosX(buttonX);
						ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Digite um nome para o save!");
					}
				}

				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
				ImGui::SetCursorPosX(buttonX);
				if (ImGui::Button("Cancelar", buttonSize))
				{
					isNewGameMenuOpen = false;
				}

				if (gumelaFontLarge)
				{
					ImGui::PopFont();
				}
			}
			else if (showLoadSaveMenu)
			{
				renderLoadSaveMenu();
			}
			else if (showScoreboard)
			{
				uiSystem->renderScoreboard(availableSaves, "");
			}

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.0f);
			ImGui::SetCursorPosX(buttonX);
			if (gumelaFontLarge)
			{
				ImGui::PushFont(gumelaFontLarge);
			}
			if (ImGui::Button("Configurações", buttonSize))
			{
			}
			if (gumelaFontLarge)
			{
				ImGui::PopFont();
			}

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.0f);
			ImGui::SetCursorPosX(buttonX);
			if (gumelaFontLarge)
			{
				ImGui::PushFont(gumelaFontLarge);
			}
			if (ImGui::Button("Fechar Jogo", buttonSize))
			{
				changeState(GameState::Quitting);
			}
			if (gumelaFontLarge)
			{
				ImGui::PopFont();
			}

			ImGui::PopStyleVar();
			ImGui::PopStyleColor(4);
		}
		ImGui::End();

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(2);
		uiSystem->endFrame();
		SDL_RenderPresent(renderer);
	}

	void GameControl::loadCustomFonts()
	{
		ImGuiIO &io = ImGui::GetIO();

		gumelaFont = io.Fonts->AddFontFromFileTTF("assets/fonts/Gumela.ttf", 18.0f);
		gumelaFontLarge = io.Fonts->AddFontFromFileTTF("assets/fonts/Gumela.ttf", 24.0f);
		gumelaFontTitle = io.Fonts->AddFontFromFileTTF("assets/fonts/Gumela.ttf", 72.0f);

		io.Fonts->Build();

		io.FontDefault = gumelaFont;
	}
}
