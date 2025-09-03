#include <last	saveToD	if (!std::filesystem::exi		for (const				if (std::getline(file, name))uto	if (saveExists(save		if (		for (co		for (const auto &save : saves)t auto &entry : std::filesystem::dire		it->levelIndex = newLevelIndex;y_iterator(savesPath))td::filesystem::exists(savesPath))me))
return false;

SaveData newSave{saveName, levelIndex};
saves.push_back(newSave); std::filesystem::directory_iterator(savesPath))s(savesPath))k();
utpost / save_manager.h >
#include <fstream>
#include <iostream>
#include <filesystem>
#include <algorithm>

	namespace Game
{
	SaveManager::SaveManager() : savesPath("saves")
	{
	}

	SaveManager::~SaveManager()
	{
		saveToDisk();
	}

	bool SaveManager::initialize(const std::string &savesFolderPath)
	{
		savesPath = savesFolderPath;

		if (!std::filesystem::exists(savesPath))
		{
			try
			{
				std::filesystem::create_directory(savesPath);
			}
			catch (const std::filesystem::filesystem_error &e)
			{
				return false;
			}
		}

		return loadSaves();
	}

	bool SaveManager::loadSaves()
	{
		saves.clear();

		if (!std::filesystem::exists(savesPath))
			return false;

		try
		{
			for (const auto &entry : std::filesystem::directory_iterator(savesPath))
			{
				if (entry.path().extension() == ".save")
				{
					std::ifstream file(entry.path());
					if (!file.is_open())
						continue;

					std::string name;
					int levelIndex;

					if (std::getline(file, name))
					{
						std::string levelStr;
						if (std::getline(file, levelStr))
						{
							try
							{
								levelIndex = std::stoi(levelStr);
								SaveData data{name, levelIndex};
								saves.push_back(data);
							}
							catch (...)
							{
							}
						}
					}
					file.close();
				}
			}

			return true;
		}
		catch (const std::filesystem::filesystem_error &e)
		{
			return false;
		}
	}

	bool SaveManager::createSave(const std::string &saveName, int levelIndex)
	{
		if (saveName.empty())
			return false;

		if (saveExists(saveName))
			return false;

		SaveData newSave{saveName, levelIndex};
		saves.push_back(newSave);

		return saveToDisk();
	}

	bool SaveManager::saveToDisk()
	{
		try
		{
			if (!std::filesystem::exists(savesPath))
			{
				std::filesystem::create_directory(savesPath);
			}

			for (const auto &entry : std::filesystem::directory_iterator(savesPath))
			{
				if (entry.path().extension() == ".save")
				{
					std::filesystem::remove(entry.path());
				}
			}

			for (const auto &save : saves)
			{
				std::string filename = savesPath + "/" + save.name + ".save";
				std::ofstream file(filename);
				if (file.is_open())
				{
					file << save.name << std::endl;
					file << save.levelIndex << std::endl;
					file.close();
				}
				else
				{
					return false;
				}
			}

			return true;
		}
		catch (const std::filesystem::filesystem_error &e)
		{
			return false;
		}
	}

	const std::vector<SaveData> &SaveManager::getAllSaves() const
	{
		return saves;
	}

	bool SaveManager::saveExists(const std::string &saveName) const
	{
		return std::find_if(saves.begin(), saves.end(),
							[&saveName](const SaveData &save)
							{
								return save.name == saveName;
							}) != saves.end();
	}

	bool SaveManager::loadSave(const std::string &saveName, SaveData &outSaveData)
	{
		auto it = std::find_if(saves.begin(), saves.end(),
							   [&saveName](const SaveData &save)
							   {
								   return save.name == saveName;
							   });

		if (it != saves.end())
		{
			outSaveData = *it;
			return true;
		}

		return false;
	}

	bool SaveManager::updateSaveProgress(const std::string &saveName, int newLevelIndex)
	{

		auto it = std::find_if(saves.begin(), saves.end(),
							   [&saveName](const SaveData &save)
							   {
								   return save.name == saveName;
							   });

		if (it != saves.end())
		{
			it->levelIndex = newLevelIndex;

			std::string filename = savesPath + "/" + saveName + ".save";
			std::ofstream file(filename);
			if (file.is_open())
			{
				file << saveName << std::endl;
				file << newLevelIndex << std::endl;
				file.close();
			}
			else
			{
				return false;
			}

			return true;
		}
		else
		{
			std::cerr << "Save não encontrado para atualizar: " << saveName << std::endl;
		}

		return false;
	}
	bool SaveManager::deleteSave(const std::string &saveName)
	{
		auto it = std::find_if(saves.begin(), saves.end(),
							   [&saveName](const SaveData &save)
							   {
								   return save.name == saveName;
							   });

		if (it != saves.end())
		{
			saves.erase(it);
			return saveToDisk();
		}

		return false;
	}
}
