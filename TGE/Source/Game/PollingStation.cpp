#include "stdafx.h"
#include "PollingStation.h"
#include "SceneManager.h"
#include "BaseDebugger.h"
#include "GameObject.h"

PollingStation::PollingStation()
{
}

void PollingStation::Init(HWND aHWND)
{
	myDebugger = std::make_unique<BaseDebugger>(this);
	myInputMapper = std::make_unique<Input::InputMapper>(aHWND);
	MapKeys();
	mySceneManager = std::make_unique<SceneManager>(this);
}

void PollingStation::MapKeys()
{
	myInputMapper.get()->MapEvent(Input::eInputAction::eHoldArrowKeyDown, Input::eInputEvent::eMoveDown);
	myInputMapper.get()->MapEvent(Input::eInputAction::eHoldArrowKeyUp, Input::eInputEvent::eMoveUp);
	myInputMapper.get()->MapEvent(Input::eInputAction::eHoldArrowKeyLeft, Input::eInputEvent::eMoveLeft);
	myInputMapper.get()->MapEvent(Input::eInputAction::eHoldArrowKeyRight, Input::eInputEvent::eMoveRight);
	myInputMapper.get()->MapEvent(Input::eInputAction::eKey_X, Input::eInputEvent::eAttack);
	myInputMapper.get()->MapEvent(Input::eInputAction::eKey_Space, Input::eInputEvent::eDash);

	myInputMapper.get()->MapEvent(Input::eInputAction::eReleaseArrowUp, Input::eInputEvent::eReleaseUp);
	myInputMapper.get()->MapEvent(Input::eInputAction::eReleaseArrowDown, Input::eInputEvent::eReleaseDown);
	myInputMapper.get()->MapEvent(Input::eInputAction::eReleaseArrowLeft, Input::eInputEvent::eReleaseLeft);
	myInputMapper.get()->MapEvent(Input::eInputAction::eReleaseArrowRight, Input::eInputEvent::eReleaseRight);

	myInputMapper.get()->MapEvent(Input::eInputAction::eKey_Escape, Input::eInputEvent::ePause);

	//------------------------------------------For Menus------------------------------------------
	myInputMapper.get()->MapEvent(Input::eInputAction::eKey_Enter, Input::eInputEvent::eSelect);
	myInputMapper.get()->MapEvent(Input::eInputAction::ePressArrowKeyDown, Input::eInputEvent::eMenuDown);
	myInputMapper.get()->MapEvent(Input::eInputAction::ePressArrowKeyUp, Input::eInputEvent::eMenuUp);
	myInputMapper.get()->MapEvent(Input::eInputAction::ePressArrowKeyLeft, Input::eInputEvent::eMenuLeft);
	myInputMapper.get()->MapEvent(Input::eInputAction::ePressArrowKeyRight, Input::eInputEvent::eMenuRight);
	//------------------------------------------For Menus------------------------------------------
}
