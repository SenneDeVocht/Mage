#include "BurgerTime/BurgerTimePCH.h"
#include "Commands.h"

PeterPepperDieCommand::PeterPepperDieCommand(PeterPepper* peterPepper)
	: m_pPeterPepper{ peterPepper }
{
}

void PeterPepperDieCommand::Execute()
{
	m_pPeterPepper->Die();
}

PeterPepperGainPointsCommand::PeterPepperGainPointsCommand(PeterPepper* peterPepper)
	: m_pPeterPepper{ peterPepper }
{
}

void PeterPepperGainPointsCommand::Execute()
{
	m_pPeterPepper->GainPoints();
}
