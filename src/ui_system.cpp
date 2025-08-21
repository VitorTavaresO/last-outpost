#include <last-outpost/ui_system.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_sdlrenderer2.h>
#include <iostream>

namespace Game
{
	const TowerInfo UISystem::towerInfos[4] = {
		{TowerType::Magic, "Magic Tower", "High damage projectiles", 50, "assets/sprites/towers/magic-tower.png"},
		{TowerType::Fire, "Fire Tower", "Area damage with fireballs", 75, "assets/sprites/towers/fire-tower.png"},
		{TowerType::Leaf, "Leaf Tower", "Slows down enemies", 60, "assets/sprites/towers/leaf-tower.png"},
		{TowerType::Thunder, "Thunder Tower", "Fast attack speed", 80, "assets/sprites/towers/thunder-tower.png"}};

	UISystem::UISystem()
		: window(nullptr), renderer(nullptr), initialized(false),
		  selectedTower(TowerType::Magic), towerSelected(false)
	{
	}

	UISystem::~UISystem()
	{
		cleanup();
	}

	bool UISystem::initialize(SDL_Window *window, SDL_Renderer *renderer)
	{
		this->window = window;
		this->renderer = renderer;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO &io = ImGui::GetIO();
		(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		ImGui::StyleColorsDark();

		if (!ImGui_ImplSDL2_InitForSDLRenderer(window, renderer))
		{
			std::cerr << "Failed to initialize ImGui SDL2 backend!" << std::endl;
			return false;
		}

		if (!ImGui_ImplSDLRenderer2_Init(renderer))
		{
			std::cerr << "Failed to initialize ImGui SDL Renderer backend!" << std::endl;
			return false;
		}

		initialized = true;
		return true;
	}

	void UISystem::cleanup()
	{
		if (initialized)
		{
			ImGui_ImplSDLRenderer2_Shutdown();
			ImGui_ImplSDL2_Shutdown();
			ImGui::DestroyContext();
			initialized = false;
		}
	}

	void UISystem::beginFrame()
	{
		if (!initialized)
			return;

		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
	}

	void UISystem::endFrame()
	{
		if (!initialized)
			return;

		ImGui::Render();
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
	}

	void UISystem::renderUI(int screenWidth, int screenHeight, int gold, int playerLife,
							const SelectedTowerInfo *selectedTowerInfo)
	{
		if (!initialized)
			return;

		renderGameStats(gold, playerLife);

		if (selectedTowerInfo)
		{
			renderSelectedTowerMenu(*selectedTowerInfo);
		}
		else
		{
			renderTowerSelectionMenu();
		}
	}

	void UISystem::renderGameStats(int gold, int playerLife)
	{
		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(300, 80), ImGuiCond_Always);

		ImGuiWindowFlags statsFlags = ImGuiWindowFlags_NoResize |
									  ImGuiWindowFlags_NoMove |
									  ImGuiWindowFlags_NoCollapse |
									  ImGuiWindowFlags_NoTitleBar |
									  ImGuiWindowFlags_NoBackground;

		ImGui::Begin("Game Stats", nullptr, statsFlags);

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.84f, 0.0f, 1.0f));
		ImGui::Text("GOLD: %d", gold);
		ImGui::PopStyleColor();

		ImGui::SameLine();
		ImGui::SetCursorPosX(150);

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
		ImGui::Text("LIFE: %d", playerLife);
		ImGui::PopStyleColor();

		ImGui::End();
	}

	void UISystem::renderTowerSelectionMenu()
	{
		float screenWidth = 0;
		float screenHeight = 0;

		if (window)
		{
			int w, h;
			SDL_GetWindowSize(window, &w, &h);
			screenWidth = static_cast<float>(w);
			screenHeight = static_cast<float>(h);
		}

		float menuWidth = screenWidth / 5.0f;
		float menuHeight = screenHeight;

		ImGui::SetNextWindowPos(ImVec2(screenWidth - menuWidth, 0), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(menuWidth, menuHeight), ImGuiCond_Always);

		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoResize |
									   ImGuiWindowFlags_NoMove |
									   ImGuiWindowFlags_NoCollapse |
									   ImGuiWindowFlags_NoTitleBar;

		ImGui::Begin("Tower Menu", nullptr, windowFlags);

		ImGui::Text("Tower Selection");
		ImGui::Separator();
		ImGui::Spacing();

		for (int i = 0; i < 4; i++)
		{
			const TowerInfo &tower = towerInfos[i];
			bool isSelected = (selectedTower == tower.type && towerSelected);

			if (isSelected)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.7f, 0.4f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.8f, 0.5f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.6f, 0.3f, 1.0f));
			}

			std::string buttonText = std::string(tower.name) + "\n$" + std::to_string(tower.cost);

			if (ImGui::Button(buttonText.c_str(), ImVec2(menuWidth - 20, 60)))
			{
				selectedTower = tower.type;
				towerSelected = true;

				if (onTowerSelectedCallback)
				{
					onTowerSelectedCallback(tower.type);
				}
			}

			if (isSelected)
			{
				ImGui::PopStyleColor(3);
			}

			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("%s", tower.description);
				ImGui::EndTooltip();
			}

			ImGui::Spacing();
		}

		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::Button("Cancel Selection", ImVec2(menuWidth - 20, 30)))
		{
			clearTowerSelection();
			if (onPlacementCancelCallback)
			{
				onPlacementCancelCallback();
			}
		}

		ImGui::Spacing();
		if (towerSelected)
		{
			ImGui::Text("Selected: %s", towerInfos[static_cast<int>(selectedTower)].name);
			ImGui::Text("Cost: $%d", towerInfos[static_cast<int>(selectedTower)].cost);
			ImGui::Spacing();
			ImGui::TextWrapped("Click on an empty tile to place the tower");
		}
		else
		{
			ImGui::Text("No tower selected");
			ImGui::Spacing();
			ImGui::TextWrapped("Select a tower type and click on the map to place it");
		}

		ImGui::End();
	}

	void UISystem::renderSelectedTowerMenu(const SelectedTowerInfo &towerInfo)
	{
		float screenWidth = 0;
		float screenHeight = 0;

		if (window)
		{
			int w, h;
			SDL_GetWindowSize(window, &w, &h);
			screenWidth = static_cast<float>(w);
			screenHeight = static_cast<float>(h);
		}

		float menuWidth = screenWidth / 5.0f;
		float menuHeight = screenHeight;

		ImGui::SetNextWindowPos(ImVec2(screenWidth - menuWidth, 0), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(menuWidth, menuHeight), ImGuiCond_Always);

		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoResize |
									   ImGuiWindowFlags_NoMove |
									   ImGuiWindowFlags_NoCollapse |
									   ImGuiWindowFlags_NoTitleBar;

		ImGui::Begin("Tower Info", nullptr, windowFlags);

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
		ImGui::Text("SELECTED TOWER");
		ImGui::PopStyleColor();
		ImGui::Separator();
		ImGui::Spacing();

		const TowerInfo &info = towerInfos[static_cast<int>(towerInfo.type)];
		ImGui::Text("Type: %s", info.name);
		ImGui::Text("Position: (%.0f, %.0f)", towerInfo.position.x, towerInfo.position.y);
		ImGui::Spacing();

		ImGui::Text("Stats:");
		ImGui::Text("  Damage: %d", towerInfo.damage);
		ImGui::Text("  Range: %.1f", towerInfo.range);
		ImGui::Text("  Fire Rate: %.1f", towerInfo.fireRate);
		ImGui::Spacing();

		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Actions:");
		ImGui::Spacing();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.3f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 0.1f, 0.1f, 1.0f));

		std::string sellText = "Sell Tower\n$" + std::to_string(towerInfo.sellValue);
		if (ImGui::Button(sellText.c_str(), ImVec2(menuWidth - 20, 50)))
		{
			if (onTowerSellCallback)
			{
				onTowerSellCallback();
			}
		}

		ImGui::PopStyleColor(3);
		ImGui::Spacing();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.8f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.7f, 0.9f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.5f, 0.7f, 1.0f));

		std::string upgradeText = "Upgrade Tower\n$" + std::to_string(towerInfo.upgradeValue);
		if (ImGui::Button(upgradeText.c_str(), ImVec2(menuWidth - 20, 50)))
		{
			if (onTowerUpgradeCallback)
			{
				onTowerUpgradeCallback();
			}
		}

		ImGui::PopStyleColor(3);
		ImGui::Spacing();

		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::Button("Deselect Tower", ImVec2(menuWidth - 20, 30)))
		{
			if (onPlacementCancelCallback)
			{
				onPlacementCancelCallback();
			}
		}

		ImGui::End();
	}

	void UISystem::handleEvent(const SDL_Event &event)
	{
		if (initialized)
		{
			ImGui_ImplSDL2_ProcessEvent(&event);
		}
	}

	void UISystem::setOnTowerSelected(std::function<void(TowerType)> callback)
	{
		onTowerSelectedCallback = callback;
	}

	void UISystem::setOnTowerSell(std::function<void()> callback)
	{
		onTowerSellCallback = callback;
	}

	void UISystem::setOnTowerUpgrade(std::function<void()> callback)
	{
		onTowerUpgradeCallback = callback;
	}

	void UISystem::setOnPlacementCancel(std::function<void()> callback)
	{
		onPlacementCancelCallback = callback;
	}

	void UISystem::clearTowerSelection()
	{
		towerSelected = false;
	}
}
