#pragma once
#include <map>
#include <vector>

class Observer;

enum class eMessageType
{

};


struct Message
{
	void* myVoidPointer;
	eMessageType myMsg;
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