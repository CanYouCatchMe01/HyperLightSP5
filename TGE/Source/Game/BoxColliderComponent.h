#pragma once
#include "Component.h"
#include "DebugObserver.h"

class BoxColliderComponent : public Component 
#ifdef _DEBUG 
	, public DebugObserver 
#endif
{
public:
	BoxColliderComponent(Tga2D::Vector3f aSize, Tga2D::Vector3f aCenter, bool aIsStatic, bool aIsTrigger);
	~BoxColliderComponent();
	// Inherited via Component
	virtual void OnAwake() override;

	virtual void OnStart() override;

	virtual void OnUpdate(const float aDeltaTime) override;

#ifdef _DEBUG
	void DebugUpdate() override;
#endif // _DEBUG


private:
	bool myDrawHitbox = false;
	bool myIsStatic;
	bool myIsTrigger;
	size_t myHandle;
	Tga2D::Vector3f myCenter;
	Tga2D::Vector3f mySize;
};