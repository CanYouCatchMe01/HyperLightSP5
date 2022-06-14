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
{
}

GameWorld::~GameWorld()
{
}

void GameWorld::Init(HWND aHWND)
{
	mySharedData.myTexture = Tga2D::Engine::GetInstance()->GetTextureManager().GetTexture(L"Sprites/tgalogo_W.dds");
	mySpriteInstance.myPivot = { 0.5f,0.5f };
	mySpriteInstance.myPosition = { 0.5f,0.5f };
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

	myPollingStation->Update();
	if (!myGroupStartUp)
	{
		myStateStack.UpdateState(aTimeDelta);
		return;
	}
	if (GetAsyncKeyState(VK_ESCAPE))
	{
		myGroupStartUp = false;
		myTGAStartUp = false;
		return;
	}

	if (myTGAStartUp)
	{
		mySpriteInstance.myColor = { 1,1,1, myAlpha };
		if (myFadeOut)
		{
			myAlpha -= aTimeDelta * 1.5f;

			if (myAlpha <= -0.5f)
			{
				myFadeOut = false;
				myTGAStartUp = false;
				myAlpha = 0;
				mySpriteInstance.mySize = { 0.5f,0.5f };
				mySpriteInstance.mySizeMultiplier = 1.f;
				myStartUpTimer = 0;
				mySharedData.myTexture = Tga2D::Engine::GetInstance()->GetTextureManager().GetTexture(L"Sprites/groupLogo.dds");
			}
			return;
		}

		if (myAlpha < 1.5f)
			myAlpha += aTimeDelta / 2;

		if (myAlpha >= 1)
		{
			myAlpha = 1;
		}

		myStartUpTimer += aTimeDelta;
		if (myStartUpTimer > myStartUpTime)
		{
			myFadeOut = true;
			myStartUpTimer = 0;
		}

		return;
	}
	if (myGroupStartUp)
	{
		mySpriteInstance.myColor = { 1,1,1, myAlpha };
		if (myFadeOut)
		{
			myAlpha -= aTimeDelta * .5f;

			if (myAlpha <= -0.75f)
			{
				myFadeOut = false;
				myGroupStartUp = false;
			}
			return;
		}

		if (myAlpha < 1)
			myAlpha += aTimeDelta / 2;

		if (myAlpha > 1)
		{
			myFadeOut = true;
		}
		return;
	}


}

void GameWorld::Render()
{
	if (myTGAStartUp || myGroupStartUp)
	{
		Tga2D::Engine::GetInstance()->GetGraphicsEngine().GetSpriteDrawer().Draw(mySharedData, mySpriteInstance);
		return;
	}
	myStateStack.RenderState();
}