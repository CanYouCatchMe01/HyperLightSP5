#include "stdafx.h"
#include "CameraComponent.h"
#include <tga2d/graphics/Camera.h>
#include "imgui/imgui.h"
#include "GameObject.h"
#include "AudioManager.h"

CameraComponent::CameraComponent(Tga2D::Camera* aCamera, const float aFieldOfView) : myCamera(aCamera)
{


	aFieldOfView;
	const auto windowSize = Tga2D::Engine::GetInstance()->GetWindowSize();
	const float vFov = aFieldOfView * 0.0174532925f;
	const float hFov = 2.0f * atan(tan(vFov / 2) * ((float)windowSize.x / (float)windowSize.y)) * (180.0f / 3.1415f);

	
	myCamera->SetPerspectiveProjection(hFov, { (float)windowSize.x, (float)windowSize.y }, 0.1f, 10000.0f);
}

void CameraComponent::OnAwake()
{
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eMoveLeft, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eMoveUp, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eMoveRight, this);
	myPollingStation->myInputMapper.get()->AddObserver(Input::eInputEvent::eMoveDown, this);

	myPollingStation->myAudioManager->SetListenerTransform(myTransform);
}

void CameraComponent::OnStart()
{
	myCameraOffset = myTransform->GetPosition();
	if (myPollingStation->myPlayer != nullptr)
	{
		myCameraOffset -= myPollingStation->myPlayer->GetTransform().GetPosition();
	}
}

void CameraComponent::RecieveEvent(Input::eInputEvent aEvent, const float aValue)
{
	aValue;
	if (myCameraMove)
	{
		Tga2D::Vector3f startPosition = myTransform->GetPosition();
		switch (aEvent)
		{
		case Input::eInputEvent::eMoveLeft:
			{
			myTransform->SetPosition({ startPosition.x - myCameraSpeed,startPosition.y,startPosition.z });
			break;
			}
		case Input::eInputEvent::eMoveUp:
		{
			myTransform->SetPosition({ startPosition.x,startPosition.y,startPosition.z + myCameraSpeed });
			break;
		}
		case Input::eInputEvent::eMoveRight:
		{
			myTransform->SetPosition({ startPosition.x + myCameraSpeed,startPosition.y,startPosition.z });
			break;
		}
		case Input::eInputEvent::eMoveDown:
		{
			myTransform->SetPosition({ startPosition.x,startPosition.y,startPosition.z - myCameraSpeed });
			break;
		}
		default:
			break;
		}
		myCamera->SetTransform(*myTransform);
	}
}

void CameraComponent::OnUpdate(const float /*aDeltaTime*/)
{
	//Follow Player
	if (myPollingStation->myPlayer != nullptr)
	{
		Tga2D::Vector3f targetPosition = myPollingStation->myPlayer->GetTransform().GetPosition();
		myTransform->SetPosition(targetPosition + myCameraOffset);
	}
	
	//Set rendering camera
	myCamera->SetTransform(*myTransform);
}

void CameraComponent::SetTargetToFollow(Tga2D::Transform* aTargetToFollow)
{
	aTargetToFollow;
}

void CameraComponent::FreeCamera()
{
}
