#pragma once
#include "Mage/Input/Command.h"
#include "PeterPepper.h"

class PeterPepperDieCommand : public Mage::Command
{
public:
	PeterPepperDieCommand(PeterPepper* peterPepper);
	void Execute() override;

private:
	PeterPepper* m_pPeterPepper;
};

class PeterPepperGainPointsCommand : public Mage::Command
{
public:
	PeterPepperGainPointsCommand(PeterPepper* peterPepper);
	void Execute() override;

private:
	PeterPepper* m_pPeterPepper;
};