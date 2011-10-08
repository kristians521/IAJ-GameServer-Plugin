// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#ifndef GOLDEN_H
#define GOLDEN_H

#include "User.h"

#define MAX_ITEM_PRIZE 100

class cGoldenArcher
{
public:
	cGoldenArcher();
	~cGoldenArcher();
	void LoadPrizeItems();
	void GoldenArcherClick(LPOBJ gObj);
	bool ChekingRena(LPOBJ gObj,int Mode);
private:
	struct
	{
		short Index;
		short ItemID;
		short RateItem;
		short MaxLvl;
		short RateSkill;
		short RateLuck;
		short MaxOpt;
		short RateExc;
		short MaxExcOpt;
	}ItemsPrize[MAX_ITEM_PRIZE];
	int MaxRateItem;
	int counter;
};

extern cGoldenArcher GoldenArcher;

#endif

