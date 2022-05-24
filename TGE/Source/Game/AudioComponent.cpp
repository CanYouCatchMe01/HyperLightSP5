#include "stdafx.h"
#include "AudioComponent.h"
#include "../../Bin/Assets/FMOD/fmod_studio_guids.hpp"
#include "GameObject.h"
#include "AudioManager.h"

AudioComponent::~AudioComponent()
{
}

void AudioComponent::OnAwake()
{
}

void AudioComponent::OnStart()
{}

void AudioComponent::OnUpdate(const float /*aDeltaTime*/)
{
	//Loop through all the events and delete the ones that are finished
	for (int i = 0; i < myEventInstances.size(); i++)
	{
		FMOD_STUDIO_PLAYBACK_STATE state;
		myEventInstances[i]->getPlaybackState(&state);

		if (state == FMOD_STUDIO_PLAYBACK_STATE::FMOD_STUDIO_PLAYBACK_STOPPED)
		{
			myEventInstances[i]->release();

			//swap myEventInstances[i] with the last element
			std::swap(myEventInstances[i], myEventInstances[myEventInstances.size() - 1]);

			//pop the last element
			myEventInstances.pop_back();
		}
	}
	
	//set the 3Dattribues
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
