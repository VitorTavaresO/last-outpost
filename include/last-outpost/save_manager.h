#ifndef __LAST_OUTPOST_SAVE_MANAGER_H__
#define __LAST_OUTPOST_SAVE_MANAGER_H__

#include <string>
#include <vector>

namespace Game
{
	// Estrutura para armazenar informações de um save
	struct SaveData
	{
		std::string name; // Nome do save
		int levelIndex;	  // Índice do nível atual
	};

	class SaveManager
	{
	private:
		std::string savesPath;
		std::vector<SaveData> saves;

	public:
		SaveManager();
		~SaveManager();

		// Inicializa o gerenciador de saves
		bool initialize(const std::string &savesFolderPath = "saves");

		// Carrega todos os saves disponíveis
		bool loadSaves();

		// Cria um novo save
		bool createSave(const std::string &saveName, int levelIndex = 0);

		// Salva os dados em arquivo
		bool saveToDisk();

		// Obtém a lista de todos os saves disponíveis
		const std::vector<SaveData> &getAllSaves() const;

		// Verifica se já existe um save com o nome fornecido
		bool saveExists(const std::string &saveName) const;

		// Carrega um save específico pelo nome
		bool loadSave(const std::string &saveName, SaveData &outSaveData);

		// Atualiza o progresso do nível em um save existente
		bool updateSaveProgress(const std::string &saveName, int newLevelIndex);

		// Deleta um save pelo nome
		bool deleteSave(const std::string &saveName);
	};
}

#endif
