#include "stdafx.h"
#include "AudioComponent.h"
#include "fmod_studio_guids.hpp"
#include "GameObject.h"
#include "AudioManager.h"

AudioComponent::~AudioComponent()
{
	for(auto& sound : myEventInstances)
	{
		sound->stop(FMOD_STUDIO_STOP_IMMEDIATE);
		sound->release();
	}
}

void AudioComponent::OnAwake()
{
}

void AudioComponent::OnStart()
{}

void AudioComponent::OnUpdate(const float /*aDeltaTime*/)
{
	//Loop through all the events and delete the ones that are finished
	for (auto it = myEventInstances.begin(); it != myEventInstances.end();)
	{
		FMOD_STUDIO_PLAYBACK_STATE state;
		(*it)->getPlaybackState(&state);
		
		if (state == FMOD_STUDIO_PLAYBACK_STATE::FMOD_STUDIO_PLAYBACK_STOPPED)
		{
			it = myEventInstances.erase(it);
		}
		else
		{
			++it;
		}
	}
	
	//set the 3Dattributes
	Tga2D::Vector3f forward, up;
	forward = myTransform->GetMatrix().GetForward().GetNormalized();
	up = myTransform->GetMatrix().GetUp().GetNormalized();

	my3Dattributes.position = { myTransform->GetPosition().x, myTransform->GetPosition().y, myTransform->GetPosition().z };
	my3Dattributes.forward = { forward.x, forward.y, forward.z };
	my3Dattributes.up = { up.x, up.y, up.z};

	for(auto& audioEvent : myEventInstances)
	{
		audioEvent->set3DAttributes(&my3Dattributes);
	}
}

FMOD::Studio::EventInstance* AudioComponent::PlayEvent3D(FMOD_GUID aEvent)
{
	FMOD::Studio::EventInstance* audioEvent = myPollingStation->myAudioManager->PlayEvent(aEvent, my3Dattributes);
	myEventInstances.push_back(audioEvent);
	return audioEvent;
}

FMOD::Studio::EventInstance* AudioComponent::PlayEvent3D(const char* aEvent)
{
	FMOD::Studio::EventInstance* audioEvent = myPollingStation->myAudioManager->PlayEvent(aEvent, my3Dattributes);
	myEventInstances.push_back(audioEvent);
	return audioEvent;
}
