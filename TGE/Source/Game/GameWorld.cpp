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
#ifdef _DEBUG
#include "BaseDebugger.h"
#endif // _DEBUG



GameWorld::GameWorld()
{}

GameWorld::~GameWorld() 
{}

void GameWorld::Init(HWND aHWND)  
{
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
	myPollingStation->Update();
}

void GameWorld::Render()
{
	myStateStack.RenderState();
}