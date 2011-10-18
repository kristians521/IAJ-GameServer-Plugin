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
	void Load();
	void Connect(LPOBJ gObj);
	void Tick(LPOBJ gObj);

	struct cConfig
	{
		int NumStates;
		int Enabled;
		char Column[25];
		char ColumnDate[25];

		struct cVIPState
		{ 
			char VIPName[8];
			int EnabledCmd;
			DWORD CostZen;
			int CostPCPoints;

			int CostWCoins;
			int BonusExp;
			int BonusZen;
			int BonusDrop; 

			int MinHours;
			int MaxHours;
		};
		cVIPState VIPState[20]; 

	}Config;

private:
	bool CheckVipTime(int TimeInMin);
};
extern cVip Vip;

#endif