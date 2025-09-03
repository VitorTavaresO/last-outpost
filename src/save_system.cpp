// Métodos adicionais para o sistema de saves

#include <last-outpost/game_control.h>
#include <imgui/imgui.h>

namespace Game
{
	void GameControl::renderCreateSaveMenu()
	{
		// Não é necessário recriar o componente aqui pois ele já está implementado
		// na função renderMainMenu com a variável isNewGameMenuOpen
	}

	void GameControl::renderLoadSaveMenu()
	{
		// Parte do menu para carregar um jogo salvo
		float buttonWidth = 300.0f;
		float buttonHeight = 60.0f;
		float buttonX = (500.0f - buttonWidth) * 0.5f; // 500.0f é a largura do menu
		ImVec2 buttonSize(buttonWidth, buttonHeight);

		// Usar fonte grande para o título
		if (gumelaFontLarge)
		{
			ImGui::PushFont(gumelaFontLarge);
		}

		ImGui::SetCursorPosX(buttonX);
		ImGui::Text("Selecione um Save:");

		if (gumelaFontLarge)
		{
			ImGui::PopFont();
		}

		// Verificar se há saves disponíveis
		if (availableSaves.empty())
		{
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.0f);
			ImGui::SetCursorPosX(buttonX);
			ImGui::Text("Nenhum save encontrado!");
		}
		else
		{
			// Listar os saves disponíveis
			for (size_t i = 0; i < availableSaves.size(); i++)
			{
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
				ImGui::SetCursorPosX(buttonX);

				// Criar um botão para cada save
				if (ImGui::Button(availableSaves[i].name.c_str(), buttonSize))
				{
					// Carregar o save selecionado
					selectedSaveIndex = i;
					loadGame(availableSaves[i].name);
					showLoadSaveMenu = false;
				}
			}
		}

		// Botão de voltar
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.0f);
		ImGui::SetCursorPosX(buttonX);

		if (gumelaFontLarge)
		{
			ImGui::PushFont(gumelaFontLarge);
		}

		if (ImGui::Button("Voltar", buttonSize))
		{
			showLoadSaveMenu = false;
		}

		if (gumelaFontLarge)
		{
			ImGui::PopFont();
		}
	}

	bool GameControl::startNewGame(const std::string &saveName)
	{
		if (saveName.empty())
			return false;

		// Criar um novo save
		if (!saveManager->createSave(saveName, 0)) // 0 = índice do primeiro nível
		{
			std::cerr << "Failed to create save: " << saveName << std::endl;
			return false;
		}

		// Recarregar a lista de saves disponíveis
		availableSaves = saveManager->getAllSaves();

		// Configurar o nível inicial
		currentLevelIndex = 0;

		// Salvar o nome do save atual
		currentSaveName = saveName;

		// Iniciar o jogo
		changeState(GameState::Playing);

		return true;
	}

	bool GameControl::loadGame(const std::string &saveName)
	{
		SaveData saveData;
		if (!saveManager->loadSave(saveName, saveData))
		{
			std::cerr << "Failed to load save: " << saveName << std::endl;
			return false;
		}

		// Configurar o nível a partir do save
		currentLevelIndex = saveData.levelIndex;

		// Verificar se o índice do nível é válido
		if (currentLevelIndex < 0 || currentLevelIndex >= static_cast<int>(levels.size()))
		{
			std::cerr << "Invalid level index in save: " << currentLevelIndex << std::endl;
			currentLevelIndex = 0; // Fallback para o primeiro nível
		}

		// Salvar o nome do save atual
		currentSaveName = saveName;

		// Iniciar o jogo
		changeState(GameState::Playing);

		return true;
	}

	bool GameControl::updateCurrentSaveProgress(int levelIndex)
	{
		// Verifica se temos um save atual
		if (currentSaveName.empty())
		{
			std::cerr << "No current save to update progress" << std::endl;
			return false;
		}

		// Atualiza o progresso no save atual
		return saveManager->updateSaveProgress(currentSaveName, levelIndex);
	}

	std::string GameControl::getCurrentSaveName() const
	{
		return currentSaveName;
	}
}
