#include "stdafx.h"
#include "AudioManager.h"
#include "../../Bin/Assets/FMOD/fmod_studio_guids.hpp"

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

	time_t now = time(0);
	tm zone;
	localtime_s(&zone, &now);
	if (zone.tm_wday == 5)
	{
		myIsFriday = true;
	}

	myChannels.insert({ Channels::Master, nullptr });
	myChannels.insert({ Channels::Music, nullptr });
	myChannels.insert({ Channels::SFX, nullptr });

	FMOD::Studio::System::create(&myContext.system);
	FMOD::System_Create(&myContext.coreSystem);

	myContext.system->getCoreSystem(&myContext.coreSystem);

	myContext.system->initialize(512, FMOD_INIT_NORMAL, 0, nullptr);
	myContext.coreSystem->init(512, FMOD_INIT_NORMAL, 0);

	myContext.system->loadBankFile("Assets/FMOD/Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &myMasterBank);

	myContext.system->getBusByID(&FSPRO::Bus::Master_Bus, &myChannels.at(Channels::Master));
	myContext.system->getBusByID(&FSPRO::Bus::Musik, &myChannels.at(Channels::Music));
	myContext.system->getBusByID(&FSPRO::Bus::FX, &myChannels.at(Channels::SFX));

	myChannels.at(Channels::Master)->setVolume(0.5f);
	myChannels.at(Channels::Music)->setVolume(0.5f);
	myChannels.at(Channels::SFX)->setVolume(0.5f);

	myLevelMusic = nullptr;
	myCurrentLevel = 0;
}

void AudioManager::Update()
{
	if (myListenerTransform)
	{
		Tga2D::Vector3f forward, up;
		forward = myListenerTransform->GetMatrix().GetForward().GetNormalized();
		up = myListenerTransform->GetMatrix().GetUp().GetNormalized();

		myListenerAttributes.position = { myListenerTransform->GetPosition().x, myListenerTransform->GetPosition().y, myListenerTransform->GetPosition().z };
		myListenerAttributes.forward = { forward.x, forward.y, forward.z };
		myListenerAttributes.up = { up.x, up.y, up.z };

		myContext.system->setListenerAttributes(0, &myListenerAttributes);
	}


	myContext.system->update();
}

void AudioManager::SetListenerTransform(const Tga2D::Transform& aTransform)
{
	myListenerTransform = &aTransform;
	//myContext.system->setListenerAttributes(0, &myListenerAttributes);
	//myContext.system->setListenerAttributes(0, &attrib);
}

void AudioManager::SetListenerTransform(const Tga2D::Transform* const aTransform)
{
	myListenerTransform = aTransform;
}

FMOD::Studio::EventInstance* AudioManager::PlayEvent(const FMOD_GUID anID, const bool /*aPlayOnce*/)
{
	FMOD::Studio::EventInstance* instance;
	FMOD::Studio::EventDescription* eventDesc;
	myContext.system->getEventByID(&anID, &eventDesc);
	eventDesc->createInstance(&instance);

	instance->start();
	instance->release();
	return instance;
}

FMOD::Studio::EventInstance* AudioManager::PlayEvent(const FMOD_GUID anID, const FMOD_3D_ATTRIBUTES& a3Dattrib, const bool /*aPlayOnce*/)
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



void AudioManager::SetLevelMusic(int aLevel, int aScene)
{
	if (myLevelMusic)
	{
		if (aLevel != myCurrentLevel || (myCurrentLevel == 1 && myCurrentScene == 1) || (aLevel == 1 && aScene == 1)
			|| (aLevel == 4 && aScene == 4) || (myCurrentLevel == 4 && myCurrentScene == 4))
			myLevelMusic->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
		else
			return;
	}
	myCurrentLevel = aLevel;
	myCurrentScene = aScene;
	
	//switch (aLevel)
	//{
	//case 0:
	//	myLevelMusic = PlayEvent(FSPRO::Event::LevelMusic_MainMenu);
	//	break;
	//case 1:
	//{
	//	switch (aScene)
	//	{
	//	case 1:
	//		myLevelMusic = PlayEvent(FSPRO::Event::LevelMusic_Level1Intro);
	//		break;
	//	default:
	//		myLevelMusic = PlayEvent(FSPRO::Event::LevelMusic_Level1);
	//		break;
	//	}
	//	break;
	//}
	//case 2:
	//	myLevelMusic = PlayEvent(FSPRO::Event::LevelMusic_Level2);
	//	break;
	//case 3:
	//	myLevelMusic = PlayEvent(FSPRO::Event::LevelMusic_Level3);
	//	break;
	//case 4:
	//{
	//	switch (aScene)
	//	{
	//	case 4:
	//		myLevelMusic = PlayEvent(FSPRO::Event::LevelMusic_LevelOutro);
	//		break;
	//	default:
	//		myLevelMusic = PlayEvent(FSPRO::Event::LevelMusic_Level4);
	//		break;
	//	}
	//	break;
	//}
	//default:
	//	myLevelMusic->stop(FMOD_STUDIO_STOP_MODE::FMOD_STUDIO_STOP_ALLOWFADEOUT);
	//	break;
	//}

}

FMOD::Studio::EventInstance* AudioManager::GetLevelMusic()
{
	return myLevelMusic;
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