#pragma once
#include <memory>
#include "InputMapper.h"
class PollingStation
{
public:
	PollingStation();
	void Init(HWND aHWND);
public:
	class GameObject* myPlayer = nullptr;
	std::unique_ptr<class SceneManager> mySceneManager;
	std::unique_ptr<Input::InputMapper> myInputMapper;
#ifdef _DEBUG
	std::unique_ptr<class BaseDebugger> myDebugger;
#endif // _DEBUG


private:

	void MapKeys();
};