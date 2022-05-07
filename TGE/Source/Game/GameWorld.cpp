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

	SoundEngine::Init("");

	bool ok = false;
	ok = SoundEngine::LoadBank("Assets/FMODBank/Master.strings.bank", 0);
	ok = SoundEngine::LoadBank("Assets/FMODBank/Master.bank", 0);
	ok = SoundEngine::LoadBank("Assets/FMODBank/Music.bank", 0);
	ok = SoundEngine::LoadBank("Assets/FMODBank/Zone 1 - Desert.bank", 0);
	ok = SoundEngine::LoadBank("Assets/FMODBank/Zone 2 - Badlands.bank", 0);
	ok = SoundEngine::LoadBank("Assets/FMODBank/Zone 3 - Forest.bank", 0);

	std::vector<std::string> eventNames;
	SoundEngine::GetEvents(eventNames);

	ok = SoundEngine::RegisterEvent("event:/Music/Zone 3 - BGM", 0);

	SoundEventInstanceHandle eventHandle = SoundEngine::CreateEventInstance(0);

	ok = SoundEngine::PlayEvent(eventHandle);
	ok;
}

void GameWorld::Update(float aTimeDelta)
{
	myStateStack.UpdateState(aTimeDelta);
	SoundEngine::Update();
}

void GameWorld::Render()
{
	myStateStack.RenderState();
}