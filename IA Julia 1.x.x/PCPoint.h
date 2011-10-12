// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#ifndef PCPoint_H
#define PCPoint_H
#include "StdAfx.h"
#include "User.h"

#define MAX_PCSHOPITEM	120
#define MAX_PCMOB		100

enum eModeUpdate {PLUS,MINUS};
enum eTypePoint  {PCPOINT,WCOIN};

class cPCPoint
{
public:
	cPCPoint();
	~cPCPoint();
	/*For PCShop*/
	void LoadIniConfigs();
	void LoadConfigs();
	void InitItemShop();
	void CreatePacketShop();
	void OpenShop(int Index);
	void BuyItem(int Index,int Position);
	int gObjCalcItems(int X, int Y);
	/*For PCPoint*/
	void Tick(LPOBJ gObj);
	void InitPCPointForPlayer(LPOBJ gObj);
	void UpdatePoints(LPOBJ gObj,int CountPoints,eModeUpdate Mode,eTypePoint Type);
	void RewardsPointsKillMob(LPOBJ gObj,LPOBJ mObj,eTypePoint Type);

	struct  
	{
		int Enabled;
		int MaximumPCPoints;
		int AddPCPointsSec;
		int AddPCPointsCount;
		int MaximumWCPoints;
		//short AddPCPointsMinLvl;
		//short AddPCPointsLevelUP;   
	}sPoints;
private:
	struct
	{
		short Index;
		short ID;
		short Reward;
		int X;
		int Y;
		short Level;
		short Opt;
		short Luck;
		short Skill;
		short Dur;
		short Exc;
		short Ancient;
		short NewOpt1;
		short NewOpt2;
		short NewOpt3;
		short NewOpt4;
		short NewOpt5;
		short Cost;
		short SlotX;
	}PCShop[MAX_PCSHOPITEM];
	struct
	{
		short MobID;
		short PCPoints;
	}PCMonsters[MAX_PCMOB];
	struct
	{
		short MobID;
		short WCoins;
	}WCoinMonsters[MAX_PCMOB];

	short AmountRecords[3];
	BYTE PCPointPacket[4680];
	int PacketSizes;
	bool ArrayFiledItems[15][8];
	bool ChekingCallInitItemsInShop;
};

extern cPCPoint PCPoint;

#endif