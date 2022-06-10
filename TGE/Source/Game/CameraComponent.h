#pragma once
#include "Component.h"
#include "InputObserver.h"
#include "Scene.h"

namespace Tga2D
{
	class Camera;
}

class CameraComponent : public Component, public Input::InputObserver
{
public:
	CameraComponent() = default;
	CameraComponent(CameraContainer* aCamera, const float aFieldOfView);
	~CameraComponent();

	// Inherited via Component
	virtual void OnAwake() override;

	virtual void OnStart() override;

	void RecieveEvent(Input::eInputEvent aEvent, const float aValue = 0) override;

	virtual void OnUpdate(const float aDeltaTime) override;
	
	void SetTargetToFollow(Tga2D::Transform* aTargetToFollow);
	void FreeCamera();

	void SetCameraOffset(Tga2D::Vector3f aCameraOffset) { myCameraOffset = aCameraOffset; }
private:
	float myCameraSpeed = 0.05f;
	bool myCameraMove = false;
	CameraContainer* const myCameraContainer;
	Tga2D::Vector3f myCameraOffset = {0.f,1.f,-10.f};
};