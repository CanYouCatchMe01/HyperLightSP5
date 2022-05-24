#pragma once
#include <bitset>

class CheckPointComponent;

struct GameData
{
	//If an upgrade is taken set it to true
	std::bitset<2> Upgrades = { false };

	std::bitset<5> TeleporterStatus = { false }; 

/*	If the teleporter is active set it to true
	------Teleporter Order------
		  0 = badlands1
		  1 = badlands2
		  2 = jungle 1
		  3 = jungle 2
		  4 = hub
	----------------------------	*/

	//If a collectible is taken set it to true
	std::bitset<6> Collectibles = { false };
};

struct PlayerData
{
	CheckPointComponent* myCheckpoint = nullptr;
	Tga2D::Vector3f mySavePosition = 0;
	int MaxHP;
	int CurrentHP;
	int AttakDMG;
};

class GameDataManager
{
public:
	GameDataManager();
	~GameDataManager();
	inline void UpdateGameData(GameData someGameData) { myGameData = someGameData; }
	inline void UpdatePlayerData(PlayerData somePlayerData) { myPlayerData = somePlayerData; }
	inline GameData GetGameData() { return myGameData; }
	inline PlayerData GetPlayerData() { return myPlayerData; }
private:
	GameData myGameData;
	PlayerData myPlayerData;
};