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
		eHoldW,
		eHoldS,
		eHoldD,
		eHoldA,
		eReleaseArrowUp,
		eReleaseArrowDown,
		eReleaseArrowRight,
		eReleaseArrowLeft,
		ePressM,
		eReleaseW,
		eReleaseS,
		eReleaseD,
		eReleaseA,
	//------For Menus------
		eKey_Enter,
		ePressArrowKeyUp,
		ePressArrowKeyDown,
		ePressArrowKeyRight,
		ePressArrowKeyLeft,
		ePressW,
		ePressS,
		ePressD,
		ePressA

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
		eEscape,
		eUnpause,
		eMap,
		eReleaseArrowUp,
		eReleaseArrowDown,
		eReleaseArrowRight,
		eReleaseArrowLeft,
	//------For Menus------
		eSelect,
		eMenuUp,
		eMenuDown,
		eMenuLeft,
		eMenuRight
	//------For Menus------
	};
}