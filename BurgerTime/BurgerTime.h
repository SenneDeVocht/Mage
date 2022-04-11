#pragma once
#include "Mage/Engine/MageGame.h"

namespace Mage
{
	class GameObject;
}

class BurgerTime final : public Mage::MageGame
{
public:
	void LoadGame() const override;

private:
	void CreatePlatform(Mage::GameObject* parent, const glm::vec2& position, bool isLight = false) const;
	void CreateLadder(Mage::GameObject* parent, const glm::vec2& position) const;
};

