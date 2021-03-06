#include "stdafx.h"
#ifndef _RETAIL


#include "BaseDebugger.h"
#include "DebugObserver.h"
#include "SceneManager.h"
#include "Scene.h"
#include "UnityLoader.h"
#include "StateStack.h"
#include "GameState.h"

BaseDebugger::BaseDebugger(PollingStation* aPollingStation)
{
	myPollingStation = aPollingStation;
}

void BaseDebugger::AddObserver(DebugObserver* anObserver)
{
	myObservers.push_back(anObserver);
}

void BaseDebugger::RemoveObserver(DebugObserver* anObserver)
{
	auto it = std::find(myObservers.begin(), myObservers.end(), anObserver);

	if (it == myObservers.end())
		return;

	myObservers.erase(it);
}

void BaseDebugger::DebugUpdate()
{


	//if (!ImGui::Begin("Debugger Menu"))
	//{
	//	// Early out if the window is collapsed, as an optimization.
	//	ImGui::End();
	//	return;
	//}
	if (ImGui::BeginTabBar("Boop"))
	{
		if (ImGui::BeginTabItem("Scenes"))
		{
			for (auto& scene : myPollingStation->mySceneManager.get()->myScenePaths)
			{
				if (ImGui::Button(scene.c_str()))
				{
					//Checks if the current state is gamestate, then don't add a more gamestate
					if (dynamic_cast<GameState*>(myStateStack->GetCurrentState()))
					{
						myPollingStation->mySceneManager.get()->LoadScene(scene);
					}
					else
					{
						myStateStack->PushState(new GameState(*myStateStack, myPollingStation, scene));
					}
				}
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Objects"))
		{
			for (DebugObserver* i : myObservers)
			{
				i->DebugUpdate();
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	//ImGui::End();


}
void BaseDebugger::SetStateStack(StateStack* aStateStack)
{
	myStateStack = aStateStack;
}
#endif // _RETAIL