#include "BurgerTime/BurgerTimePCH.h"
#include "SavedScoreManager.h"

#include <fstream>

#include "Mage/ResourceManagement/ResourceManager.h"

int SavedScoreManager::GetSavedScore()
{
	const auto root = Mage::ResourceManager::GetInstance().GetDataRoot();
	const auto filePath = root + "highscore.bin";

	std::ifstream file(filePath, std::ios::binary);

	if (!file.is_open())
		return 0;

	uint32_t score;
	file.read(reinterpret_cast<char*>(&score), sizeof(uint32_t));

	return static_cast<int>(score);
}

void SavedScoreManager::SetSavedScore(int score)
{
	const auto root = Mage::ResourceManager::GetInstance().GetDataRoot();
	const auto filePath = root + "highscore.bin";

	std::ofstream file(filePath, std::ios::binary);

	if (!file.is_open())
		return;

	uint32_t scoreToSave = static_cast<uint32_t>(score);
	file.write(reinterpret_cast<char*>(&scoreToSave), sizeof(uint32_t));
}