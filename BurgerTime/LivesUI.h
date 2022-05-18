#pragma once
#include "Mage/Components/Component.h"

class PeterPepper;

class LivesUI final : public Mage::Component
{
public:
	LivesUI(const PeterPepper* pPeterPepper);

	void Initialize() override;
	void Update() override;

private:
	const PeterPepper* m_pPeterPepper;
	std::vector<Mage::GameObject*> m_pLifeObjects;
};
