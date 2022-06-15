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
		myDialogues.push_back("The tribes have stolen music away from the people \nand the once peaceful land had lost its harmony.");
		myDialogues.push_back("Find the casette pieces and bring it back to them!");
		myDialogues.push_back("There are two tribes with one half each, they have hidden them \naway in their sanctuaries, you need to find them. ");
		myDialogues.push_back("Before you go into their world, you have to make it through \nthis trial to get used to their world.");
		myDialogues.push_back("...");
		myDialogues.push_back("GO!");
	}
	else if (currentData.myKeys[0] && currentData.myKeys[1])
	{
		myDialogues.push_back("The artifacts are assembled and the land is filled with \nsweet music once again!");
		myDialogues.push_back("The tribes backed down after having been so ruthelessly \nbeaten down by Elise. They would not want to taste her wrath \nonce more.");
		myDialogues.push_back("The people could feel safe that the music would be protected \nby Elise if it were ever to be threatened again.");
		myDialogues.push_back("Harmony had returned to the lands, thanks to Elise...");
		myDialogues.push_back("... and thanks to you!");
		myDialogues.push_back("Thank you for playing!");

		myEndGame = true;

		currentData.myUpgrades[0] = false;
		currentData.myUpgrades[1] = false;
		currentData.myKeys[0] = false;
		currentData.myKeys[1] = false;
	}
	else if (currentData.myKeys[0] || currentData.myKeys[1])
	{
		myDialogues.push_back("You did it!");
		myDialogues.push_back("The essence has returned, but your quest is not done yet.");
		myDialogues.push_back("There is still one more to find to restore music to the world. \nI believe in you.");
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
	if(!myEndGame)
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
		{
			myNumberOfPops = 1;
			if (myEndGame)
				myNumberOfPops = 2;
		}
	}
	else if (aEvent == Input::eInputEvent::eSelect)
	{
		myNumberOfPops = 1;
		if (myEndGame)
			myNumberOfPops = 2;
	}
}
