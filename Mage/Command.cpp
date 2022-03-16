#include "MagePCH.h"
#include "Command.h"

#include "PeterPepper.h"

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