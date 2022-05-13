#include "stdafx.h"
#include "MeleeComponent.h"

MeleeComponent::MeleeComponent()
{
}

MeleeComponent::~MeleeComponent()
{
}

void MeleeComponent::OnUpdate(float aDeltaTime)
{
	aDeltaTime;
}
#ifdef _DEBUG
void MeleeComponent::DebugUpdate()
{
	if (ImGui::CollapsingHeader("MeleeComponent"))
	{
		ImGui::InputInt("Damage", &myDamage);
		ImGui::InputInt("AttackSpeed", &myAttackSpeed);
	}
}
#endif // _DEBUG
void MeleeComponent::OnAwake()
{
}
void MeleeComponent::OnStart()
{
}



