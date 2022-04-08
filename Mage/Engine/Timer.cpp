#include "Mage/MagePCH.h"
#include "Timer.h"

#include <chrono>

class Mage::Timer::TimerImpl
{
public:
	void CalculateTime();

	float GetDeltaTime() const { return m_DeltaTime; }
	float GetFixedTimeStep() const { return m_FixedTimeStep; }

private:
	float m_DeltaTime = 0;
	float m_FixedTimeStep = 0.02f;

	std::chrono::steady_clock::time_point m_LastTime;
};

void Mage::Timer::TimerImpl::CalculateTime()
{
	const auto currentTime = std::chrono::high_resolution_clock::now();
	m_DeltaTime = std::chrono::duration<float>(currentTime - m_LastTime).count();
	m_LastTime = currentTime;
}

Mage::Timer::Timer()
	: m_pImpl{ std::make_unique<TimerImpl>() }
{
}

Mage::Timer::~Timer()
{
}

void Mage::Timer::CalculateTime() const
{
	m_pImpl->CalculateTime();
}

float Mage::Timer::GetDeltaTime() const
{
	return m_pImpl->GetDeltaTime();
}

float Mage::Timer::GetFixedTimeStep() const
{
	return m_pImpl->GetFixedTimeStep();
}