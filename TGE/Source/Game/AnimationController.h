#pragma once
#include <functional>

class AnimationController
{
private:
	struct Transition
	{
		~Transition();
		void Update();
		Transition* AddTransition(std::string aName, std::function<bool()> aFunction, float aSpeed = 1.0f);
		Transition* AddTransition(Transition* aTransition, std::function<bool()> aFunction);

		float mySpeed;
		std::string myName;
		bool myHasCircleReferences = false;
		std::vector<std::pair<std::function<bool()>, Transition*>> myNextStates;
		AnimationController* myAnimationController;
	};
public:
	friend class AnimatedMeshComponent;
	AnimationController() = default;
	void Update();
	Transition* AddTransition(std::string aName, std::function<bool()> aFunction, float aSpeed = 1.0f);
	Transition* AddTransition(Transition* aTransition, std::function<bool()> aFunction);
	//Transition* AddTransition(std::string aName);
private:
	void Init(class AnimatedMeshComponent* aAnimatedMesh);
	AnimatedMeshComponent* myMesh;
	Transition* myCurrent = nullptr;
	Transition myRootTransition;
};