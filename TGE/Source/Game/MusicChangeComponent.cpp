#include "stdafx.h"
#include "MusicChangeComponent.h"
#include "AudioManager.h"

MusicChangeComponent::MusicChangeComponent(const std::string& aMusicName) : myMusicName(aMusicName)
{
}

void MusicChangeComponent::OnAwake()
{
	myPollingStation->myAudioManager.get()->SetMusic(myMusicName.c_str());
}

void MusicChangeComponent::OnStart()
{
	
}

void MusicChangeComponent::OnUpdate(const float /*aDeltaTime*/)
{
}
