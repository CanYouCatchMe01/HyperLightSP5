#pragma once
#include "Observer.h"
#include <tga2d/sprite/sprite.h>

class PollingStation;

enum eElementType
{
	eHealthKit,
	eHealthBar
};

class UIElement : public Observer
{
public:
	UIElement(Tga2D::Vector2f aPosition, eElementType anElementType, PollingStation* aPollingStation);
	~UIElement();
	void Render();
	void RecieveMsg(const Message& aMsg);
private:
	PollingStation* myPollingStation;
	int myNumberOfHealthKits;
	std::vector<Tga2D::Texture*> myTextures;
	eElementType myElementType;
	Tga2D::SpriteSharedData mySharedData;
	Tga2D::SpriteSharedData mySharedDataOutline;
	Tga2D::Sprite2DInstanceData mySpriteInstanceHealth = {};
	Tga2D::Sprite2DInstanceData mySpriteInstance = {};
};