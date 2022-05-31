#include "stdafx.h"
#include "MusicParameterComponent.h"
#include "GameObject.h"
#include "AudioManager.h"

MusicParameterComponent::MusicParameterComponent(const std::string& aParameterName, const float aParameterValue) :
	myParameterName(aParameterName), myParameterValue(aParameterValue)
{
}

void MusicParameterComponent::OnAwake()
{
}

void MusicParameterComponent::OnStart()
{
}

void MusicParameterComponent::OnUpdate(const float /*aDeltaTime*/)
{
}

void MusicParameterComponent::ChangeParameter()
{
	auto result = myPollingStation->myAudioManager.get()->GetMusicInstance()->setParameterByName(myParameterName.c_str(), myParameterValue);
	if (result != FMOD_OK)
	{
		ERROR_PRINT("%s %s", "MusicParameterComponent: Failed to set parameter", myParameterName.c_str());
	}
}
