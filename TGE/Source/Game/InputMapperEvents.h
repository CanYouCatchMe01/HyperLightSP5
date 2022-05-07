#pragma once
namespace Input
{
	enum class eInputAction
	{
		eKey_Space,
		eKey_Escape,
		eKey_X,
		eHoldArrowKeyUp,
		eHoldArrowKeyDown,
		eHoldArrowKeyRight,
		eHoldArrowKeyLeft,
		eReleaseArrowUp,
		eReleaseArrowDown,
		eReleaseArrowRight,
		eReleaseArrowLeft,
	//------For Menus------
		eKey_Enter,
		ePressArrowKeyUp,
		ePressArrowKeyDown,
		ePressArrowKeyRight,
		ePressArrowKeyLeft
	//------For Menus------
	};

	enum class eInputEvent
	{
		eMoveUp,
		eMoveDown,
		eMoveRight,
		eMoveLeft,
		eAttack,
		eDash,
		eHeal,
		ePause,
		eUnpause,
		eReleaseUp,
		eReleaseDown,
		eReleaseRight,
		eReleaseLeft,
	//------For Menus------
		eSelect,
		eMenuUp,
		eMenuDown,
		eMenuLeft,
		eMenuRight
	//------For Menus------
	};
}