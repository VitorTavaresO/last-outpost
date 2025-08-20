#include <last-outpost/ui_system.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_sdlrenderer2.h>
#include <iostream>

namespace Game
{
	const TowerInfo UISystem::towerInfos[5] = {
		{TowerType::Canon, "Canon Tower", "Basic tower with good damage", 50, "assets/sprites/towers/canon-tower.png"},
		{TowerType::Fire, "Fire Tower", "Shoots fireballs with area damage", 75, "assets/sprites/towers/fire-tower.png"},
		{TowerType::Magic, "Magic Tower", "High damage with slow rate", 100, "assets/sprites/towers/magic-tower.png"},
		{TowerType::Leaf, "Leaf Tower", "Slows down enemies", 60, "assets/sprites/towers/leaf-tower.png"},
		{TowerType::Thunder, "Thunder Tower", "Fast attack speed", 80, "assets/sprites/towers/thunder-tower.png"}};

	UISystem::UISystem()
		: window(nullptr), renderer(nullptr), initialized(false),
		  selectedTower(TowerType::Canon), towerSelected(false)
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

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO &io = ImGui::GetIO();
		(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// Setup Platform/Renderer backends
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

		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
	}

	void UISystem::endFrame()
	{
		if (!initialized)
			return;

		// Rendering
		ImGui::Render();
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
	}

	void UISystem::renderTowerMenu(int screenWidth, int screenHeight)
	{
		if (!initialized)
			return;

		// Calculate menu dimensions (1/5 of screen width)
		float menuWidth = screenWidth / 5.0f;
		float menuHeight = screenHeight;

		// Set window position and size
		ImGui::SetNextWindowPos(ImVec2(screenWidth - menuWidth, 0), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(menuWidth, menuHeight), ImGuiCond_Always);

		// Create the tower menu window
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoResize |
									   ImGuiWindowFlags_NoMove |
									   ImGuiWindowFlags_NoCollapse |
									   ImGuiWindowFlags_NoTitleBar;

		ImGui::Begin("Tower Menu", nullptr, windowFlags);

		// Title
		ImGui::Text("Tower Selection");
		ImGui::Separator();
		ImGui::Spacing();

		// Tower buttons
		for (int i = 0; i < 5; i++)
		{
			const TowerInfo &tower = towerInfos[i];
			bool isSelected = (selectedTower == tower.type && towerSelected);

			if (isSelected)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.7f, 0.4f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.8f, 0.5f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.6f, 0.3f, 1.0f));
			}

			// Create button with tower name and cost
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

			// Show tooltip with description
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

		// Clear selection button
		if (ImGui::Button("Clear Selection", ImVec2(menuWidth - 20, 30)))
		{
			clearTowerSelection();
		}

		// Display current selection
		ImGui::Spacing();
		if (towerSelected)
		{
			ImGui::Text("Selected: %s", towerInfos[static_cast<int>(selectedTower)].name);
			ImGui::Text("Cost: $%d", towerInfos[static_cast<int>(selectedTower)].cost);
		}
		else
		{
			ImGui::Text("No tower selected");
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

	void UISystem::clearTowerSelection()
	{
		towerSelected = false;
	}
}
