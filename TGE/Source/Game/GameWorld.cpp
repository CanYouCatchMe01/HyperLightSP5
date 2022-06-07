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
	mySharedData.myTexture = Tga2D::Engine::GetInstance()->GetTextureManager().GetTexture(L"Sprites/tgalogo_W.dds");
	mySpriteInstance.myPivot = { 0.5f,0.5f };
	mySpriteInstance.myPosition= { 0.5f,0.5f };
	mySpriteInstance.mySizeMultiplier = { 1.f,0.32f };
	myPollingStation = new PollingStation;
	myPollingStation->Init(aHWND);

	myStateStack.Init(myPollingStation);
	myStateStack.PushState(new MainMenuState(myStateStack, myPollingStation));
#ifdef _RETAIL
	Tga2D::Engine::GetInstance()->SetFullScreen(true);
#endif

#ifdef _DEBUG
	myPollingStation->myDebugger.get()->SetStateStack(&myStateStack);
#endif // _DEBUG
}

void GameWorld::Update(float aTimeDelta)
{
#ifdef _DEBUG
	myPollingStation->myDebugger.get()->DebugUpdate();
#endif // _DEBUG

	if (myStartUp)
	{
		myStartUpTimer += aTimeDelta;
		if (myStartUpTimer > myStartUpTime)
			myStartUp = false;

		return;
	}

	myStateStack.UpdateState(aTimeDelta);
	myPollingStation->Update();
}

void GameWorld::Render()
{
	if (myStartUp)
	{
		Tga2D::Engine::GetInstance()->GetGraphicsEngine().GetSpriteDrawer().Draw(mySharedData, mySpriteInstance);
		return;
	}
	myStateStack.RenderState();
}