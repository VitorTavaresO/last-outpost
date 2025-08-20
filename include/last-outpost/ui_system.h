#ifndef LAST_OUTPOST_UI_SYSTEM_H
#define LAST_OUTPOST_UI_SYSTEM_H

#include <SDL.h>
#include <last-outpost/types.h>
#include <functional>

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

	class UISystem
	{
	public:
		UISystem();
		~UISystem();

		bool initialize(SDL_Window *window, SDL_Renderer *renderer);
		void cleanup();

		void beginFrame();
		void endFrame();

		void renderTowerMenu(int screenWidth, int screenHeight);
		void handleEvent(const SDL_Event &event);

		void setOnTowerSelected(std::function<void(TowerType)> callback);
		TowerType getSelectedTower() const { return selectedTower; }
		bool isTowerSelected() const { return towerSelected; }
		void clearTowerSelection();

	private:
		SDL_Window *window;
		SDL_Renderer *renderer;
		bool initialized;

		TowerType selectedTower;
		bool towerSelected;
		std::function<void(TowerType)> onTowerSelectedCallback;

		static const TowerInfo towerInfos[4];
		void renderTowerButton(const TowerInfo &tower, bool isSelected);
	};
}

#endif
