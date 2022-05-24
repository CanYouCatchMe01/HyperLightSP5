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

	FMOD::Studio::EventInstance* PlayEvent3D(FMOD_GUID aEvent);
	FMOD::Studio::EventInstance* PlayEvent3D(const char* aEvent);

private:
	std::vector<FMOD::Studio::EventInstance*> myEventInstances;
	FMOD_3D_ATTRIBUTES my3Dattributes;
};