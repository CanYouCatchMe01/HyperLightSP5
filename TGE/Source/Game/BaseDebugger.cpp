#include "stdafx.h"
#ifdef _DEBUG


#include "BaseDebugger.h"
#include "DebugObserver.h"
#include "SceneManager.h"
#include "Scene.h"
#include "UnityLoader.h"

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
					myPollingStation->mySceneManager->LoadScene(scene);
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
#endif // _DEBUG