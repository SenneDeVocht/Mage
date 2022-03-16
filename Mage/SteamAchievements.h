#pragma once

#include <steamtypes.h>
#include <isteamuserstats.h>

#define _ACH_ID( id, name ) { id, #id, name, "", 0, 0 }
struct Achievement_t
{
	int AchievementID;
	const char* pAchievementID;
	char Name[128];
	char Description[256];
	bool Achieved;
	int IconImage;
};

class SteamAchievements
{
private:
	int64 m_AppID; // Our current AppID
	Achievement_t* m_pAchievements; // Achievements data
	int m_NumAchievements; // The number of Achievements
	bool m_Initialized; // Have we called Request stats and received the callback?

public:
	SteamAchievements(Achievement_t* achievements, int numAchievements);
	~SteamAchievements();

	bool RequestStats();
	bool SetAchievement(const char* id);

	STEAM_CALLBACK(SteamAchievements, OnUserStatsReceived, UserStatsReceived_t, m_CallbackUserStatsReceived);
	STEAM_CALLBACK(SteamAchievements, OnUserStatsStored, UserStatsStored_t, m_CallbackUserStatsStored);
	STEAM_CALLBACK(SteamAchievements, OnAchievementStored, UserAchievementStored_t, m_CallbackAchievementStored);
};