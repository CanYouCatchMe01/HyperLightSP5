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
#ifndef _RETAIL
	std::unique_ptr<class BaseDebugger> myDebugger;
#endif // _RETAIL
	std::unique_ptr <class AudioManager> myAudioManager;
	std::unique_ptr <class GameDataManager> myGameDataManager;

private:

	void MapKeys();
};