#include "stdafx.h"
#include "DialogueState.h"
#include "PollingStation.h"
#include "SceneManager.h"
#include "StateStack.h"
#include "GameDataManager.h"
#include "Scene.h"

DialogueState::DialogueState(StateStack& aStateStack, PollingStation* aPollingStation)
	:
	State(aStateStack, aPollingStation), 
	myDialogueBox({0.5f, 0.8f},{1.5f,0.75f},L"sprites/UI/Menus/NarrativeUI/ui_nar_textBox.dds"),
	myAstralProjection({0.7f,0.7f},{-1.f,1.f}, L"sprites/UI/Menus/NarrativeUI/ui_nar_astralPortrait.dds")
{
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eSelect, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eDash, this);
	myText = Tga2D::Text(L"Text/Nectar.ttf", Tga2D::FontSize_18);

	GameData& currentData = myPollingStation->myGameDataManager.get()->GetGameData();

	if (myPollingStation->mySceneManager.get()->GetActiveScene()->name == "Tutorial")
	{
		myDialogues.push_back("Elise");
		myDialogues.push_back("...Awaken!");
		myDialogues.push_back("The tribes have stolen music away from the people. \nFind the casette pieces and bring it back to them! ");
		myDialogues.push_back("There are two tribes with one half each, they have hidden them \naway in their sanctuaries, you need to find them. ");
		myDialogues.push_back("...");
		myDialogues.push_back("GO!");
	}
	else if (currentData.myKeys[0] && currentData.myKeys[1])
	{
		myDialogues.push_back("Congratulations!");
		myDialogues.push_back("You managed to beat the whole damn game.");
		myDialogues.push_back("We're happy you made it...");
		myDialogues.push_back("But how are you gonna spend the rest of the day?");
		myDialogues.push_back("Maybe watch a video?");
		myDialogues.push_back("Maybe restart and try again?");
	}
	else if (currentData.myKeys[0] || currentData.myKeys[1])
	{
		myDialogues.push_back("First key is got!");
		myDialogues.push_back("Now find the last one pls.");
	}
	if (myDialogues.size() > 0)
	{
		myText.SetText(myDialogues[myIndex]);
	}
	else
	{
		myNumberOfPops = 1;
	}

	myText.SetColor({ 1,1,1,1 });
	myText.SetPosition({ 0.13f, 0.7f });
}

DialogueState::~DialogueState()
{
	SetPollingStation(myPollingStation);
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eSelect, this);
	myPollingStation->myInputMapper.get()->RemoveObserver(Input::eInputEvent::eDash, this);
}

int DialogueState::Update(const float /*aDeltaTime*/)
{
	myIsActive = true;
	return myNumberOfPops;
}

void DialogueState::Init()
{}

void DialogueState::Render()
{
	myStateStack.RenderUnderlyingState();
	Tga2D::Engine::GetInstance()->GetGraphicsEngine().SetCamera(Tga2D::Camera());
	myAstralProjection.Render();
	myDialogueBox.Render();
	myText.Render();
}

void DialogueState::RecieveEvent(const Input::eInputEvent aEvent, const float /*aValue*/)
{
	if (!myIsActive) return;

	if (aEvent == Input::eInputEvent::eDash)
	{
		myIndex++;
		if (myIndex < myDialogues.size())
			myText.SetText(myDialogues[myIndex]);
		else
			myNumberOfPops = 1;
	}
	else if (aEvent == Input::eInputEvent::eSelect)
	{
		myNumberOfPops = 1;
	}
}
