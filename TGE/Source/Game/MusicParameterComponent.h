#pragma once
#include "Component.h"


class MusicParameterComponent : public Component
{
public:
	MusicParameterComponent(const std::string& aParameterName, const float aParameterValue);

	void OnAwake();
	void OnStart();
	void OnUpdate(const float aDeltaTime);

	void ChangeParameter();

private:
	std::string myParameterName;
	float myParameterValue;
};