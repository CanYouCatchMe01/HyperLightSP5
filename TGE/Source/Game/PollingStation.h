#pragma once
#include <memory>
#include "InputMapper.h"

class Postmaster;

class PollingStation
{
public:
	PollingStation();
	void Init(HWND aHWND);
	void Update();
public:
	class GameObject* myPlayer = nullptr;
	std::unique_ptr<class SceneManager> mySceneManager;
	std::unique_ptr<Input::InputMapper> myInputMapper;
	std::unique_ptr<Postmaster> myPostmaster;
#ifdef _DEBUG
	std::unique_ptr<class BaseDebugger> myDebugger;
#endif // _DEBUG
	std::unique_ptr <class AudioManager> myAudioManager;

private:

	void MapKeys();
};