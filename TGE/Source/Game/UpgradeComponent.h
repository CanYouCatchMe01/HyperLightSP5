#pragma once
#include "Component.h"

class UpgradeComponent : public Component
{
public:
	UpgradeComponent(int anUpgradeIndex);
	~UpgradeComponent();
	void OnAwake();
	void OnStart();
	void OnUpdate(const float aDeltaTime);
	inline int GetIndex() { return myUpgradeIndex; }
private:
	int myUpgradeIndex;
};