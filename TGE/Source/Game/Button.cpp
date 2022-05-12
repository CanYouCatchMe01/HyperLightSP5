#include "stdafx.h"
#include "Button.h"
#include <tga2d/engine.h>
#include <tga2d/texture/TextureManager.h>
#include <tga2d/graphics/GraphicsEngine.h>
#include <tga2d/drawers/SpriteDrawer.h>


Button::Button(eButtonType aButtonType, Tga2D::Vector2f aPosition) 
	: 
	myButtonType(aButtonType)
{
	mySpriteInstance.myPosition = aPosition;
	const wchar_t* texturePathSelected = nullptr;
	const wchar_t* texturePathDeselected = nullptr;
//	const wchar_t* texturePathPressed = nullptr;

	myText.SetText("");
	myText = Tga2D::Text(L"Text/Nectar.ttf", Tga2D::FontSize_18);
	myText.SetText("");
	myText.SetColor({ 1,1,1,1 });

	mySpriteInstance.mySize = { 0.2f,0.2f };
	mySpriteInstance.mySizeMultiplier = { 1.5f, 0.75f };
	mySpriteInstance.myColor = { 1,1,1,1 };
	mySpriteInstance.myPivot = { 0.5f, 0.5f };

	mySpriteInstance.myPosition = aPosition;

	bool fullscreenStat = Tga2D::Engine::GetInstance()->GetGraphicsEngine().GetFullsScreenStatus();

	//set textures based on button type
	switch (aButtonType)
	{
	case eButtonType::Start:
		texturePathSelected = L"Sprites/UI/Menus/MainMenu/ui_mainMenu_start_selected.dds";
		texturePathDeselected = L"Sprites/UI/Menus/MainMenu/ui_mainMenu_start_unSelected.dds";
		break;
	case eButtonType::Options:
		texturePathSelected = L"Sprites/UI/Menus/MainMenu/ui_mainMenu_options_selected.dds";
		texturePathDeselected = L"Sprites/UI/Menus/MainMenu/ui_mainMenu_options_unSelected.dds";
		break;
	case eButtonType::ArrowRight:
		texturePathSelected = L"Sprites/UI/Menus/OptionsMenu/ui_optionsMenu_arrowRight_pressed.dds";
		texturePathDeselected = L"Sprites/UI/Menus/OptionsMenu/ui_optionsMenu_arrowRight_unSelected.dds";

		mySpriteInstance.mySizeMultiplier = { 0.75f };
		break;
	case eButtonType::ArrowLeft:
		texturePathSelected = L"Sprites/UI/Menus/OptionsMenu/ui_optionsMenu_arrowLeft_pressed.dds";
		texturePathDeselected = L"Sprites/UI/Menus/OptionsMenu/ui_optionsMenu_arrowLeft_unSelected.dds";

		mySpriteInstance.mySizeMultiplier = { 0.75f };
		break;
	case eButtonType::MasterVol:
		texturePathSelected = L"Sprites/UI/Menus/OptionsMenu/ui_optionsMenu_masterVolume_selected.dds";
		texturePathDeselected = L"Sprites/UI/Menus/OptionsMenu/ui_optionsMenu_masterVolume_unSelected.dds";
		myText.SetText("Master Test123");
		myHasText = true;
		break;
	case eButtonType::MusicVol:
		texturePathSelected = L"Sprites/UI/Menus/OptionsMenu/ui_optionsMenu_musicVolume_selected.dds";
		texturePathDeselected = L"Sprites/UI/Menus/OptionsMenu/ui_optionsMenu_musicVolume_unSelected.dds";
		myText.SetText("Music Test123");
		myHasText = true;
		break;
	case eButtonType::SFXVol:
		texturePathSelected = L"Sprites/UI/Menus/OptionsMenu/ui_optionsMenu_sfxVolume_selected.dds";
		texturePathDeselected = L"Sprites/UI/Menus/OptionsMenu/ui_optionsMenu_sfxVolume_unSelected.dds";
		myText.SetText("SFX Test123");
		myHasText = true;
		break;
	case eButtonType::Resolution:
		texturePathSelected = L"Sprites/UI/Menus/OptionsMenu/ui_optionsMenu_resolution_selected.dds";
		texturePathDeselected = L"Sprites/UI/Menus/OptionsMenu/ui_optionsMenu_resolution_unSelected.dds";
		myText.SetText("Res Test123");
		myHasText = true;
		break;
	case eButtonType::FullScreen:
		texturePathSelected = L"Sprites/UI/Menus/OptionsMenu/ui_optionsMenu_fullscreen_selected.dds";
		texturePathDeselected = L"Sprites/UI/Menus/OptionsMenu/ui_optionsMenu_fullscreen_unSelected.dds";
		myText.SetColor({ 1,0,0,1 });
		if (!fullscreenStat)
			myText.SetText("Windowed");
		else
			myText.SetText("FullScreen");
		myHasText = true;
		break;
	case eButtonType::Credits:
		texturePathSelected = L"Sprites/UI/Menus/MainMenu/ui_mainMenu_credits_selected.dds";
		texturePathDeselected = L"Sprites/UI/Menus/MainMenu/ui_mainMenu_credits_unSelected.dds";
		break;
	case eButtonType::Back:
		texturePathSelected = L"Sprites/UI/Menus/OptionsMenu/ui_optionsMenu_back_selected.dds";
		texturePathDeselected = L"Sprites/UI/Menus/OptionsMenu/ui_optionsMenu_back_unSelected.dds";
		break;
	case eButtonType::Resume:
		texturePathSelected = L"Sprites/UI/Menus/PauseMenu/ui_pauseMenu_resume_selected.dds";
		texturePathDeselected = L"Sprites/UI/Menus/PauseMenu/ui_pauseMenu_resume_unSelected.dds";

		break;
	case eButtonType::ExitToMain:
		texturePathSelected = L"Sprites/UI/Menus/PauseMenu/ui_pauseMenu_quit_selected.dds";
		texturePathDeselected = L"Sprites/UI/Menus/PauseMenu/ui_pauseMenu_quit_unSelected.dds";
		break;
	case eButtonType::Exit:
		texturePathSelected = L"Sprites/UI/Menus/MainMenu/ui_mainMenu_quit_selected.dds";
		texturePathDeselected = L"Sprites/UI/Menus/MainMenu/ui_mainMenu_quit_unSelected.dds";
		break;
	default:
		break;
	}

	mySelectedTexture = Tga2D::Engine::GetInstance()->GetTextureManager().GetTexture(texturePathSelected);
	myDeselectedTexture = Tga2D::Engine::GetInstance()->GetTextureManager().GetTexture(texturePathDeselected);


	mySharedData.myTexture = myDeselectedTexture;
}

