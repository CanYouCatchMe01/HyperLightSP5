#include "stdafx.h"
#include "BaseDebugger.h"
#include "DebugObserver.h"

void BaseDebugger::AddObserver(DebugObserver* anObserver)
{
	myObservers.push_back(anObserver);
}

void BaseDebugger::RemoveObserver(DebugObserver* anObserver)
{
	auto it = std::find(myObservers.begin(), myObservers.end(), anObserver);

	if (it != myObservers.end())
		return;

	myObservers.erase(it);
}

void BaseDebugger::DebugUpdate()
{
	if (!ImGui::Begin("Menu"))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Scenes"))
		{
			ImGui::End();
		}
		if (ImGui::BeginMenu("Objects"))
		{
			for (DebugObserver* i : myObservers)
			{
				i->DebugUpdate();
			}
			ImGui::End();
		}
	}
	ImGui::End();
}
