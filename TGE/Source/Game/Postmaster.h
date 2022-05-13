#pragma once
#include <map>
#include <vector>

class Observer;

enum class eMessageType
{
	ePlayerTookDMG,
	ePlayerHealed,
	ePlayerPickedUpHealth
};


struct Message
{
	void* myVoidPointer;
	eMessageType myMsg;
	float aFloatValue;
	int anIntValue;
	//float or int? for player health
};

class Postmaster
{
public:
	void AddObserver(Observer* anObserver, const eMessageType aMessageType);
	void RemoveObserver(Observer* anObserver, const eMessageType aMessageType);
	void SendMsg(const Message& aMessage);
private:
	std::map<eMessageType, std::vector<Observer*>> myObservers;
};