#pragma once
#include <tga2d/sprite/sprite.h>

class PollingStation;

enum eElementType
{
	eHealthKit,
	eHealthBar,
	eMapTeleporter
};

class UIElement
{
public:
	UIElement(Tga2D::Vector2f aPosition, Tga2D::Vector2f aSizeMultiplier, std::wstring aTexturePath);
	~UIElement();
	void Render();
	void ChangeTexture(std::wstring aTexturePath);
	void ChangeSizeMultiplier(Tga2D::Vector2f aSizeMultiplier);
	void SetPivot(Tga2D::Vector2f aPosition);
	void SetPosition(Tga2D::Vector2f aPosition);
	inline Tga2D::Vector2f GetSizeMultiplier() { return mySpriteInstance.mySizeMultiplier; }
private:
	Tga2D::Vector2f myStartSizeMultiplier;
	Tga2D::Texture* myTexture;

	Tga2D::SpriteSharedData mySharedData;
	Tga2D::Sprite2DInstanceData mySpriteInstance = {};
};