#pragma once
#include "Singleton.h"

class Timer final : public Singleton<Timer>
{
public:
	void SetDeltaTime(float deltaTime) { m_DeltaTime = deltaTime; }
	float GetDeltaTime() const { return m_DeltaTime; }

	void SetFixedTimeStep(float fixedTimeStep) { m_FixedTimeStep = fixedTimeStep; }
	float GetFixedTimeStep() const { return m_FixedTimeStep; }

private:
	friend class Singleton<Timer>;
	Timer() = default;

	float m_DeltaTime = 0;
	float m_FixedTimeStep = 0;
};