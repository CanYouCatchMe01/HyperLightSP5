#pragma once
#include <bitset>

class CheckPointComponent;

struct GameData
{
	//If an upgrade is taken set it to true
	std::bitset<2> myUpgrades = { false };

	std::bitset<5> myTeleporterStatus = { false }; 

/*	If the teleporter is active set it to true
	------Teleporter Order------
		  0 = badlands 2
		  1 = badlands 3
		  2 = jungle 2
		  3 = jungle 3
		  4 = hub
	----------------------------	*/

	//If a keys is taken set it to true
	std::bitset<2> myKeys = { false };
	int myCurrentKeysAmount = 0;
	int myResolutionIndex = 1;
	CheckPointComponent* myCheckpoint = nullptr;
};

struct PlayerData
{
	Tga2D::Vector3f mySavePosition;
	int myMaxHP;
	int myCurrentHP;
	int myHealKitAmmnt = 0;
};

class GameDataManager
{
public:
	GameDataManager();
	~GameDataManager();

	inline GameData& GetGameData()							{ return myGameData; }
	inline PlayerData& GetPlayerData()						{ return myPlayerData; }

private:
	GameData myGameData;
	PlayerData myPlayerData;
};