#include "stdafx.h"
#include "AudioComponent.h"
#include "../../Bin/Assets/FMOD/fmod_studio_guids.hpp"
#include "GameObject.h"
#include "AudioManager.h"

AudioComponent::~AudioComponent()
{
	Stop();
}

void AudioComponent::OnAwake()
{
	myEventInstance = nullptr;
}

void AudioComponent::OnStart()
{}

void AudioComponent::OnUpdate(const float /*aDeltaTime*/)
{
	Tga2D::Vector3f forward, up;
	forward = myTransform->GetMatrix().GetForward().GetNormalized();
	up = myTransform->GetMatrix().GetUp().GetNormalized();

	my3Dattributes.position = { myTransform->GetPosition().x, myTransform->GetPosition().y, myTransform->GetPosition().z };
	my3Dattributes.forward = { forward.x, forward.y, forward.z };
	my3Dattributes.up = { up.x, up.y, up.z};

	myEventInstance->set3DAttributes(&my3Dattributes);
	//myEventInstance->getParameterByName("Health", &value);
}

void AudioComponent::PlayEvent(FMOD_GUID aEvent, bool aStopPrevious)
{
	if (aStopPrevious && myEventInstance)
	{
		myEventInstance->stop( FMOD_STUDIO_STOP_ALLOWFADEOUT);
	}

	myEventInstance = myPollingStation->myAudioManager->PlayEvent(aEvent, my3Dattributes);
}

void AudioComponent::SetParameter(const char* aName, const float aValue)
{
	if (!myEventInstance)
	{
		return;
	}
	myEventInstance->setParameterByName(aName, aValue);
}

void AudioComponent::Stop()
{
	if (!myEventInstance)
	{
		return;
	}
	myEventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
}

void AudioComponent::Pause()
{
	if (!myEventInstance)
	{
		return;
	}
	myEventInstance->setPaused(true);
}

void AudioComponent::Resume()
{
	if (!myEventInstance)
	{
		return;
	}
	myEventInstance->setPaused(false);
}

void AudioComponent::SetVolume(float aVolume)
{
	if (!myEventInstance)
	{
		return;
	}
	myEventInstance->setVolume(aVolume);
}
