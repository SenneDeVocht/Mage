#pragma once
#include "Component.h"
#include "Observer.h"

enum Achievement
{
    OneGameWon = 0
};

class AchievementsManager : public Component, public Observer
{
public:
	AchievementsManager();
	~AchievementsManager() override;
    
    AchievementsManager(const AchievementsManager& other) = delete;
    AchievementsManager(AchievementsManager&& other) noexcept = delete;
    AchievementsManager& operator=(const AchievementsManager& other) = delete;
    AchievementsManager& operator=(AchievementsManager&& other) noexcept = delete;

    void Notify(Component* component, Event event) override;

private:
	class AchievementsImpl;
	std::unique_ptr<AchievementsImpl> m_pImpl;
};