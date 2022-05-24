#pragma once

// stdlib
#include <functional>

class Timer
{
public:
	using CallbackType = std::function<void()>;

	Timer();

	Timer Start();
	Timer Stop();
	Timer Reset();
	Timer SetDuration(const float aDurationInSeconds);
	Timer SetCallback(const CallbackType& aCallback);

	void Update(const float aDt);

	bool IsOn() const;
	float GetCompletionRatio() const;

private:
	bool myIsOn;
	float myDuration; // in seconds
	float myElapsedTime; // in seconds
	CallbackType myCallback;
};