#pragma once
#include "Component.h"

class FpsCounterComponent final : public Component
{
public:
	explicit FpsCounterComponent() = default;

	~FpsCounterComponent() = default;
	FpsCounterComponent(const FpsCounterComponent& other) = delete;
	FpsCounterComponent(FpsCounterComponent&& other) = delete;
	FpsCounterComponent& operator=(const FpsCounterComponent& other) = delete;
	FpsCounterComponent& operator=(FpsCounterComponent&& other) = delete;

	void Update() override;
	void FixedUpdate() override {};
	void Render() const override {};

private:
	std::string m_Text;

	const float m_TimeBetweenUpdates = 0.2;
	int m_FramesSinceLastUpdate = 0;
	float m_TimeSinceLastUpdate = 0;
};