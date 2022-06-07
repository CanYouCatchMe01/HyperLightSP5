#pragma once
#include "Component.h"

class CassetTapeComponent : public Component
{
public:
	CassetTapeComponent(const int& aNumber);

	void OnAwake() override;
	void OnStart() override;
	void OnUpdate(float aDeltaTime) override;

	const int& GetNumber();

private:
	const int myNumber;
};

