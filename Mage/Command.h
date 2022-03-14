#pragma once

class PeterPepper;

class Command
{
public:
	virtual ~Command() = default;
	virtual void Execute() = 0;
};

class PeterPepperDieCommand : public Command
{
public:
	PeterPepperDieCommand(PeterPepper* peterPepper);
    void Execute() override;

private:
	PeterPepper* m_pPeterPepper;
};