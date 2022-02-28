#pragma once
#include "Component.h"
class TrashTheCache final : public Component
{
	template<typename componentType, typename... argTypes>
	friend componentType* GameObject::CreateComponent(argTypes&&... args);

public:
	~TrashTheCache() = default;
	TrashTheCache(const TrashTheCache& other) = delete;
	TrashTheCache(TrashTheCache&& other) = delete;
	TrashTheCache& operator=(const TrashTheCache& other) = delete;
	TrashTheCache& operator=(TrashTheCache&& other) = delete;

	void Update() override;
	void FixedUpdate() override {};
	void Render() const override {};

private:
	explicit TrashTheCache() = default;

	struct Transform
	{
		float matrix[16]
		{
		    1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	};
	struct Object
	{
		Transform Transform;
		int ID = 1;
	};
	struct AltObject
	{
		Transform* Transform = nullptr;
		int ID = 1;
	};

	std::vector<int> m_StepSizes = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024 };

	// Integers
	void TrashWithInts();
	void DisplayIntsGraph();

	int m_IntSamples = 10;
	std::vector<float> m_IntTimes;

	// Objects
	void TrashWithObjects();
	void DisplayObjectsGraph();

	int m_ObjectSamples = 10;
	std::vector<float> m_ObjectTimes;

	// AltObjects
	void TrashWithAltObjects();
	void DisplayAltObjectsGraph();

	int m_AltObjectSamples = 10;
	std::vector<float> m_AltObjectTimes;

	void DisplayCombinedObjectsGraph();
};

