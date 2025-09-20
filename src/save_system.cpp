#include <last-outpost/game_control.h>
#include <imgui/imgui.h>

namespace Game
{
	void GameControl::renderCreateSaveMenu()
	{
	}

	void GameControl::renderLoadSaveMenu()
	{
		float buttonWidth = 300.0f;
		float buttonHeight = 60.0f;
		float buttonX = (500.0f - buttonWidth) * 0.5f;
		ImVec2 buttonSize(buttonWidth, buttonHeight);

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

		if (availableSaves.empty())
		{
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.0f);
			ImGui::SetCursorPosX(buttonX);
			ImGui::Text("Nenhum save encontrado!");
		}
		else
		{
			for (size_t i = 0; i < availableSaves.size(); i++)
			{
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
				ImGui::SetCursorPosX(buttonX);

				if (ImGui::Button(availableSaves[i].name.c_str(), buttonSize))
				{
					selectedSaveIndex = i;
					loadGame(availableSaves[i].name);
					showLoadSaveMenu = false;
				}
			}
		}

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

		saveManager->createSave(saveName, 0);
		availableSaves = saveManager->getAllSaves();

		currentLevelIndex = 0;
		currentGold = 100;

		currentSaveName = saveName;

		changeState(GameState::Playing);

		return true;
	}

	bool GameControl::loadGame(const std::string &saveName)
	{
		SaveData saveData;
		if (!saveManager->loadSave(saveName, saveData))
		{
			return false;
		}
		currentLevelIndex = saveData.levelIndex;
		if (currentLevelIndex < 0 || currentLevelIndex >= static_cast<int>(levels.size()))
		{
			currentLevelIndex = 0;
		}
		currentSaveName = saveName;

		if (currentLevelIndex == 0)
		{
			currentGold = 100;
		}

		changeState(GameState::Playing);

		return true;
	}

	bool GameControl::updateCurrentSaveProgress(int levelIndex, int newGold)
	{
		if (currentSaveName.empty())
		{
			std::cerr << "No current save to update progress" << std::endl;
			return false;
		}

		return saveManager->updateSaveProgress(currentSaveName, levelIndex, newGold);
	}

	std::string GameControl::getCurrentSaveName() const
	{
		return currentSaveName;
	}
}
