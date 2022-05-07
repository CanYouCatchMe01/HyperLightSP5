#pragma once

struct Message;

class Observer 
{
public:
	virtual void RecieveMsg(const Message& aMsg) = 0;
};