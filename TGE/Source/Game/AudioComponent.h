#pragma once
#include "Component.h"
#include "fmod.hpp"
#include "../../Bin/Assets/FMOD/fmod_studio_guids.hpp"
#include "fmod_studio.hpp"

class AudioComponent : public Component
{
public:
	~AudioComponent();
	void OnAwake() override;
	void OnStart() override;
	void OnUpdate(const float aDeltaTime) override;

	void PlayEvent(FMOD_GUID aEvent, bool aStopPrevious=true);

	void SetParameter(const char* aName, const float value);

	void Stop();
	void Pause();
	void Resume();
	void SetVolume(float aVolume);

private:
	FMOD::Studio::EventInstance* myEventInstance;
	FMOD_3D_ATTRIBUTES my3Dattributes;
};