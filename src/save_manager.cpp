#include <last-outpost/save_manager.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <algorithm>

// Usaremos um formato JSON simples para armazenar os dados
// Para evitar adicionar dependências externas, vamos implementar
// um gerenciamento simples de JSON

namespace Game
{
	SaveManager::SaveManager() : savesPath("saves")
	{
	}

	SaveManager::~SaveManager()
	{
		// Salvar automaticamente ao destruir
		saveToDisk();
	}

	bool SaveManager::initialize(const std::string &savesFolderPath)
	{
		savesPath = savesFolderPath;

		// Verificar se o diretório de saves existe e criar se necessário
		if (!std::filesystem::exists(savesPath))
		{
			try
			{
				std::filesystem::create_directory(savesPath);
			}
			catch (const std::filesystem::filesystem_error &e)
			{
				std::cerr << "Error creating saves directory: " << e.what() << std::endl;
				return false;
			}
		}

		return loadSaves();
	}

	bool SaveManager::loadSaves()
	{
		saves.clear();

		// Verificar se o diretório existe
		if (!std::filesystem::exists(savesPath))
			return false;

		try
		{
			// Percorrer todos os arquivos no diretório de saves
			for (const auto &entry : std::filesystem::directory_iterator(savesPath))
			{
				if (entry.path().extension() == ".save")
				{
					std::ifstream file(entry.path());
					if (!file.is_open())
						continue;

					std::string name;
					int levelIndex;

					// Formato simples: primeira linha = nome, segunda linha = índice do nível
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
								// Ignorar arquivos mal formatados
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
			std::cerr << "Error loading saves: " << e.what() << std::endl;
			return false;
		}
	}

	bool SaveManager::createSave(const std::string &saveName, int levelIndex)
	{
		if (saveName.empty())
			return false;

		// Verificar se já existe um save com este nome
		if (saveExists(saveName))
			return false;

		// Adicionar o novo save à lista
		SaveData newSave{saveName, levelIndex};
		saves.push_back(newSave);

		// Salvar em disco
		return saveToDisk();
	}

	bool SaveManager::saveToDisk()
	{
		try
		{
			// Verificar se o diretório existe e criar se necessário
			if (!std::filesystem::exists(savesPath))
			{
				std::filesystem::create_directory(savesPath);
			}

			// Limpar arquivos de save existentes
			for (const auto &entry : std::filesystem::directory_iterator(savesPath))
			{
				if (entry.path().extension() == ".save")
				{
					std::filesystem::remove(entry.path());
				}
			}

			// Salvar cada save em um arquivo separado
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
			std::cerr << "Error saving files: " << e.what() << std::endl;
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
		std::cout << "Atualizando save '" << saveName << "' para nivel " << newLevelIndex << std::endl;

		auto it = std::find_if(saves.begin(), saves.end(),
							   [&saveName](const SaveData &save)
							   {
								   return save.name == saveName;
							   });

		if (it != saves.end())
		{
			// Atualizar o índice do nível
			it->levelIndex = newLevelIndex;

			// Também vamos atualizar diretamente o arquivo para garantir
			std::string filename = savesPath + "/" + saveName + ".save";
			std::ofstream file(filename);
			if (file.is_open())
			{
				file << saveName << std::endl;
				file << newLevelIndex << std::endl;
				file.close();
				std::cout << "Save atualizado com sucesso: " << filename << std::endl;
			}
			else
			{
				std::cerr << "Erro ao abrir arquivo para atualizar save: " << filename << std::endl;
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
