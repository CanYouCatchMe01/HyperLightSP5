#include "stdafx.h"
#include "AnimationController.h"
#include "AnimatedMeshComponent.h"

void AnimationController::Update()
{
	if (myCurrent)
		myCurrent->Update();
	else
		myRootTransition.Update();
}

AnimationController::Transition* AnimationController::AddTransition(std::string aName, std::function<bool()> aFunction, float aSpeed)
{
	Transition* transition = new Transition();
	transition->mySpeed = aSpeed;
	transition->myName = std::move(aName);
	transition->myAnimationController = this;
	myRootTransition.myNextStates.push_back({ aFunction, transition });
	return myRootTransition.myNextStates.back().second;
}

AnimationController::Transition* AnimationController::AddTransition(Transition* aTransition, std::function<bool()> aFunction)
{
	aTransition->myHasCircleReferences = true;
	myRootTransition.myNextStates.push_back({ aFunction, aTransition });
	return myRootTransition.myNextStates.back().second;
}

void AnimationController::Init(AnimatedMeshComponent* aAnimatedMesh)
{
	myMesh = aAnimatedMesh;
	myRootTransition.myAnimationController = this;
}

AnimationController::Transition::~Transition()
{
	for (auto& s : myNextStates)
	{
		if (s.second == nullptr)
			continue;

		if (s.second == &myAnimationController->myRootTransition)
			continue;

		if (s.second->myHasCircleReferences)
			continue;

		delete s.second;
		s.second = nullptr;
	}
}

void AnimationController::Transition::Update()
{
	for (auto& a : myNextStates)
	{
		if (a.first())
		{
			myAnimationController->myCurrent = a.second;
			myAnimationController->myMesh->PlayAnimation(a.second->myName);
		}
	}
}

AnimationController::Transition* AnimationController::Transition::AddTransition(std::string aName, std::function<bool()> aFunction, float aSpeed)
{
	Transition* transition = new Transition();
	transition->mySpeed = aSpeed;
	transition->myAnimationController = myAnimationController;
	transition->myName = std::move(aName);
	myNextStates.push_back({ aFunction, transition });
	return myNextStates.back().second;
}

AnimationController::Transition* AnimationController::Transition::AddTransition(Transition* aTransition, std::function<bool()> aFunction)
{
	aTransition->myHasCircleReferences = true;
	myNextStates.push_back({ aFunction, aTransition });
	return myNextStates.back().second;
}