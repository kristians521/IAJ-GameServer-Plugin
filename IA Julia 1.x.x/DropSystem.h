// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#ifndef DROPSYSTEM
#define DROPSYSTEM

#define MAX_MONSTER_ID			600
#define MAX_ITEM_FOR_MONSTER	40

#include "User.h"

class cDropSystem
{
public:
	cDropSystem();
	~cDropSystem();
	void LoadDropItems();
	bool DropItem(LPOBJ mObj,LPOBJ pObj);

private:
	struct sItemsDrop
	{
		short Map;
		short MinLvl;
		short MaxLvl;
		short Group;
		short Index;
		short RateItem;
		short Level;
		short Option;
		short Skill;
		short Luck;
		short Exc;
	};
	sItemsDrop ItemsDrop[MAX_MONSTER_ID][MAX_ITEM_FOR_MONSTER];
	sItemsDrop AllMobItemsDrop[1000];

	int LastRandomValue;
	int LastRandomItem;

	unsigned char ArrayMaxItem[MAX_MONSTER_ID];
	unsigned int AllMobArrayMaxItem;

	bool DropItem2(LPOBJ mObj,LPOBJ pObj, sItemsDrop ItemDrop[], unsigned int MaxItem);
};

extern cDropSystem DropSystem;

#endif