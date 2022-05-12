#include "stdafx.h"
#include "GameWorld.h"
#include <tga2d/engine.h>
#include <tga2d/error/ErrorManager.h>
#include <tga2d/graphics/GraphicsEngine.h>
#include <tga2d/sprite/sprite.h>
#include <tga2d/drawers/SpriteDrawer.h>
#include <tga2d/texture/TextureManager.h>
#include "PollingStation.h"
#include "SceneManager.h"
#include "MainMenuState.h"
#include "GameState.h"
#include "SoundEngine-FMod/SoundEngine.h"
#ifdef _DEBUG
#include "BaseDebugger.h"
#endif // _DEBUG



GameWorld::GameWorld()
{}

GameWorld::~GameWorld() 
{}

void GameWorld::Init(HWND aHWND)  
{
	//Audio
	SoundEngine::Init("");
	bool ok = false;
	ok = SoundEngine::LoadBank("Assets/FMODBank/Master.strings.bank", 0);
	ok = SoundEngine::LoadBank("Assets/FMODBank/Master.bank", 0);

	SoundEngine::RegisterAllEvents();




	myPollingStation = new PollingStation;
	myPollingStation->Init(aHWND);

	myStateStack.Init(myPollingStation);
	myStateStack.PushState(new MainMenuState(myStateStack, myPollingStation));

	


}

void GameWorld::Update(float aTimeDelta)
{
#ifdef _DEBUG
	myPollingStation->myDebugger.get()->DebugUpdate();
#endif // _DEBUG


	myStateStack.UpdateState(aTimeDelta);
	SoundEngine::Update();
}

void GameWorld::Render()
{
	myStateStack.RenderState();
}