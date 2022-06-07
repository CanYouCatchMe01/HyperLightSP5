#ifndef _RETAIL

#pragma once

struct Message;
class DebugObserver
{
public:
	virtual void DebugUpdate() = 0;
};
#endif // _RETAIL
