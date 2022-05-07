#include "SoundEngine.pch.h"
#include "SoundEngineStructs.h"
#include "SoundEngine.h"
#include "SoundEngine.impl"

SoundEventInstanceHandle SoundEventInstanceHandle::InvalidHandle = {"", -1};

SoundEventInstanceHandle::SoundEventInstanceHandle(const std::string& anEvent, int aHandleId)
{
	if(aHandleId >= 0)
	{
		myInstance = aHandleId;
		myEventName = anEvent;
	}
}

int SoundEventInstanceHandle::GetId() const
{
	return myInstance;
}

const std::string& SoundEventInstanceHandle::GetEvent() const
{
	return myEventName;
}

bool SoundEventInstanceHandle::IsValid() const
{
	return !myEventName.empty() && myInstance >= 0;
}
