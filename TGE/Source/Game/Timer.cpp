#include "stdafx.h"
#include "Timer.h"

Timer::Timer()
	: myIsOn(false)
	, myDuration(1.f)
	, myElapsedTime(0.f)
	, myCallback()
{
}

Timer Timer::Start()
{
	myIsOn = true;
	return *this;
}
Timer Timer::Stop()
{
	myIsOn = false;
	return *this;
}

Timer Timer::Reset()
{
	myElapsedTime = 0.f;
	return *this;
}

void Timer::Update(const float aDt)
{
	if (!myIsOn)
		return;

	if (myElapsedTime < myDuration)
		myElapsedTime += aDt;

	if (myElapsedTime <= myDuration)
		return;

	if (myCallback)
		myCallback();

	myElapsedTime = myDuration;
	myIsOn = false;
}

Timer Timer::SetDuration(const float aDurationInSeconds)
{
	if (aDurationInSeconds > 0.f)
		myDuration = aDurationInSeconds;
	return *this;
}
Timer Timer::SetCallback(const CallbackType& aCallback)
{
	myCallback = aCallback;
	return *this;
}

bool Timer::IsOn() const
{
	return myIsOn;
}

float Timer::GetCompletionRatio() const
{
	return myElapsedTime / myDuration;
}
