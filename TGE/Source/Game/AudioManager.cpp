#include "stdafx.h"
#include "AudioManager.h"

#ifdef _DEBUG
	#include <exception>
	#include <filesystem>
	#include <iostream>
#endif // _DEBUG

namespace fs = std::filesystem;

AudioManager::AudioManager()
{
	Init();
}

AudioManager::~AudioManager()
{
	myContext.system->release();
}

void AudioManager::Init()
{
	myListenerAttributes = FMOD_3D_ATTRIBUTES();
	myListenerAttributes.up = { 0,1,0 };
	myListenerAttributes.forward = { 0,0,1 };

	myChannels.insert({ Channels::Master, nullptr });
	myChannels.insert({ Channels::Music, nullptr });
	myChannels.insert({ Channels::SFX, nullptr });

	FMOD::Studio::System::create(&myContext.system);
	FMOD::System_Create(&myContext.coreSystem);

	myContext.system->getCoreSystem(&myContext.coreSystem);

	myContext.system->initialize(512, FMOD_INIT_NORMAL, 0, nullptr);
	myContext.coreSystem->init(512, FMOD_INIT_NORMAL, 0);

	myContext.system->loadBankFile("Assets/FMOD/Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &myMasterBank);
	myContext.system->loadBankFile("Assets/FMOD/Master.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &myMasterStringBank);

	myContext.system->getBusByID(&FSPRO::Bus::Master_Bus, &myChannels.at(Channels::Master));
	myContext.system->getBusByID(&FSPRO::Bus::music, &myChannels.at(Channels::Music));
	myContext.system->getBusByID(&FSPRO::Bus::sfx, &myChannels.at(Channels::SFX));

	myChannels.at(Channels::Master)->setVolume(1.0f);
	myChannels.at(Channels::Music)->setVolume(0.5f);
	myChannels.at(Channels::SFX)->setVolume(1.0f);
}

void AudioManager::Update()
{
	//Need to set the music position where the camere is every frame
	myMusicInstance->set3DAttributes(&myListenerAttributes);

	myContext.system->update();
}

void AudioManager::SetListenerAttributes(const Tga2D::Transform& aTransform)
{
	Tga2D::Vector3f forward, up;
	forward = aTransform.GetMatrix().GetForward().GetNormalized();
	up = aTransform.GetMatrix().GetUp().GetNormalized();

	myListenerAttributes.position = { aTransform.GetPosition().x, aTransform.GetPosition().y, aTransform.GetPosition().z };
	myListenerAttributes.forward = { forward.x, forward.y, forward.z };
	myListenerAttributes.up = { up.x, up.y, up.z };

	myContext.system->setListenerAttributes(0, &myListenerAttributes);
}

FMOD::Studio::EventInstance* AudioManager::PlayEvent(const FMOD_GUID anID)
{
	return PlayEvent(anID, myListenerAttributes);
}

FMOD::Studio::EventInstance* AudioManager::PlayEvent(const char* anEvent)
{
	return PlayEvent(anEvent, myListenerAttributes);
}

FMOD::Studio::EventInstance* AudioManager::PlayEvent(const FMOD_GUID anID, const FMOD_3D_ATTRIBUTES& a3Dattrib)
{
	FMOD::Studio::EventInstance* instance;
	FMOD::Studio::EventDescription* eventDesc;
	myContext.system->getEventByID(&anID, &eventDesc);
	eventDesc->createInstance(&instance);

	instance->set3DAttributes(&a3Dattrib);

	instance->start();
	instance->release();
	return instance;
}

FMOD::Studio::EventInstance* AudioManager::PlayEvent(const char* anEvent, const FMOD_3D_ATTRIBUTES& a3Dattrib)
{
	FMOD::Studio::EventInstance* instance;
	FMOD::Studio::EventDescription* eventDesc;
	myContext.system->getEvent(anEvent, &eventDesc);
	eventDesc->createInstance(&instance);

	instance->set3DAttributes(&a3Dattrib);

	instance->start();
	instance->release();
	return instance;
}


void AudioManager::StopEvent(const FMOD_GUID anID)
{
	FMOD::Studio::EventInstance* instance;
	FMOD::Studio::EventDescription* eventDesc;
	myContext.system->getEventByID(&anID, &eventDesc);
	eventDesc->createInstance(&instance);

	instance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
	instance->release();
}

void AudioManager::StopAllEvents()
{
	myChannels.at(Channels::Master)->stopAllEvents(FMOD_STUDIO_STOP_ALLOWFADEOUT);
	myChannels.at(Channels::Music)->stopAllEvents(FMOD_STUDIO_STOP_ALLOWFADEOUT);
	myChannels.at(Channels::SFX)->stopAllEvents(FMOD_STUDIO_STOP_ALLOWFADEOUT);
}

void AudioManager::PauseAllEvents()
{
	myChannels.at(Channels::Master)->setPaused(true);
}

void AudioManager::StartAllPausedEvents()
{
	myChannels.at(Channels::Master)->setPaused(false);
}

void AudioManager::SetMusic(const char* aMusicEvent)
{
	myMusicInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
	myMusicInstance = PlayEvent(aMusicEvent);
}

void AudioManager::SetMusic(const FMOD_GUID anID)
{
	myMusicInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
	myMusicInstance = PlayEvent(anID);
}

FMOD::Studio::EventInstance* AudioManager::GetMusicInstance()
{
	return myMusicInstance;
}

FMOD::Studio::EventInstance* AudioManager::GetLevelMusic()
{
	return myMusicInstance;
}

void AudioManager::SetChannelVolume(const Channels aChannelID, float aVolume)
{
	auto& bus = myChannels.at(aChannelID);
	bus->setMute(false);
	bus->setVolume(aVolume);
}

void AudioManager::SetVolume(float aVolume)
{
	auto& bus = myChannels.at(Channels::Master);
	bus->setMute(false);
	bus->setVolume(aVolume);
}

void AudioManager::MuteAll()
{
	for (const auto& chan : myChannels)
	{
		chan.second->setMute(true);
	}
}

void AudioManager::MuteChannel(const Channels aChannelID, bool aValue)
{
	myChannels.at(aChannelID)->setMute(aValue);
}

void AudioManager::UnmuteChannel(const Channels aChannelID)
{
	myChannels.at(aChannelID)->setMute(false);
}

float AudioManager::GetVolume(const Channels aChannelID) const
{
	float volume;
	myChannels.at(aChannelID)->getVolume(&volume);
	return volume;
}