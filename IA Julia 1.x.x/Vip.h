
#ifndef VIP_H
#define VIP_H

#include "StdAfx.h"
#include "User.h"

class cVip
{
public:
	void Connect(LPOBJ gObj);
	void Tick(LPOBJ gObj);
private:
	bool CheckVipTime(int TimeInMin);
};
extern cVip Vip;

#endif