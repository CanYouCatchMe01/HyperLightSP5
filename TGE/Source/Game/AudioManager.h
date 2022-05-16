#pragma once
#include "fmod_studio.hpp"
#include "../../Bin/Assets/FMOD/fmod_studio_guids.hpp"
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

	void SetListenerTransform(const Tga2D::Transform& aTransform);
	void SetListenerTransform(const Tga2D::Transform* const aTransform);

	//Play2D right by the listener
	FMOD::Studio::EventInstance* PlayEvent(const FMOD_GUID aEventID);
	
	//Play3D
	FMOD::Studio::EventInstance* PlayEvent(const FMOD_GUID anID, const FMOD_3D_ATTRIBUTES& a3Dattrib);
	void StopEvent(const FMOD_GUID anID);
	void StopAllEvents();
	void PauseAllEvents();
	void StartAllPausedEvents();
	void SetLevelMusic(int aLevel, int aScene);
	FMOD::Studio::EventInstance* GetLevelMusic();
	float GetVolume(const Channels aChannelID) const;
	void SetChannelVolume(const Channels aChannelID, float aVolume);
	void SetVolume(float aVolume);
	void MuteChannel(const Channels aChannelID, bool aValue = true);
	void UnmuteChannel(const Channels aChannelID);
	void MuteAll();

	inline bool GetFriday() { return myIsFriday; }
private:
	//AudioManager();
	//~AudioManager();
	AudioManager(const AudioManager&) = delete;
	AudioManager operator=(const AudioManager&) = delete;
private:

	FMOD_3D_ATTRIBUTES myListenerAttributes;
	Tga2D::Transform const* myListenerTransform;
	FMOD::Studio::EventInstance* myLevelMusic;
	int myCurrentLevel;
	int myCurrentScene;


	SoundContext myContext;
	bool myIsFriday = false;
	std::map<std::string, FMOD_GUID> mySounds;

	std::map<Channels, FMOD::Studio::Bus*> myChannels;
	FMOD::Studio::Bank* myMasterBank = nullptr;
};