void Button::SetState(eState aState)
{
	myState = aState;
	switch (myState)
	{
	case eState::None:
		mySharedData.myTexture = myDeselectedTexture;
		break;
	case eState::Selected:
		mySharedData.myTexture = mySelectedTexture;
		break;
	default:
		break;
	}
}

void Button::Render()
{
	Tga2D::Engine::GetInstance()->GetGraphicsEngine().GetSpriteDrawer().Draw(mySharedData, mySpriteInstance);
	if (myHasText)
	{
		myText.SetPosition({ mySpriteInstance.myPosition.x - (myText.GetWidth() / 2), mySpriteInstance.myPosition.y + 0.015f});
		myText.Render();
	}
}

void Button::SetActiveColour()
{
	myText.SetColor({ 1,0,0,1 });
}

void Button::ResetColour()
{
	myText.SetColor({ 1,1,1,1 });
}

void Button::ToggleFullScreen()
{
	if (myButtonType == eButtonType::FullScreen)
	{
		bool fullscreenStat = Tga2D::Engine::GetInstance()->GetGraphicsEngine().GetFullsScreenStatus();

		if (!fullscreenStat)
			myText.SetText("Windowed");
		else
			myText.SetText("FullScreen");
	}
}

void Button::SetText(std::string aText)
{
	myText.SetText(aText);
}
