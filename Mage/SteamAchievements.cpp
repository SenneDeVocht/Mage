#include "MagePCH.h"
#include "SteamAchievements.h"

#include <isteamuser.h>
#include <isteamutils.h>

SteamAchievements::SteamAchievements(Achievement_t* achievements, int numAchievements) :
    m_AppID(0),
    m_Initialized(false),
    m_CallbackUserStatsReceived(this, &SteamAchievements::OnUserStatsReceived),
    m_CallbackUserStatsStored(this, &SteamAchievements::OnUserStatsStored),
    m_CallbackAchievementStored(this, &SteamAchievements::OnAchievementStored)
{
    m_AppID = SteamUtils()->GetAppID();
    m_pAchievements = achievements;
    m_NumAchievements = numAchievements;
    RequestStats();
}

bool SteamAchievements::RequestStats()
{
	// Is Steam loaded? If not we can't get stats.
	if (SteamUserStats() == nullptr || SteamUser() == nullptr)
	{
		return false;
	}
	// Is the user logged on?  If not we can't get stats.
	if (!SteamUser()->BLoggedOn())
	{
		return false;
	}
	// Request user stats.
	return SteamUserStats()->RequestCurrentStats();
}

bool SteamAchievements::SetAchievement(const char* id)
{
	// Have we received a call back from Steam yet?
	if (m_Initialized)
	{
		SteamUserStats()->SetAchievement(id);
		return SteamUserStats()->StoreStats();
	}
	// If not then we can't set achievements yet
	return false;
}

void SteamAchievements::OnUserStatsReceived(UserStatsReceived_t* pCallback)
{
	// we may get callbacks for other games' stats arriving, ignore them
	if ((uint64)m_AppID == pCallback->m_nGameID)
	{
		if (k_EResultOK == pCallback->m_eResult)
		{
			std::cout << "Received stats and achievements from Steam\n" << std::endl;
			m_Initialized = true;

			// load achievements
			for (int iAch = 0; iAch < m_NumAchievements; ++iAch)
			{
				Achievement_t& ach = m_pAchievements[iAch];

				SteamUserStats()->GetAchievement(ach.pAchievementID, &ach.Achieved);
				_snprintf_s(ach.Name, sizeof(ach.Name), "%s",
					SteamUserStats()->GetAchievementDisplayAttribute(ach.pAchievementID,
						"name"));
				_snprintf_s(ach.Description, sizeof(ach.Description), "%s",
					SteamUserStats()->GetAchievementDisplayAttribute(ach.pAchievementID,
						"desc"));
			}
		}
		else
		{
			std::cout << "RequestStats - failed, " << pCallback->m_eResult << std::endl;
		}
	}
}

void SteamAchievements::OnUserStatsStored(UserStatsStored_t* pCallback)
{
	// we may get callbacks for other games' stats arriving, ignore them
	if ((uint64)m_AppID == pCallback->m_nGameID)
	{
		if (k_EResultOK == pCallback->m_eResult)
		{
			std::cout << "Stored stats for Steam" << std::endl;
		}
		else
		{
			std::cout << "StatsStored - failed, " << pCallback->m_eResult << std::endl;
		}
	}
}

void SteamAchievements::OnAchievementStored(UserAchievementStored_t* pCallback)
{
	// we may get callbacks for other games' stats arriving, ignore them
	if ((uint64)m_AppID == pCallback->m_nGameID)
	{
		std::cout << "Stored Achievement for Steam" << std::endl;
	}
}