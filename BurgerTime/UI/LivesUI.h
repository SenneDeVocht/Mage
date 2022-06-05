#pragma once
#include "Mage/Components/Component.h"

class GameManager;

class LivesUI final : public Mage::Component
{
public:
	LivesUI(const GameManager* pPeterPepper);

	void Initialize() override;
	void Update() override;

private:
	const GameManager* m_pGameManager;
	std::vector<Mage::GameObject*> m_pLifeObjects;
};
