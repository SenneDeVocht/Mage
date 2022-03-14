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
