#include "stdafx.h"
#include "Postmaster.h"
#include "Observer.h"
#include <assert.h>

void Postmaster::AddObserver(Observer* anObserver, const eMessageType aMessageType)
{
	auto it = std::find(myObservers[aMessageType].begin(), myObservers[aMessageType].end(), anObserver);

	if (it != myObservers[aMessageType].end()) //if observer is already in here return
		return;

	myObservers[aMessageType].push_back(anObserver);
}

void Postmaster::RemoveObserver(Observer* anObserver, const eMessageType aMessageType)
{
	auto it = std::find(myObservers[aMessageType].begin(), myObservers[aMessageType].end(), anObserver);

	if (it == myObservers[aMessageType].end()) //if observer has already been removed
		return;

	myObservers[aMessageType].erase(it); //(anObserver);
}

void Postmaster::SendMsg(const Message& aMessage)
{
	for (auto observer : myObservers[aMessage.myMsg])
	{
		observer->RecieveMsg(aMessage);
	}
}
