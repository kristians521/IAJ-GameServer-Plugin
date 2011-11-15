// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#ifndef TradeSystem_h
#define TradeSystem_h

#include "User.h"

class cTradeSystem
{
public:
	void Load();
	bool CheckTradeItem(LPOBJ gObj);

	int Enabled;
	int TradeItemsCount;

	struct sTradeItems
	{
		int Group;
		int Index;
		int Level;
		int Skill;
		int Luck;
		int Opt;
		int Exc;
		int Anc;
	}TradeItems[256];
};

extern cTradeSystem TradeSystem;

#endif