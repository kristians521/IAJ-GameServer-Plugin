// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#pragma once

#ifndef USER_H
#define USER_H

#include "StdAfx.h"
#include "Configs.h"
#include "Prodef.h"

typedef	struct sAddTab 
{			
	//Add Stats
	int		ADD_Type;
	DWORD	ADD_Value;

	//Post Delay
	int		POST_Delay; 

	//Anti AFK
	int		AFK_Timer;
	int		AFK_MapNumber;
	int		AFK_X;
	int		AFK_Y;
	int		AFK_Warnings;
	DWORD	AFK_Temp;	

	//VIP System
	int		VIP_Type;
	int		VIP_Min;
	int		VIP_Sec; 

	//Duel System
	BYTE	DUEL_Room;
	int		DUEL_Wins;
	int		DUEL_Lose;
	int		DUEL_AllDuels;
	int		DUEL_DuelLimit;
	int		DUEL_SpecVisible;    

	//PCPoints
	int		PC_InfoMod;
    int		PC_InfoReceived;
    int		PC_PlayerPoints;
	int		PC_OnlineTimer;
	int		PC_POINT_TEST;

	//Online
	int		ON_Min;   
	int		ON_Sek;
	int		ON_Hour;

	//Other
	int		Resets;
	int		UsedSlot;
}sAddTab;

typedef OBJECTSTRUCT * LPOBJ;
extern sAddTab AddTab[OBJECT_MAX];

// #	FIXES	# //
void __stdcall gObjViewportPatchExecute(OBJECTSTRUCT *gObj);
void __stdcall JGPGetCharInfoEx(int Loop, unsigned char *DSPacket, unsigned char *toClientPacket);	
// # ---------- # //

// # Hook in GS # //
void gObjLevelUpPointAddEx(BYTE type, OBJECTSTRUCT* lpObj);
BOOL gObjGameClose_Func(int aIndex);
void GCKillPlayerExpSendHook(int aIndex, int TargetIndex, int exp, int AttackDamage, BOOL MSBFlag);
void MyObjCalCharacter(int aIndex);			   	 
void GCEquipmentSendHook(int aIndex); 
// # ---------- # //

// #	TICK	# //
void TradeSystem__Cancel(void * lpParam);	
// # ---------- # //

class cUser
{
public:
	void OnlineTimeTick(LPOBJ gObj);
	void PlayerConnect(LPOBJ gObj); 
	void CheckRingSend(LPOBJ gObj, LPBYTE aRecv);	
	void CheckRing(LPOBJ gObj, LPBYTE aRecv);
	void RingSkin(LPOBJ gObj);
	bool CGPartyRequestRecv(PMSG_PARTYREQUEST * lpMsg, int aIndex);
	bool CharacterCreate(PMSG_CHARCREATE* lpMsg, int aIndex);
	bool GuildMasterInfoSave(int aIndex,PMSG_GUILDINFOSAVE* lpMsg);	
	void gObjCharacterWingsSetPreview(short ItemType, unsigned char *CharSet,int Type,OBJECTSTRUCT *lpObj);	
	bool CheckMaxPoints(BYTE type, OBJECTSTRUCT* lpObj);
private:			
	void LoginMsg(LPOBJ gObj);
};
extern cUser User;

#endif