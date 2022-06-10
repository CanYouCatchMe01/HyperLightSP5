#pragma once
#include "fmod_studio.hpp"
#include "fmod_studio_guids.hpp"
#include <map>
#include <tga2d/math/Transform.h>

enum class Channels
{
	Master,
	SFX,
	Music,
};

struct SoundContext
{
	FMOD::System* coreSystem = NULL;
	FMOD::Studio::System* system = NULL;
};

class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	void Init();
	void Update();

	void SetListenerAttributes(const Tga2D::Transform& aTransform);

	//Play2D right by the listener
	FMOD::Studio::EventInstance* PlayEvent(const FMOD_GUID aEventID);
	FMOD::Studio::EventInstance* PlayEvent(const char* anEvent);
	
	//Play3D
	FMOD::Studio::EventInstance* PlayEvent(const FMOD_GUID anID, const FMOD_3D_ATTRIBUTES& a3Dattrib);
	FMOD::Studio::EventInstance* PlayEvent(const char* anEvent, const FMOD_3D_ATTRIBUTES& a3Dattrib);
	void StopEvent(const FMOD_GUID anID);
	void StopAllEvents();
	void PauseAllEvents();
	void StartAllPausedEvents();
	
	//music
	void SetMusic(const char* aMusicEvent);
	void SetMusic(const FMOD_GUID anID);
	FMOD::Studio::EventInstance* GetMusicInstance();
	
	FMOD::Studio::EventInstance* GetLevelMusic();
	float GetVolume(const Channels aChannelID) const;
	void SetChannelVolume(const Channels aChannelID, float aVolume);
	void SetVolume(float aVolume);
	void MuteChannel(const Channels aChannelID, bool aValue = true);
	void UnmuteChannel(const Channels aChannelID);
	void MuteAll();

private:
	AudioManager(const AudioManager&) = delete;
	AudioManager operator=(const AudioManager&) = delete;
private:

	FMOD_3D_ATTRIBUTES myListenerAttributes;
	FMOD::Studio::EventInstance* myMusicInstance;

	SoundContext myContext;

	std::map<Channels, FMOD::Studio::Bus*> myChannels;
	FMOD::Studio::Bank* myMasterBank = nullptr;
	FMOD::Studio::Bank* myMasterStringBank = nullptr;
};