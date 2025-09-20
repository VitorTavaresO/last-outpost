#ifndef LAST_OUTPOST_UI_SYSTEM_H
#define LAST_OUTPOST_UI_SYSTEM_H

#include <SDL.h>
#include <last-outpost/types.h>
#include <last-outpost/save_manager.h>
#include <my-lib/matrix.h>
#include <functional>
#include <vector>
#include <string>

struct ImFont;

namespace Game
{
	enum class TowerType
	{
		Magic,
		Fire,
		Leaf,
		Thunder
	};

	struct TowerInfo
	{
		TowerType type;
		const char *name;
		const char *description;
		int cost;
		const char *iconPath;
	};

	struct SelectedTowerInfo
	{
		int index;
		TowerType type;
		int damage;
		float range;
		float fireRate;
		int sellValue;
		int upgradeValue;
		Vector position;
	};

	class UISystem
	{
	public:
		UISystem();
		~UISystem();

		bool initialize(SDL_Window *window, SDL_Renderer *renderer);
		void cleanup();

		void beginFrame();
		void endFrame();

		void renderUI(int screenWidth, int screenHeight, int gold, int playerLife,
					  const SelectedTowerInfo *selectedTowerInfo = nullptr);
		void handleEvent(const SDL_Event &event);

		void setOnTowerSelected(std::function<void(TowerType)> callback);
		void setOnTowerSell(std::function<void()> callback);
		void setOnTowerUpgrade(std::function<void()> callback);
		void setOnPlacementCancel(std::function<void()> callback);
		void setOnScoreboardReturn(std::function<void()> callback);
		void setCustomFonts(ImFont *normalFont, ImFont *largeFont, ImFont *titleFont);

		TowerType getSelectedTower() const { return selectedTower; }
		bool isTowerSelected() const { return towerSelected; }
		void clearTowerSelection();

		void renderScoreboard(const std::vector<SaveData> &saves, const std::string &highlightSave = "", bool showReturnButton = true);

	private:
		SDL_Window *window;
		SDL_Renderer *renderer;
		bool initialized;

		TowerType selectedTower;
		bool towerSelected;

		std::function<void(TowerType)> onTowerSelectedCallback;
		std::function<void()> onTowerSellCallback;
		std::function<void()> onTowerUpgradeCallback;
		std::function<void()> onPlacementCancelCallback;
		std::function<void()> onScoreboardReturnCallback;
		ImFont *customNormalFont;
		ImFont *customLargeFont;
		ImFont *customTitleFont;

		static const TowerInfo towerInfos[4];

		void renderTowerSelectionMenu();
		void renderSelectedTowerMenu(const SelectedTowerInfo &towerInfo);
		void renderGameStats(int gold, int playerLife);
		void setupWoodParchmentTheme();
	};
}

#endif
