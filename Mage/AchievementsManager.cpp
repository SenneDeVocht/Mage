#include "MagePCH.h"
#include "AchievementsManager.h"

#include <array>

#include "PeterPepper.h"
#include "steam_api.h"
#include "SteamAchievements.h"

class AchievementsManager::AchievementsImpl
{
private:
    std::array<Achievement_t, 1> m_Achievements = 
	{
		_ACH_ID(Achievement::OneGameWon, "Winner")
	};

    std::unique_ptr<SteamAchievements> m_pSteamAchievements;

public:
    AchievementsImpl()
        : m_pSteamAchievements{ std::make_unique<SteamAchievements>(m_Achievements.data(), 1) }
    {}

    void SetAchievement(Achievement achievement) const
    {
        std::string asString;

        switch (achievement)
        {
        case Achievement::OneGameWon:
            asString = "ACH_WIN_ONE_GAME";
            break;
        }

        if (m_pSteamAchievements)
            m_pSteamAchievements->SetAchievement(asString.c_str());
    }
};

AchievementsManager::AchievementsManager()
    : m_pImpl{ std::make_unique<AchievementsImpl>() }
{
}

AchievementsManager::~AchievementsManager()
{
}

void AchievementsManager::Notify(Component* component, Event event)
{
    switch (event)
    {
        case Event::PointsChanged:
            PeterPepper* peter = dynamic_cast<PeterPepper*>(component);
            if (peter == nullptr)
                return;

            int points = peter->GetPoints();
            if (points >= 500)
            {
                m_pImpl->SetAchievement(Achievement::OneGameWon);
            }
            
            break;
    }
}
