// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#ifndef MONSTER_H
#define MONSTER_H

#include "StdAfx.h"
#include "User.h"
#include "Logger.h"

#ifdef _GS
DWORD MonsterAddTick();
#endif

// # Hook in GS # //
int MygEventMonsterItemDrop(BYTE *b_MonsterDataAddr,BYTE *a_gObjAddr);		   
void __cdecl MonsterDie(LPOBJ lpObjOfMonster, LPOBJ lpObjOfPlayer);	 
// # ---------- # //

class cMonster
{
public:
#ifdef _GS
	void ReadMonsterAdd();
	int MonsterAddAndSpawn(int Mob, int Map, int Speed, int X1, int Y1, int X2, int Y2, int Dir);
	int SetBoxPosition(int TableNum, int mapnumber, int ax, int ay, int aw, int ah);
#endif
	void LoadPkClear();
	bool NPCTalkEx(LPOBJ gObj, int NpcId);
	void LahapDupeBug(LPOBJ gObj);																			  
	void NPCMessage(int IndexPlayer, LPOBJ mObj, char* Msg,...);	
	void NPCMessageLog(sColor LogColor, sLogType LogType, LPOBJ gObj, LPOBJ mObj, char* Msg,...);
	void NPCMessageNear(LPOBJ mObj, char* Msg,...);	
	int GetPartyMemberCount(LPOBJ lpObj);

	struct sPkClear
	{
		int Enabled;
		int NpcId;	  
		int OnlyForGm;
		int Type;	
		DWORD PriceZen;
		DWORD PriceZenForAll;	  
		int PricePcPoints;
		int PricePcPointsForAll;	   
		int PriceWCoins;
		int PriceWCoinsForAll;
		int LevelReq;
	};	

	sPkClear ClearNpc;			 
	sPkClear ClearCommand;
private:			
	void PkClear(LPOBJ gObj, LPOBJ NpcObj); 
};
extern cMonster Monster;

#define OBJ_MAXMONSTER 7400
#endif