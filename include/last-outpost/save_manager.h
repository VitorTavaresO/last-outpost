#ifndef __LAST_OUTPOST_SAVE_MANAGER_H__
#define __LAST_OUTPOST_SAVE_MANAGER_H__

#include <string>
#include <vector>

namespace Game
{
	struct SaveData
	{
		std::string name;
		int levelIndex;
	};

	class SaveManager
	{
	private:
		std::string savesPath;
		std::vector<SaveData> saves;

	public:
		SaveManager();
		~SaveManager();

		bool initialize(const std::string &savesFolderPath = "saves");

		bool loadSaves();

		bool createSave(const std::string &saveName, int levelIndex = 0);

		bool saveToDisk();

		const std::vector<SaveData> &getAllSaves() const;

		bool saveExists(const std::string &saveName) const;

		bool loadSave(const std::string &saveName, SaveData &outSaveData);

		bool updateSaveProgress(const std::string &saveName, int newLevelIndex);

		bool deleteSave(const std::string &saveName);
	};
}

#endif
