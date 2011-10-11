// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

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