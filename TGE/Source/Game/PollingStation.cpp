#include "stdafx.h"
#include "PollingStation.h"
#include "SceneManager.h"
#include "Postmaster.h"
#include "GameDataManager.h"
#include "GameObject.h"
#ifndef _RETAIL
#include "BaseDebugger.h"
#endif // _RETAIL

#include "AudioManager.h"

PollingStation::PollingStation()
{
}

void PollingStation::Init(HWND aHWND)
{
#ifndef _RETAIL
	myDebugger = std::make_unique<BaseDebugger>(this);
#endif // _RETAIL

	myAudioManager = std::make_unique<AudioManager>();
	myGameDataManager = std::make_unique<GameDataManager>();
	myInputMapper = std::make_unique<Input::InputMapper>(aHWND);
	MapKeys();
	myGameDataManager = std::make_unique<GameDataManager>();
	mySceneManager = std::make_unique<SceneManager>(this);
	myPostmaster = std::make_unique<Postmaster>();
	

}

void PollingStation::Update()
{
	myInputMapper.get()->Update();
	myAudioManager.get()->Update();
}

void PollingStation::MapKeys()
{
	myInputMapper.get()->MapEvent(Input::eInputAction::eHoldArrowKeyDown, Input::eInputEvent::eMoveDown);
	myInputMapper.get()->MapEvent(Input::eInputAction::eHoldArrowKeyUp, Input::eInputEvent::eMoveUp);
	myInputMapper.get()->MapEvent(Input::eInputAction::eHoldArrowKeyLeft, Input::eInputEvent::eMoveLeft);
	myInputMapper.get()->MapEvent(Input::eInputAction::eHoldArrowKeyRight, Input::eInputEvent::eMoveRight);
	myInputMapper.get()->MapEvent(Input::eInputAction::eHoldS, Input::eInputEvent::eMoveDown);
	myInputMapper.get()->MapEvent(Input::eInputAction::eHoldW, Input::eInputEvent::eMoveUp);
	myInputMapper.get()->MapEvent(Input::eInputAction::eHoldA, Input::eInputEvent::eMoveLeft);
	myInputMapper.get()->MapEvent(Input::eInputAction::eHoldD, Input::eInputEvent::eMoveRight);
	myInputMapper.get()->MapEvent(Input::eInputAction::eKey_X, Input::eInputEvent::eAttack);
	myInputMapper.get()->MapEvent(Input::eInputAction::eKey_Space, Input::eInputEvent::eDash);
	myInputMapper.get()->MapEvent(Input::eInputAction::eKey_C, Input::eInputEvent::eHeal);

	myInputMapper.get()->MapEvent(Input::eInputAction::eReleaseArrowUp, Input::eInputEvent::eReleaseArrowUp);
	myInputMapper.get()->MapEvent(Input::eInputAction::eReleaseArrowDown, Input::eInputEvent::eReleaseArrowDown);
	myInputMapper.get()->MapEvent(Input::eInputAction::eReleaseArrowLeft, Input::eInputEvent::eReleaseArrowLeft);
	myInputMapper.get()->MapEvent(Input::eInputAction::eReleaseArrowRight, Input::eInputEvent::eReleaseArrowRight);

	myInputMapper.get()->MapEvent(Input::eInputAction::eKey_Escape, Input::eInputEvent::eEscape);
	myInputMapper.get()->MapEvent(Input::eInputAction::ePressM, Input::eInputEvent::eMap);

	//------------------------------------------For Menus------------------------------------------
	myInputMapper.get()->MapEvent(Input::eInputAction::eKey_Enter, Input::eInputEvent::eSelect);
	myInputMapper.get()->MapEvent(Input::eInputAction::ePressArrowKeyDown, Input::eInputEvent::eMenuDown);
	myInputMapper.get()->MapEvent(Input::eInputAction::ePressArrowKeyUp, Input::eInputEvent::eMenuUp);
	myInputMapper.get()->MapEvent(Input::eInputAction::ePressArrowKeyLeft, Input::eInputEvent::eMenuLeft);
	myInputMapper.get()->MapEvent(Input::eInputAction::ePressArrowKeyRight, Input::eInputEvent::eMenuRight);
	//------------------------------------------For Menus------------------------------------------

	//-----------------------------------------For Cheaters----------------------------------------
	myInputMapper.get()->MapEvent(Input::eInputAction::ePressL, Input::eInputEvent::eGodMode);
	//-----------------------------------------For Cheaters----------------------------------------
}
