// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # GreatDevelop 1.0.x								# //
// ================================================== //

#include "StdAfx.h"
#ifndef MONSTER_H
#define MONSTER_H
#ifdef _GS
int MonsterAddAndSpawn(WORD Monster,BYTE Speed,BYTE Map,BYTE X, BYTE Y);
#endif
void ReadMonsterAdd();
int MygEventMonsterItemDrop(BYTE *b_MonsterDataAddr,BYTE *a_gObjAddr);		   
void __cdecl MonsterDie(LPOBJ lpObjOfMonster, LPOBJ lpObjOfPlayer);	 
//Monster Speak																												  
void NPCMessage(int IndexPlayer, LPOBJ mObj, char* Msg,...);	
void NPCMessageLog(sColor LogColor, sLogType LogType, LPOBJ gObj, LPOBJ mObj, char* Msg,...);
void NPCMessageNear(LPOBJ mObj, char* Msg,...);	
//						 	
#define OBJ_MAXMONSTER 7400
#endif