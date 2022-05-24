#pragma once
#include "Component.h"


class MusicChangeComponent : public Component
{
public:
	MusicChangeComponent(const std::string& aMusicName);

	void OnAwake();
	void OnStart();
	void OnUpdate(const float aDeltaTime);

private:
	std::string myMusicName;
};

