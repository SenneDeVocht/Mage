#pragma once
#include "Mage/Singleton.h"

class SettingsManager : public Singleton<SettingsManager>
{
public:
	enum class GameMode
	{
		SinglePlayer,
		Multiplayer
	};

	void SetGameMode(GameMode mode) { m_GameMode = mode; }
	GameMode GetGameMode() const { return m_GameMode; }

private:
	friend class Singleton<SettingsManager>;
	SettingsManager() = default;

	GameMode m_GameMode{ GameMode::SinglePlayer };
};
