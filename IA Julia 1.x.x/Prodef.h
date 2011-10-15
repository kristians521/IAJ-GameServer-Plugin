// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#include "StdAfx.h"

#ifndef PRODEF_H
#define PRODEF_H
					 
#pragma once
#define MAKE_NUMBERW(x,y)  ( (WORD)(((BYTE)((y)&0xFF)) |   ((BYTE)((x)&0xFF)<<8 ))  ) 
#define SET_NUMBERH(x) ( (BYTE)((DWORD)(x)>>(DWORD)8) )
#define SET_NUMBERL(x) ( (BYTE)((DWORD)(x) & 0xFF) )

#ifndef MAX_ACCOUNT_LEN
	#define MAX_ACCOUNT_LEN	10
	#define MAX_GUILD_LEN	8
#endif

#define OBJECT_MAXRANGE(aIndex) ( ((aIndex) < 0 )?FALSE:( (aIndex) > OBJECT_MAX -1 )?FALSE:TRUE   )
			   		  
void BuxConvert(char* buf, int size);
			  
void PHeadSetB(LPBYTE lpBuf, BYTE head, int size);

void PHeadSubSetB(LPBYTE lpBuf, BYTE head, BYTE sub, int size);
// ADVANCE DEVIRVATED
struct PBMSG_HEAD	// Packet - Byte Type
{
public:
	void set ( LPBYTE lpBuf, BYTE head, BYTE size)	// line : 18
	{
		lpBuf[0] = 0xC1;
		lpBuf[1] = size;
		lpBuf[2] = head;
	};	// line : 22

	void setE ( LPBYTE lpBuf, BYTE head, BYTE size)	// line : 25
	{
		lpBuf[0] = 0xC3;
		lpBuf[1] = size;
		lpBuf[2] = head;
	};	// line : 29

	BYTE c;
	BYTE size;
	BYTE headcode;
}; 

struct PBMSG_HEAD2	// Packet - Byte Type
{

public:

	void set( LPBYTE lpBuf, BYTE head, BYTE sub, BYTE size)
	{
		lpBuf[0] = 0xC1;
		lpBuf[1] = size;
		lpBuf[2] = head;
		lpBuf[3] = sub;
	};

	BYTE c;
	BYTE size;
	BYTE headcode;
	BYTE subcode;
};

struct PMSG_DEFAULT2
{
	PBMSG_HEAD h;
	BYTE subcode;
};

struct PMSG_PARTYREQUEST
{
	PBMSG_HEAD h;
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
};			 

struct PWMSG_HEAD	// Packet - Word Type
{
public:

	void set( LPBYTE lpBuf, BYTE head, int size)
	{
		lpBuf[0] = 0xC2;
		lpBuf[1] = SET_NUMBERH(size);
		lpBuf[2] = SET_NUMBERL(size);
		lpBuf[3] = head;
	};

	void setE( LPBYTE lpBuf, BYTE head, int size)	// line : 49
	{
		lpBuf[0] = 0xC4;
		lpBuf[1] = SET_NUMBERH(size);
		lpBuf[2] = SET_NUMBERL(size);
		lpBuf[3] = head;
	};

	BYTE c;
	BYTE sizeH;
	BYTE sizeL;
	BYTE headcode;
};

struct PMSG_SEND_WINDOW
{
	BYTE uHead;
	BYTE uSize;
	BYTE uPacketType;
	BYTE uNum;
}; 

struct PMSG_SERVERCMD
{
	PBMSG_HEAD h;
	BYTE subcode;	// 3
	BYTE CmdType;	// 4
	BYTE X;	// 5
	BYTE Y;	// 6
};	 
							   							 
struct PMSG_CLIENTCLOSE
{
	PBMSG_HEAD h;
	BYTE SubCode;	// 3
	BYTE Flag;	// 4
};			

struct PMSG_CHARCREATE
{
	PBMSG_HEAD h;
	BYTE subcode;	// 3
	char Name[10];	// 4
	BYTE ClassSkin;	// E
};

struct PMSG_GUILDINFOSAVE
{
	PBMSG_HEAD h;
	BYTE btGuildType;	// 3
	char GuildName[8];	// 4
	BYTE Mark[32];	// C
};

struct PMSG_GUILDCREATED_RESULT
{
	PBMSG_HEAD h;	// C1:56
	BYTE Result;	// 3
	BYTE btGuildType;	// 4
};


//Function Defines //
#ifdef _GS
#define IAJuliaGS										".\\GameServer.ini"
#define GSSIZE										4452946
#define GetToken									0x00402CA2
#define GetNumber									0x00407257
#define ShopExcellentMod_Hook1						0x0055C6D4
#define ShopExcellentMod_Exit1						0x0055C6DF
#define ShopExcellentMod_Hook2						0x0055BC05
#define ShopExcellentMod_Exit2						0x0055BC0A
#define MonsterCount 0x7D95B94
#define MonsterReads 0x7D800B6

#define ChatTargetSend ((void(*)(OBJECTSTRUCT *lpObj, char*, DWORD)) 0x00439260)
#define GSCheckInventoryEmptySpace ((int(*) (LPOBJ lpObj, int iItemHeight, int iItemWidth)) 0x0051C770 ) 
#define GSItemGetSize		((void(*) (int index, int & width, int & height)) 0x00558AE0 )
#define gObjDel				((void(*) (int)) 0x00403AE4)
#define gObjAddMonster		((short(*)(int iMapNumber)) 0x00508910)
#define gObjSetPosMonster	((int(*)(int aIndex, int PosTableNum)) 0x00506870)
#define gObjSetMonster		((int(*)(int aIndex, int MonsterClass)) 0x00506CA0)
#define goBjHoookLoad1 ((bool(*) (LPOBJ)) 0x0053E7E0)
#define goBjHoookLoad2 ((int(*) (LPOBJ, LPOBJ)) 0x0053E860)	   		
#define gObjInventoryDeleteItem ((void(*)(int,int)) 0x00402464)				   
#define gObjMonsterDieGiveItem ((int(*) ( LPOBJ , LPOBJ )) 0x004050DD)
#define GCInventoryItemDeleteSend ((void(*)(int,int,unsigned char)) 0x00403AE9)
#define JGCharacterCreateFailSend ((void(*)(int aIndex, char* id)) 0x0042D990)
#define GCServerMsgStringSend ((void(*) (char * Texto,DWORD PlayerID,int Type)) 0x004066B3)
#define GCStateInfoSendg ((void(*)(OBJECTSTRUCT *lpObj, BYTE State, BYTE ViewSkillState)) 0x004040A2) //duel
#define gObjViewportListProtocolDestroy ((void(*)(OBJECTSTRUCT* lpObj)) 0x00402F5E)	//duel
#define gObjMoveGate ((int(*)(int aIndex, int Gate)) 0x00405C9A) //duel
#define gObjUserDie ((void(*)(OBJECTSTRUCT *lpObj,OBJECTSTRUCT *lpTargetObj)) 0x0050F7F0) //duel
#define gGetItemNumber ((DWORD(*)()) 0x0051A900);
#define gObjShopBuyInventoryInsertItem ((int(*)(int, CItem)) 0x0051B520) //moss
#define ItemGetDurability ((int(*)(short index, int itemLevel, int ExellentItem, int SetItem)) 0x00558B80) //moss
#define gObjTeleport ((void(*) (int,unsigned char,unsigned char,unsigned char)) 0x00403382)
#define AddBuff ((void(*)(LPOBJ lpObj,BYTE state, BYTE ViewSkillState,int TargetIndex, int IsSkill, int a6, int Time  )) 0x0060A360)
#define CloseClient ((void(*)(DWORD))0x0040788D) 
#define gObjViewportListProtocolCreate ((void(*)(OBJECTSTRUCT* tObj))0x0052F870) // Skin
#define MsgSendV2 ((void(*)(LPOBJ lpObj, unsigned char* Msg, int size))0x00438540)	 
#define DataSend ((void(*) (DWORD PlayerID,PBYTE Packet,DWORD Size)) 0x004B3370)
#define DataRecv ((void(*) (BYTE,PBYTE,DWORD,DWORD,...)) 0x004368E0)
#define DataSendAll ((void(*)(PBYTE lpMsg, int iMsgSize)) 0x0040729D)
#define ItemSerialCreateSend ((int(*)(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level, BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption)) 0x00407004)
#define GCKillPlayerExpSend ((void(*)(int aIndex, int TargetIndex, int exp, int AttackDamage, BOOL MSBFlag)) 0x00455EC0)
#define gObjCalCharacter ((void(*)(int)) 0x004E8AC0)
#define CloseClient ((void(*)(DWORD))0x0040788D)   
#define GCEquipmentSend	((void(*)(int)) 0x0043C7B0)
#define GCItemListSend	((void(*)(int)) 0x0042E780)
#define gObjMakePreviewCharSet ((void(*)(int)) 0x005263D0)
#define gObjGameClose ((BOOL(*)(int)) 0x00509940)  	
#define GCMoneySend ((void(*) (DWORD,int)) 0x004075EA)
#define GSGOBJGetIndex ((short(*)(SOCKET, char* ip)) 0x00508460)
#define gEventMonsterItemDrop ((int(*) (BYTE*,BYTE*)) 0x00420330)
#define GCPkLevelSend ((void(*)(int,unsigned char)) 0x0043C2F0)
#define pShopDataLoad ((void(*)()) 0x0055C7F0)
#define GameMonsterAllCloseAndReload ((void(*)()) 0x0040759F)
#define CGLevelUpPointAdd ((int(*)(BYTE * Arg1,DWORD PlayerID)) 0x004073DD)
#define GCLevelUpMsgSend ((void(*)(int, unsigned char)) 0x0043BDC0)
#define gObjLevelUpPointAdd ((bool(*)(BYTE type, OBJECTSTRUCT* lpObj)) 0x0050D990) 	 
#define GCgObjAttack ((int(*)(LPOBJ lpObj, LPOBJ lpTargetObj, class CMagicInf* lpMagic, int magicsend, unsigned char MSBFlag, int AttackDamage, BOOL bCombo)) 0x005165A0)
#define GCgObjPlayerKiller ((void(*)(LPOBJ lpObj, LPOBJ lpTargetObj))	0x0050EF80)
#define GDGuildNoticeSave ((void(*)(char* Name, char* MSG)) 0x0046DBA0)
#endif

#ifdef _GS_CS
#define IAJuliaGS										".\\GameServerCS.ini"
#define GSSIZE										4689920
#define GetToken									0x00402C02
#define GetNumber									0x00407B2B
#define ShopExcellentMod_Hook1						0x0056F6F4
#define ShopExcellentMod_Exit1						0x0056F6FF
#define ShopExcellentMod_Hook2						0x0056EC25
#define ShopExcellentMod_Exit2						0x0056EC2A
#define gObjGameClose ((BOOL(*)(int)) 0x0051B3E0)  	
#define ChatTargetSend ((void(*)(OBJECTSTRUCT *lpObj, char*, DWORD))0x0043C2A0);
#define gObjMoveGate ((int(*)(int aIndex, int Gate)) 0x00545170) //duel
#define gObjTeleport ((void(*) (int,unsigned char,unsigned char,unsigned char)) 0x0040341D)
#define CloseClient ((void(*)(DWORD))0x0040822E)	
#define DataRecv ((void(*) (BYTE,PBYTE,DWORD,DWORD,...)) 0x00439900)
#define JGCharacterCreateFailSend ((void(*)(int aIndex, char* id)) 0x0042F2F0)
#define GCServerMsgStringSend ((void(*) (char * Texto,DWORD PlayerID,int Type)) 0x00406E0B)
#define ItemSerialCreateSend ((int(*)(DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,DWORD)) 0x00407897)
#define gObjViewportListProtocolCreate ((void(*)(OBJECTSTRUCT* tObj))0x00542310) // Skin
#define DataSendAll ((void(*)(PBYTE lpMsg, int iMsgSize)) 0x00407B71)				
#define gObjMonsterDieGiveItem ((int(*) ( LPOBJ , LPOBJ )) 0x0040556F)
#define GCKillPlayerExpSend ((void(*)(int aIndex, int TargetIndex, int exp, int AttackDamage, BOOL MSBFlag)) 0x004584A0)
#define DataSend ((void(*) (DWORD PlayerID,PBYTE Packet,DWORD Size)) 0x004C4390)
#define gObjMakePreviewCharSet ((void(*)(int)) 0x005389B0)
#define GCItemListSend	((void(*)(int)) 0x004300E0)
#define GCEquipmentSend	((void(*)(int)) 0x0043F830)
#define gObjCalCharacter ((void(*)(int)) 0x004FA660)
#define GCMoneySend ((void(*) (DWORD,int)) 0x00407F45)
#define GSGOBJGetIndex ((short(*)(SOCKET, char* ip)) 0x00519F00) 
#define gEventMonsterItemDrop ((int(*) (BYTE*,BYTE*)) 0x00421D50)
#define GCInventoryItemDeleteSend ((void(*)(int,int,unsigned char)) 0x00403C7E)
#define gObjInventoryDeleteItem ((void(*)(int,int)) 0x004022AC)
#define GCPkLevelSend ((void(*)(int,unsigned char)) 0x00405722) 
#define GameMonsterAllCloseAndReload ((void(*)()) 0x00407EEB)
#define pShopDataLoad ((void(*)()) 0x00407360)
#define GCLevelUpMsgSend ((void(*)(int, unsigned char)) 0x0043EE00)
#define CGLevelUpPointAdd ((int(*)(BYTE * Arg1,DWORD PlayerID)) 0x00407CED)
#define gObjLevelUpPointAdd ((bool(*)(BYTE type, OBJECTSTRUCT* lpObj)) 0x0051F9A0) 
#define ItemGetDurability ((int(*)(short index, int itemLevel, int ExellentItem, int SetItem)) 0x0056BBA0) //moss
#define GSItemGetSize		((void(*) (int index, int & width, int & height)) 0x0056BB00 )
#define GSCheckInventoryEmptySpace ((int(*) (LPOBJ lpObj, int iItemHeight, int iItemWidth)) 0x0052EE50 )
#define GCgObjAttack ((int(*)(LPOBJ lpObj, LPOBJ lpTargetObj, class CMagicInf* lpMagic, int magicsend, unsigned char MSBFlag, int AttackDamage, BOOL bCombo)) 0x00528790)
#define GCgObjPlayerKiller ((void(*)(LPOBJ lpObj, LPOBJ lpTargetObj))	0x00521040)
#define GDGuildNoticeSave ((void(*)(char* Name, char* MSG)) 0x00473860)		
#endif


//Configs Defines //
#define IAJuliaAntiAFK			"..\\IA Julia\\AntiAFK.txt"
#define IAJuliaCmd				"..\\IA Julia\\Commands.ini"
#define IAJuliaCommon			"..\\IA Julia\\Common.ini"
#define IAJuliaDropSystem		"..\\IA Julia\\DropSystem.ini"
#define IAJuliaEvents			"..\\IA Julia\\Events.ini"
#define IAJuliaEventTime		"..\\IA Julia\\EventTimer.dat"
#define IAJuliaArcher			"..\\IA Julia\\GoldenArcher.txt"
#define IAJuliaItems			"..\\IA Julia\\Items.ini"	
#define IAJuliaMossGambler		"..\\IA Julia\\MossGambler.ini"
#define IAJuliaPcPoints		"..\\IA Julia\\PCPoints_WCoins.dat"
#define IAJuliaPets			"..\\IA Julia\\Pets.ini"
#define IAJuliaPkClear			"..\\IA Julia\\PkClearGuard.ini"
#define IAJuliaSkills			"..\\IA Julia\\Skills.ini"  
#define IAJuliaMobAdd			"..\\IA Julia\\MonsterSpawn.ini"  
#define IAJuliaMapSystem		"..\\IA Julia\\MapSystem.ini"
#define IAJuliaEventDropBC		"..\\IA Julia\\WzEvents\\EventDropBC.txt"
#define IAJuliaEventDropIT		"..\\IA Julia\\WzEvents\\EventDropIT.txt"
#define IAJuliaEventDropWW		"..\\IA Julia\\WzEvents\\EventDropWW.txt"
#define IAJuliaEventDropCC		"..\\IA Julia\\WzEvents\\EventDropCC.txt"
#define	IAJuliaWzEvents		"..\\IA Julia\\WzEvents"
#define IAJuliaNews			".\\News.txt"
#define IAJuliaGM				".\\GMSystem.txt"
#define IAJuliaIPBlock			".\\IPBlock.txt" 
#define IAJuliaVIP				".\\VIPSystem.ini" 
#define IAJuliaChatLogs		"..\\IA Julia\\ChatLogs"
#define IAJuliaConsoleLogs		"..\\IA Julia\\ConsoleLogs" 
#define IAJuliaLog				".\\Log" 

#define GS_CONFIG_FILE				".\\Data\\ServerInfo.dat" 

#ifdef _GS
//================================================//
// Maximum Level							      //
//================================================//
#define GS_NOEXP_LEVEL				0x0056A352
#define GS_MAX_LEVEL1				0x004E8435
#define GS_MAX_LEVEL2				0x0050D1DA
#define GS_MAX_LEVEL3				0x00582C77
#define GS_MAX_LEVEL4				0x005938F7
#define GS_MAX_LEVEL5				0x005C27F9
#define GS_MAX_MASTERLEVEL          0x004F77BF
#define GS_ML_POINTS				0x004F7984

//================================================//
// Maximum Zen								      //
//================================================//
#define MaxZen						(0x0053F2F7+3)
#define MaxZen1						(0x00452F23+3)
#define MaxZen2						(0x00453019+3)
#define MaxZen3						(0x00452F4F+2)

//================================================//
// Ancient Item Drop Rate					      //
//================================================//
#define	GS_ANCREWARD_SKILL_RATE		0x0053F39F
#define GS_ANCREWARD_LUCK_RATE		0x0053F3A6
#define GS_ANCRANDOM_SKILL_RATE		0x0053F5DF
#define	GS_ANCRANDOM_LUCK_RATE		0x0053F5E6

//================================================//
// Devil Square PartyExp					      //
//================================================//
#define GS_DS_PARTYDIFFEXP_3		0x00478870
#define GS_DS_PARTYDIFFEXP_4		0x0047887F
#define GS_DS_PARTYDIFFEXP_5		0x0047888E
#define GS_DS_PARTYEXP_2			0x004788A6
#define GS_DS_PARTYEXP_3			0x004788B5
#define GS_DS_PARTYEXP_4			0x004788C4
#define GS_DS_PARTYEXP_5			0x004788D3

//================================================//
// Skills Div								      //
//================================================//
#define GS_BERSERKER_DIV1			0x004E5B07
#define GS_BERSERKER_DIV2			0x004E5B1F
#define GS_BERSERKER_DIV3			0x004E5B34
#define GS_DAMAGEREF_DIV1			0x004E4314
#define GS_DAMAGEREF_DIV2			0x004E432C
#define GS_CRITICALDMG_COMMAND		0x004DFA44
#define GS_CRITICALDMG_ENERGY		0x004DFA64
#define GS_CRITICALDMG_TIME			0x004DFA87

//================================================//
//Transformation Rings						      //
//================================================//
#define GS_TRANSFORMATIONRING1	(0x004DBCA4+3)
#define GS_TRANSFORMATIONRING2	(0x004DBCAD+3)
#define GS_TRANSFORMATIONRING3	(0x004DBCB6+3)
#define GS_TRANSFORMATIONRING4	(0x004DBCBF+3)
#define GS_TRANSFORMATIONRING5	(0x004DBCC8+3)
#define GS_TRANSFORMATIONRING6	(0x004DBCD1+3)

//================================================//
//Summon Orbs								      //
//================================================//
#define GS_SUMMONORB1			(0x004DA297+1)
#define GS_SUMMONORB2 			(0x004DA2C8+1)
#define GS_SUMMONORB3			(0x004DA2F9+1)
#define GS_SUMMONORB4			(0x004DA32A+1)
#define GS_SUMMONORB5			(0x004DA35B+1)
#define GS_SUMMONORB6			(0x004DA38C+1)
#define GS_SUMMONORB7			(0x004DA3C0+1)

//================================================//
//Jewel Prices								      //
//================================================//
#define GS_BLESS_PRICE			(0x0055219E+3)
#define GS_SOUL_PRICE			(0x005521C0+3)
#define GS_CHAOS_PRICE			(0x005521E1+3)
#define GS_LIFE_PRICE			(0x00552203+3)
#define GS_CREATION_PRICE		(0x00552225+3)
#define GS_GUARDIAN_PRICE		(0x005522FF+3)
#define GS_FRUITS_PRICE			(0x005522A0+3)
#define GS_MONARCH_PRICE		(0x005522CE+3)
#define GS_FEATHER_PRICE		(0x005522DE+3)
#define GS_BLESSPOT_PRICE		(0x00552337+2)
#define GS_SOULPOT_PRICE		(0x0055235D+2)

#define GS_KUNDUN_ITEM_NUMB		(0x0041DFFA+6)
#define GS_KUNDUN_ANC_PERCT		(0x0041E1C5+2)


//================================================//
//Chaos Castle									  //
//================================================//
#define GS_CCPLAYER				(0x00594890+3) 

#define GS_CCREWARD1			(0x00597D69+3) 
#define GS_CCREWARD2			(0x00597D23+3)
#define GS_CCREWARD3			(0x00597C9F+3)
#define GS_CCREWARD4			(0x00597C59+3)

//================================================//
//Illusion Temple								  //
//================================================//
#define GS_ITPLAYER			(0x005DE535+6)
/*
#define GS_IT_DROP_ID       (0x005E6713+1)
#define GS_IT_GROUP_ID      (0x005E6715+1)
#define GS_IT_ITEM_LVL      (0x005E6732+1)
#define GS_IT_ITEM_DUR      (0x005E6730+1)
#define GS_IT_ITEM_SKL      (0x005E672E+1)
#define GS_IT_ITEM_LCK      (0x005E672C+1)
#define GS_IT_ITEM_LIF      (0x005E672A+1)
#define GS_IT_ITEM_EXC      (0x005E6724+1)
#define GS_IT_ITEM_ANC      (0x005E6722+1)
*/
//================================================//
//Item Looting Time								  //
//================================================//
#define GS_ITEM_TIME1       (0x004BB23A+1)
#define GS_ITEM_TIME2       (0x004BB3EB+1)

//================================================//
//Guild Alliance Members						  //
//================================================//
#define GS_GUILDALLIANCE		(0x00467E0D+2)

//================================================//
//GS UDP Port									  //
//================================================//
#define GS_UDP_PORT				(0x0056A21F+1)

//================================================//
//Jewel Rates									  //
//================================================//
#define GS_SOUL_RATE_NORMAL		(0x005370C3+3)
#define GS_SOUL_RATE_SPECIAL	(0x00537082+3)
#define GS_SOUL_RATE_SOCKET		(0x005370B7+3)
#define GS_SOUL_LUCK_ADD		(0x0053701C+2)
#define GS_JOL_FAIL_RATE		(0x005378D7+3)

//================================================//
//Blood Castle Reward                             //
//================================================//
/*
#define GS_BC_DROP_ID			(0x0058810B+1)
#define GS_BC_DROP_GROUP		(0x0058810D+1)
#define GS_BC_ITEM_LVL			(0x005881B2+1)
#define GS_BC_ITEM_DUR			(0x005881B0+1)
#define GS_BC_ITEM_SKL			(0x005881AE+1)
#define GS_BC_ITEM_LCK			(0x005881AC+1)
#define GS_BC_ITEM_LIF			(0x005881AA+1)
#define GS_BC_ITEM_EXC			(0x005881A4+1)
#define GS_BC_ITEM_ANC			(0x005881A2+1)
*/

//================================================//
//Chaos Machine Rates						      //
//================================================//
#define GS_CM_MIX_10_NORMAL		(0x005EB63F+6)
#define GS_CM_MIX_10_SPECIAL	(0x005EB65B+6)
#define GS_CM_MIX_10_SOCKET		(0x005EB678+6)
#define GS_CM_MIX_11_12_NORMAL	(0x005EB6CF+6)
#define GS_CM_MIX_11_12_SPECIAL	(0x005EB6EC+6)
#define GS_CM_MIX_11_12_SOCKET	(0x005EB709+6)
#define GS_CM_MIX_13_NORMAL		(0x005EB761+6)
#define GS_CM_MIX_13_SPECIAL	(0x005EB77E+6)
#define GS_CM_MIX_13_SOCKET		(0x005EB79A+6)
#define GS_CM_MIX_10_LUCK		(0x005EB7E7+6)
#define GS_CM_MIX_11_13_LUCK	(0x005EB7F3+6)
#define GS_CM_MIX_LUCK_ADD		(0x005EB7D8+2)
#define DEFAULTMIXRATE			(0x005EB7A9+6)

//================================================//
//Dark Lord Pet Rates							  //
//================================================//
#define GS_DARK_HORSE			(0x005EDE9C+6)
#define GS_DARK_HORSE_PRICE		(0x005EDEBB+3)
#define GS_DARK_SPIRIT			(0x005EE59D+6)
#define GS_DARK_SPIRIT_PRICE	(0x005EE5BC+3)

//================================================//
//Item Drop Rates								  //
//================================================//
#define GS_EXC_DROP_RATE           (0x0041F0E1+1)
#define GS_EXC_SKILL_RATE           (0x0041F4EA+6)
#define GS_EXC_LUCK_RATE           (0x0041F4F4+6)
#define GS_SKILL_RATE                (0x0041F543+6)
#define GS_LUCK_RATE                (0x0041F54D+6)

//================================================//
//Level Up Points							      //
//================================================//
#define GS_LVL_UP_DL1			(0x0050D3BB+2)
#define GS_LVL_UP_DL2			(0x00582DF4+2)
#define GS_LVL_UP_DL3			(0x00593A74+2)
#define GS_LVL_UP_DL4			(0x005C2976+2)
#define GS_LVL_UP_DL5			(0x005E7102+2)
#define GS_LVL_UP_MG1			(0x0050D3E3+2)
#define GS_LVL_UP_MG2			(0x00582DF4+2) 
#define GS_LVL_UP_MG3			(0x00593A74+2) 
#define GS_LVL_UP_MG4			(0x005C2976+2)
#define GS_LVL_UP_MG5			(0x005E7102+2)
#define GS_LVL_UP_NORMAL1		(0x0050D3FA+2)
#define GS_LVL_UP_NORMAL2		(0x00582E25+2)
#define GS_LVL_UP_NORMAL3		(0x00593AA5+2) 
#define GS_LVL_UP_NORMAL4		(0x005C29A7+2) 
#define GS_LVL_UP_NORMAL5		(0x005E7132+2) 
#define GS_LVL_UP_QUEST1		(0x0050D41E+2) 
#define GS_LVL_UP_QUEST2		(0x00582E6E+2) 
#define GS_LVL_UP_QUEST3		(0x00593AEE+2) 
#define GS_LVL_UP_QUEST4		(0x005C29F0+2)
#define GS_LVL_UP_QUEST5		(0x005E717C+2)

//================================================//
//White Wizard Event Reward						  //
//================================================//
/*
#define GS_WW_DROP_ID       (0x004834D6+1)
#define GS_WW_GROUP_ID      (0x004834D8+1)
#define GS_WW_ITEM_LVL      (0x004834F5+1)
#define GS_WW_ITEM_DUR      (0x004834F3+1)
#define GS_WW_ITEM_SKL      (0x004834F1+1)
#define GS_WW_ITEM_LCK      (0x004834EF+1)
#define GS_WW_ITEM_LIF      (0x004834ED+1)
#define GS_WW_ITEM_EXC      (0x004834E7+1)
#define GS_WW_ITEM_ANC      (0x004834E5+1)
*/
//================================================//
//Chaos Machine Mixes							  //
//================================================//
#define GS_WINGLVL1MIX		(0x005ED2EA+6)
#define GS_WINGLVL2MIX		(0x005ED2F6+6)
#define GS_WINGLVL3MIX11	(0x005F27B0+6)
#define GS_WINGLVL3MIX12	(0x005F27BC+6)
#define GS_WINGLVL3MIX21	(0x005F2EB2+6)
#define GS_WINGLVL3MIX22	(0x005F2EBE+6)
#define GS_SETITEMMIX1 (0x005F3C7B+6)
#define GS_SETITEMMIX2 (0x005F3C87+6)
#define GS_DINO_MIX (0x005EC455+6)

//================================================//
//Party Exp										  //
//================================================//
#define GS_PARTYEXP_2_NORMAL	(0x0051701E+3)
#define GS_PARTYEXP_3_NORMAL	(0x0051702D+3)
#define GS_PARTYEXP_4_NORMAL	(0x0051703C+3)
#define GS_PARTYEXP_5_NORMAL	(0x0051704B+3)
#define GS_PARTYEXP_3_SET		(0x00516FE8+3) 
#define GS_PARTYEXP_4_SET		(0x00516FF7+3)
#define GS_PARTYEXP_5_SET		(0x00517006+3)

//================================================//
// Skills										  //
//================================================//
#define GS_TWISTINGSLASH_USELV      (0x004A806B+7)
#define GS_RAGEFULBLOW_USELV        (0x004A8084+7)
#define GS_DEATHSTAB_USELV          (0x004A809D+7)
#define GS_IMPALE_USELV             (0x004A80B6+7)
#define GS_INNERSTRENGTH_USELV      (0x004A80CF+7)
#define GS_PENETRATION_USELV        (0x004A80E8+7)

#define GS_MSNORMAL01				(0x004DB03D+1) 
#define GS_MSNORMAL02				(0x004DB05D+1) 
#define GS_MSMASTER01				(0x004FAA9A+1)
#define GS_MSMASTER02				(0x004FAABA+1)
#define NormalManaShieldTime1		(0x004DB082+1) 


#define MasterManaShieldTime2		(0x004FAB15+1)

#define NormalGreatForitiuteVit01	(0x004DAD67+1)
#define NormalGreatForitiuteVit02	(0x004DADD7+1)
#define NormalGreatForitiuteVit03	(0x004DAE67+1)

#define MasterGreatForitiuteVit01	(0x004FB405+1)
#define MasterGreatForitiuteVit02	(0x004FB478+1)
#define MasterGreatForitiuteVit03	(0x004FB50D+1)

#define NormalGreatForitiuteTime	(0x004DADB1+1)
#define MasterGreatForitiuteTime	(0x004FB452+1)
//
#define GS_GFNORMAL02				(0x004DAD87+1)
#define GS_GFMASTER02				(0x004FB425+1)

//================================================//
//PK Kill Limit									  //
//================================================//
#define GS_PKBugLimitFix1 (0x004D69F9+2) 
#define GS_PKBugLimitFix2 (0x004D6DD8+2) 
#define GS_PKBugLimitFix3 (0x004D6E66+2) 

//================================================//
//Misc											  //
//================================================//
#define GS_MAPEXPLORER			(0x0057A4ED+7)
#define GUARD_SAY        (0x00771CEC+12)
#define LogAdd ((void(*) (BYTE,char*,...)) 0x0040732E) 
#endif

#ifdef _GS_CS
//================================================//
// Maximum Level							      //
//================================================//
#define MaxZen						( 0x005521C7+3 ) 
#define MaxZen1						( 0x00455223+3 ) 
#define MaxZen2						( 0x00455319+3 )
#define MaxZen3						( 0x0045524F+2 )
#define GS_MAX_MASTERLEVEL          0x00508C6F
#define GS_ML_POINTS				0x00508E34

//================================================//
// Maximum Zen								      //
//================================================//
#define GS_NOEXP_LEVEL				0x0057D2E0
#define GS_MAX_LEVEL1				0x004F9FD5
#define GS_MAX_LEVEL2				0x0051F1EA
#define GS_MAX_LEVEL3				0x00596097
#define GS_MAX_LEVEL4				0x005A6D17
#define GS_MAX_LEVEL5				0x005FA039

//================================================//
// Ancient Item Drop Rate					      //
//================================================//
#define GS_ANCREWARD_SKILL_RATE		0x0055226F
#define GS_ANCREWARD_LUCK_RATE		0x00552276
#define GS_ANCRANDOM_SKILL_RATE		0x005524AF
#define	GS_ANCRANDOM_LUCK_RATE		0x005524B6

//================================================//
// Skills Div								      //
//================================================//
#define GS_BERSERKER_DIV1			0x004F7517
#define GS_BERSERKER_DIV2			0x004F752F
#define GS_BERSERKER_DIV3			0x004F7544
#define GS_DAMAGEREF_DIV1			0x004F5B34
#define GS_DAMAGEREF_DIV2			0x004F5B4C
#define GS_CRITICALDMG_COMMAND		0x004F10E4
#define GS_CRITICALDMG_ENERGY		0x004F1104
#define GS_CRITICALDMG_TIME			0x004F1127

//================================================//
//Transformation Rings						      //
//================================================//
#define GS_TRANSFORMATIONRING1	(0x004ED034+3) 
#define GS_TRANSFORMATIONRING2	(0x004ED03D+3)
#define GS_TRANSFORMATIONRING3	(0x004ED046+3)
#define GS_TRANSFORMATIONRING4	(0x004ED04F+3)
#define GS_TRANSFORMATIONRING5	(0x004ED058+3)
#define GS_TRANSFORMATIONRING6	(0x004ED061+3)

//================================================//
//Summon Orbs								      //
//================================================//
#define GS_SUMMONORB1			(0x004EB627+1) 
#define GS_SUMMONORB2 			(0x004EB658+1)
#define GS_SUMMONORB3			(0x004EB689+1) 
#define GS_SUMMONORB4			(0x004EB6BA+1)
#define GS_SUMMONORB5			(0x004EB6EB+1) 
#define GS_SUMMONORB6			(0x004EB71C+1)
#define GS_SUMMONORB7			(0x004EB750+1) 

//================================================//
//Jewel Prices								      //
//================================================//
#define GS_BLESS_PRICE			(0x005651BE+3)
#define GS_SOUL_PRICE			(0x005651E0+3)
#define GS_CHAOS_PRICE			(0x00565201+3)
#define GS_LIFE_PRICE			(0x00565223+3)
#define GS_CREATION_PRICE		(0x00565245+3)
#define GS_GUARDIAN_PRICE		(0x0056531F+3)
#define GS_FRUITS_PRICE			(0x005652C0+3)
#define GS_MONARCH_PRICE		(0x005652EE+3)
#define GS_FEATHER_PRICE		(0x005652FE+3)
#define GS_BLESSPOT_PRICE		(0x00565357+2)
#define GS_SOULPOT_PRICE		(0x0056537D+2) 

//================================================//
//Guild Alliance Members						  //
//================================================//
#define GS_GUILDALLIANCE		(0x00468DCD+2)

//================================================//
//GS UDP Port									  //
//================================================//
#define GS_UDP_PORT				(0x0057D1AD+1)

//================================================//
//Jewel Rates									  //
//================================================//
#define GS_SOUL_RATE_NORMAL		(0x0054A023+3)
#define GS_SOUL_RATE_SPECIAL	(0x00549FE2+3)
#define GS_SOUL_RATE_SOCKET		(0x0054A017+3)
#define GS_SOUL_LUCK_ADD		(0x00549F7C+2)
#define GS_JOL_FAIL_RATE		(0x0054A837+3)

//================================================//
//Chaos Machine Rates						      //
//================================================//
#define GS_CM_MIX_10_NORMAL		(0x006203AF+6)
#define GS_CM_MIX_10_SPECIAL	(0x006203CB+6)
#define GS_CM_MIX_10_SOCKET		(0x006203E8+6)
#define GS_CM_MIX_11_12_NORMAL	(0x0062043F+6)
#define GS_CM_MIX_11_12_SPECIAL	(0x0062045C+6)
#define GS_CM_MIX_11_12_SOCKET	(0x00620479+6) 
#define GS_CM_MIX_13_NORMAL		(0x006204D1+6)
#define GS_CM_MIX_13_SPECIAL	(0x006204EE+6)
#define GS_CM_MIX_13_SOCKET		(0x0062050A+6)
#define GS_CM_MIX_10_LUCK		(0x00620557+6)
#define GS_CM_MIX_11_13_LUCK	(0x00620563+6)
#define GS_CM_MIX_LUCK_ADD		(0x00620548+2)
#define DEFAULTMIXRATE			(0x00620519+6)

//================================================//
//Dark Lord Pet Rates							  //
//================================================//
#define GS_DARK_HORSE			(0x00622C0C+6)
#define GS_DARK_HORSE_PRICE		(0x00622C2B+3)
#define GS_DARK_SPIRIT			(0x0062330D+6)
#define GS_DARK_SPIRIT_PRICE	(0x0062332C+3)

//================================================//
//Item Drop Rates								  //
//================================================//
#define GS_EXC_DROP_RATE          (0x00420B3A+1)
#define GS_EXC_SKILL_RATE         (0x00420F43+6)
#define GS_EXC_LUCK_RATE          (0x00420F4D+6)
#define GS_SKILL_RATE             (0x00420F9C+6)
#define GS_LUCK_RATE              (0x00420FA6+6) 

//================================================//
//Item Looting Time								  //
//================================================//
#define GS_ITEM_TIME1       (0x004CC25A+1)
#define GS_ITEM_TIME2       (0x004CC40B+1)

//================================================//
//Level Up Points							      //
//================================================//
#define GS_LVL_UP_DL1			(0x0051F3CB+2)
#define GS_LVL_UP_DL2			(0x00596214+2) 
#define GS_LVL_UP_DL3			(0x005A6E94+2)
#define GS_LVL_UP_DL4			(0x005FA1B6+2)
#define GS_LVL_UP_DL5			(0x0061BE82+2)

#define GS_LVL_UP_MG1			(0x0051F3F3+2) 
#define GS_LVL_UP_MG2			(0x00596214+2)
#define GS_LVL_UP_MG3			(0x005A6E94+2)
#define GS_LVL_UP_MG4			(0x005FA1B6+2)
#define GS_LVL_UP_MG5			(0x0061BE82+2)

#define GS_LVL_UP_NORMAL1		(0x0051F40A+2) 
#define GS_LVL_UP_NORMAL2		(0x00596245+2)
#define GS_LVL_UP_NORMAL3		(0x005A6EC5+2) 
#define GS_LVL_UP_NORMAL4		(0x005FA1E7+2) 
#define GS_LVL_UP_NORMAL5		(0x0061BEB2+2)

#define GS_LVL_UP_QUEST1		(0x0051F42E+2)
#define GS_LVL_UP_QUEST2		(0x0059628E+2)
#define GS_LVL_UP_QUEST3		(0x005A6F0E+2) 
#define GS_LVL_UP_QUEST4		(0x005FA230+2)
#define GS_LVL_UP_QUEST5		(0x0061BEFC+2) 

//================================================//
//Chaos Machine Mixes							  //
//================================================//
#define GS_WINGLVL1MIX			(0x0062205A+6) 
#define GS_WINGLVL2MIX			(0x00622066+6) 
#define GS_WINGLVL3MIX21		(0x006284F2+6) 
#define GS_WINGLVL3MIX22		(0x006284FE+6) 
#define GS_WINGLVL3MIX11		(0x00627DF0+6)
#define GS_WINGLVL3MIX12		(0x00627DFC+6) 
#define GS_SETITEMMIX1			(0x006292BB+6)
#define GS_SETITEMMIX2			(0x006292C7+6)
#define GS_DINO_MIX				(0x006211C5+6) 

//================================================//
//Party Exp										  //
//================================================//
#define GS_PARTYEXP_2_NORMAL	(0x0052920E+3)
#define GS_PARTYEXP_3_NORMAL	(0x0052921D+3)
#define GS_PARTYEXP_4_NORMAL	(0x0052922C+3)
#define GS_PARTYEXP_5_NORMAL	(0x0052923B+3) 
#define GS_PARTYEXP_3_SET		(0x005291D8+3)
#define GS_PARTYEXP_4_SET		(0x005291E7+3)
#define GS_PARTYEXP_5_SET		(0x005291F6+3)

//================================================//
// Skills										  //
//================================================//
#define GS_TWISTINGSLASH_USELV      (0x004B908B+7)
#define GS_RAGEFULBLOW_USELV        (0x004B90A4+7)
#define GS_DEATHSTAB_USELV          (0x004B90BD+7)
#define GS_IMPALE_USELV             (0x004B90D6+7)
#define GS_INNERSTRENGTH_USELV      (0x004B90EF+7)
#define GS_PENETRATION_USELV        (0x004B9108+7)

#define GS_MSNORMAL01				(0x004EC3CD+1)
#define GS_MSNORMAL02				(0x004EC3ED+1)
#define GS_MSMASTER01				(0x0050BF4A+1)
#define GS_MSMASTER02				(0x0050BF6A+1)
#define NormalManaShieldTime1		(0x004EC412+1)
#define MasterManaShieldTime2		(0x0050BFC5+1)

#define NormalGreatForitiuteVit01   (0x004EC0F7+1)	
#define NormalGreatForitiuteVit02	(0x004EC167+1)
#define NormalGreatForitiuteVit03	(0x004EC1F7+1) 


#define MasterGreatForitiuteVit01	(0x0050C915+1)
#define MasterGreatForitiuteVit02	(0x0050C988+1)
#define MasterGreatForitiuteVit03	(0x0050CA1D+1)

#define GS_GFNORMAL02               (0x004EC0F7+1)
#define GS_GFMASTER02				(0x0050C935+1)
#define NormalGreatForitiuteTime	(0x004EC141+1) 
#define MasterGreatForitiuteTime	(0x0050C962+1)
//================================================//
//PK Kill Limit									  //
//================================================//
#define GS_PKBugLimitFix1 (0x004E7BD9+2) 
#define GS_PKBugLimitFix2 (0x004E7FB8+2) 
#define GS_PKBugLimitFix3 (0x004E8055+2) 

//================================================//
// Castle Siege & Crywolf Offsets				  //
//================================================//
#define GSCS_LOT_MAXPRICE			0x0046D352
#define GSCS_SENIOR_MAXZEN1			0x00435B57
#define GSCS_SENIOR_MAXZEN2			0x00435B75
#define GSCS_LORDMIX_MAXUSE			0x00624585
#define GSCS_CRYWOLF_REWARD_ELF		0x005F623F
#define GSCS_CRYWOLF_REWARD_TOP5	0x005F633F

//================================================//
//Map Explorer									  //
//Defines The Max Number of Maps			      //
//Configurable On GameServerCS					  //
//================================================//
#define GS_MAPEXPLORER			(0x0058D777+7) // Default 22
#define LogAdd ((void(*) (BYTE,char*,...)) 0x00407C07) 
#endif


#ifdef _GS
#define OBJECT_BASE	(*(char**)0x7A5502C)
#define OBJECT_SIZE	0x2228
#define OBJECT_MIN 0x1F40
#define OBJECT_MAX 0x2328
#define OBJECT_STARTUSERINDEX (OBJECT_MAX - 1000)
#endif

#ifdef _GS_CS
#define OBJECT_BASE	(*(char**)0x3E74614)
#define OBJECT_SIZE	0x2238
#define OBJECT_MIN 0x9C4
#define OBJECT_MAX 0x0F9F
#define OBJECT_STARTUSERINDEX (OBJECT_MAX - 1000)
#endif

#define PLAYER_EMPTY  0
#define PLAYER_CONNECTED 1
#define PLAYER_LOGGED 2
#define PLAYER_PLAYING 3

#define OBJECT_MAXUSER 1000
#define OBJECT_POINTER(aIndex) ((aIndex * OBJECT_SIZE) + OBJECT_BASE)
#define OBJECT_TABINDEX(lpObj) ((lpObj - OBJECT_BASE) / OBJECT_SIZE)

enum OBJECT_TYPE 
{
	OBJECT_EMPTY	    = -1,
	OBJECT_USER		    = 1,
	OBJECT_MONSTER	    = 2,
	OBJECT_NPC		    = 3
};


struct ComboSkillData 
{
	/*<thisrel this+0x0>*/ /*|0x4|*/ unsigned long dwTime;
	/*<thisrel this+0x4>*/ /*|0x6|*/ unsigned short Skill[3];
	/*<thisrel this+0xc>*/ /*|0x4|*/ int ProgressIndex;
};

struct VIEWPORT_STRUCT 
{
	/*<thisrel this+0x0>*/ /*|0x1|*/ char state;
	/*<thisrel this+0x2>*/ /*|0x2|*/ short number;
	/*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char type;
	/*<thisrel this+0x6>*/ /*|0x2|*/ short index;
	/*<thisrel this+0x8>*/ /*|0x4|*/ int dis;
};

struct VIEWPORT_PLAYER_STRUCT 
{
	/*<thisrel this+0x0>*/ /*|0x1|*/ char state;
	/*<thisrel this+0x2>*/ /*|0x2|*/ short number;
	/*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char type;
	/*<thisrel this+0x6>*/ /*|0x2|*/ short index;
	/*<thisrel this+0x8>*/ /*|0x4|*/ int dis;
};

struct HITDAMAGE_STRUCT 
{
	/*<thisrel this+0x0>*/ /*|0x2|*/ short number;
	/*<thisrel this+0x4>*/ /*|0x4|*/ int HitDamage;
	/*<thisrel this+0x8>*/ /*|0x4|*/ unsigned long LastHitTime;
};

struct tagInterfaceState 
{
	/*<bitfield this+0x0>*/ /*|0x4|*/ unsigned long use:2;
	/*<bitfield this+0x0>*/ /*|0x4|*/ unsigned long state:4;
	/*<bitfield this+0x0>*/ /*|0x4|*/ unsigned long type:0xA;
};

struct tagActionState 
{
	/*<bitfield this+0x0>*/ /*|0x4|*/ unsigned long Rest:1;
	/*<bitfield this+0x0>*/ /*|0x4|*/ unsigned long Attack:1;
	/*<bitfield this+0x0>*/ /*|0x4|*/ unsigned long Move:1;
	/*<bitfield this+0x0>*/ /*|0x4|*/ unsigned long Escape:1;
	/*<bitfield this+0x0>*/ /*|0x4|*/ unsigned long Emotion:4;
	/*<bitfield this+0x0>*/ /*|0x4|*/ unsigned long EmotionCount:8;
};

class TDurMagicKeyChecker 
{
public:
	/*<thisrel this+0x0>*/ /*|0xf0|*/ unsigned long m_dwDurationTime[60];
	/*<thisrel this+0xf0>*/ /*|0x3c|*/ unsigned char m_btValidCount[60];
};

class CSkillDelay 
{
public:
	/*<thisrel this+0x4>*/ /*|0x400|*/ unsigned long LastSkillUseTime[600];
	CSkillDelay(){};
	virtual ~CSkillDelay(){};
};

struct ITEMOPTION_FOR380ITEM_EFFECT 
{

	/*<thisrel this+0x0>*/ /*|0x2|*/ short OpAddAttackSuccessRatePVP;
	/*<thisrel this+0x2>*/ /*|0x2|*/ short OpAddDamage;
	/*<thisrel this+0x4>*/ /*|0x2|*/ short OpAddDefenseSuccessRatePvP;
	/*<thisrel this+0x6>*/ /*|0x2|*/ short OpAddDefense;
	/*<thisrel this+0x8>*/ /*|0x2|*/ short OpAddMaxHP;
	/*<thisrel this+0xa>*/ /*|0x2|*/ short OpAddMaxSD;
	/*<thisrel this+0xc>*/ /*|0x2|*/ short OpRefillOn;
	/*<thisrel this+0xe>*/ /*|0x2|*/ short OpAddRefillSD;
};

struct JEWELOFHARMONY_ITEM_EFFECT 
{
	/*<thisrel this+0x0>*/ /*|0x2|*/ short HJOpAddMinAttackDamage;
	/*<thisrel this+0x2>*/ /*|0x2|*/ short HJOpAddMaxAttackDamage;
	/*<thisrel this+0x4>*/ /*|0x2|*/ short HJOpRequireStr;
	/*<thisrel this+0x6>*/ /*|0x2|*/ short HJOpRequireDex;
	/*<thisrel this+0x8>*/ /*|0x2|*/ short HJOpAddAttackDamage;
	/*<thisrel this+0xa>*/ /*|0x2|*/ short HJOpAddCriticalDamage;
	/*<thisrel this+0xc>*/ /*|0x2|*/ short HJOpAddSkillAttack;
	/*<thisrel this+0xe>*/ /*|0x2|*/ short HJOpAddAttackSuccessRatePVP;
	/*<thisrel this+0x10>*/ /*|0x2|*/ short HJOpDecreaseSDRate;
	/*<thisrel this+0x12>*/ /*|0x2|*/ short HJOpAddIgnoreSDRate;
	/*<thisrel this+0x14>*/ /*|0x2|*/ short HJOpAddMagicPower;
	/*<thisrel this+0x16>*/ /*|0x2|*/ short HJOpAddDefense;
	/*<thisrel this+0x18>*/ /*|0x2|*/ short HJOpAddMaxAG;
	/*<thisrel this+0x1a>*/ /*|0x2|*/ short HJOpAddMaxHP;
	/*<thisrel this+0x1c>*/ /*|0x2|*/ short HJOpAddRefillHP;
	/*<thisrel this+0x1e>*/ /*|0x2|*/ short HJOpAddRefillMP;
	/*<thisrel this+0x20>*/ /*|0x2|*/ short HJOpAddDefenseSuccessRatePvP;
	/*<thisrel this+0x22>*/ /*|0x2|*/ short HJOpAddDamageDecrease;
	/*<thisrel this+0x24>*/ /*|0x2|*/ short HJOpAddSDRate;
};

class TMonsterAIAgroInfo 
{

	/*<thisrel this+0x0>*/ /*|0x4|*/ int m_iUserIndex;
	/*<thisrel this+0x4>*/ /*|0x4|*/ int m_iAgroValue;

};

class TMonsterAIAgro 
{
public:
	/*<thisrel this+0x4>*/ /*|0x320|*/ class TMonsterAIAgroInfo m_Agro[100];
	TMonsterAIAgro(){};
	virtual ~TMonsterAIAgro(){};
};

class TMonsterAIMovePathInfo 
{
public:
	/*<thisrel this+0x0>*/ /*|0x4|*/ int m_iType;
	/*<thisrel this+0x4>*/ /*|0x4|*/ int m_iMapNumber;
	/*<thisrel this+0x8>*/ /*|0x4|*/ int m_iPathX;
	/*<thisrel this+0xc>*/ /*|0x4|*/ int m_iPathY;
	TMonsterAIMovePathInfo(){};
};

class TMonsterAIMovePath 
{
public:
	/*<thisrel this+0x4>*/ /*|0x4|*/ int m_bDataLoad;
	/*<thisrel this+0x8>*/ /*|0x12c0|*/ class TMonsterAIMovePathInfo m_MovePathInfo[300];
	/*<thisrel this+0x12c8>*/ /*|0x4|*/ int m_iMovePathSpotCount;
	TMonsterAIMovePath(){};

	virtual ~TMonsterAIMovePath(){};
};

class TMonsterSkillElementInfo 
{
public:
	/*<thisrel this+0x0>*/ /*|0x4|*/ int m_iSkillElementDefense;
	/*<thisrel this+0x4>*/ /*|0x4|*/ int m_iSkillElementDefenseTime;
	/*<thisrel this+0x8>*/ /*|0x4|*/ int m_iSkillElementAttack;
	/*<thisrel this+0xc>*/ /*|0x4|*/ int m_iSkillElementAttackTime;
	/*<thisrel this+0x10>*/ /*|0x4|*/ int m_iSkillElementAutoHP;
	/*<thisrel this+0x14>*/ /*|0x4|*/ int m_iSkillElementAutoHPCycle;
	/*<thisrel this+0x18>*/ /*|0x4|*/ int m_iSkillElementAutoHPTime;
	/*<thisrel this+0x1c>*/ /*|0x4|*/ int m_iSkillElementAutoMP;
	/*<thisrel this+0x20>*/ /*|0x4|*/ int m_iSkillElementAutoMPCycle;
	/*<thisrel this+0x24>*/ /*|0x4|*/ int m_iSkillElementAutoMPTime;
	/*<thisrel this+0x28>*/ /*|0x4|*/ int m_iSkillElementAutoAG;
	/*<thisrel this+0x2c>*/ /*|0x4|*/ int m_iSkillElementAutoAGCycle;
	/*<thisrel this+0x30>*/ /*|0x4|*/ int m_iSkillElementAutoAGTime;
	/*<thisrel this+0x34>*/ /*|0x4|*/ int m_iSkillElementImmuneNumber;
	/*<thisrel this+0x38>*/ /*|0x4|*/ int m_iSkillElementImmuneTime;
	/*<thisrel this+0x3c>*/ /*|0x4|*/ int m_iSkillElementResistNumber;
	/*<thisrel this+0x40>*/ /*|0x4|*/ int m_iSkillElementResistTime;
	/*<thisrel this+0x44>*/ /*|0x4|*/ int m_iSkillElementModifyStat;
	/*<thisrel this+0x48>*/ /*|0x4|*/ int m_iSkillElementModifyStatType;
	/*<thisrel this+0x4c>*/ /*|0x4|*/ int m_iSkillElementModifyStatTime;
	TMonsterSkillElementInfo(){};
};

struct MONSTERKILLINFO
{
	int MonIndex;
	int KillCount;
};

struct OBJECTSTRUCT
{										
	/*<thisrel this+0x0>*/ /*|0x4|*/ int m_Index;
	/*<thisrel this+0x4>*/ /*|0x4|*/ int Connected;
	/*<thisrel this+0x8>*/ /*|0x1|*/ char LoginMsgSnd;
	/*<thisrel this+0x9>*/ /*|0x1|*/ char LoginMsgCount;
	/*<thisrel this+0xa>*/ /*|0x1|*/ char CloseCount;
	/*<thisrel this+0xb>*/ /*|0x1|*/ char CloseType;
	/*<thisrel this+0xc>*/ /*|0x4|*/ int bEnableDelCharacter;
	/*<thisrel this+0x10>*/ /*|0x4|*/ struct _PER_SOCKET_CONTEXT* PerSocketContext;
	/*<thisrel this+0x14>*/ /*|0x4|*/ unsigned int m_socket;
	/*<thisrel this+0x18>*/ /*|0x10|*/ char Ip_addr[16];
	/*<thisrel this+0x28>*/ /*|0x4|*/ int UserNumber;
	/*<thisrel this+0x2c>*/ /*|0x4|*/ int DBNumber;
	/*<thisrel this+0x30>*/ /*|0x1|*/ unsigned char Magumsa;
	/*<thisrel this+0x34>*/ /*|0x4|*/ unsigned long AutoSaveTime;
	/*<thisrel this+0x38>*/ /*|0x4|*/ unsigned long ConnectCheckTime;
	/*<thisrel this+0x3c>*/ /*|0x4|*/ unsigned long CheckTick;
	/*<thisrel this+0x40>*/ /*|0x1|*/ unsigned char CheckSpeedHack;
	/*<thisrel this+0x44>*/ /*|0x4|*/ unsigned long CheckTick2;
	/*<thisrel this+0x48>*/ /*|0x1|*/ unsigned char CheckTickCount;
	/*<thisrel this+0x4c>*/ /*|0x4|*/ unsigned long ukn_4c;
	/*<thisrel this+0x50>*/ /*|0x4|*/ unsigned long SaveTimeForStatics;
	/*<thisrel this+0x54>*/ /*|0x4|*/ int iPingTime;
	/*<thisrel this+0x58>*/ /*|0x1|*/ unsigned char m_TimeCount;
	/*<thisrel this+0x5C>*/ /*|0x4|*/ unsigned long m_dwPKTimer;
	/*<thisrel this+0x60>*/ /*|0x2|*/ short CheckSumTableNum;
	/*<thisrel this+0x64>*/ /*|0x4|*/ unsigned long CheckSumTime;
	/*<thisrel this+0x68>*/ /*|0x2|*/ unsigned short Type;
	/*<thisrel this+0x6a>*/ /*|0x1|*/ unsigned char Live;
	/*<thisrel this+0x6b>*/ /*|0x1|*/ char CharacterPos;
	/*<thisrel this+0x6C>*/ /*|0xb|*/ char AccountID[11];
	/*<thisrel this+0x77>*/ /*|0xb|*/ char Name[11];
	/*<thisrel this+0x82>*/ /*|0xe|*/ char LastJoominNumber[14];
	/*<thisrel this+0x90>*/ /*|0x1|*/ unsigned char PlusStatQuestClear;
	/*<thisrel this+0x91>*/ /*|0x1|*/ unsigned char ComboSkillquestClear;
	/*<thisrel this+0x94>*/ /*|0x10|*/ struct ComboSkillData comboSkill;
	/*<thisrel this+0xa4>*/ /*|0x2|*/ unsigned short Class;
	/*<thisrel this+0xa6>*/ /*|0x1|*/ unsigned char DbClass;
	/*<thisrel this+0xa7>*/ /*|0x1|*/ unsigned char ChangeUP;
	/*<thisrel this+0xa8>*/ /*|0x1|*/ unsigned char ChangeUP2; 
	/*<thisrel this+0xaa>*/ /*|0x2|*/ short Level;
	/*<thisrel this+0xac>*/ /*|0x4|*/ int LevelUpPoint;
	/*<thisrel this+0xb0>*/ /*|0x4|*/ int iFruitPoint;
	/*<thisrel this+0xb4>*/ /*|0x4|*/ unsigned long Experience;
	/*<thisrel this+0xb8>*/ /*|0x4|*/ unsigned long NextExp;
	/*<thisrel this+0xbc>*/ /*|0x2|*/ short MLevel;
	/*<thisrel this+0xc0>*/ /*|0x8|*/ __int64 MLExp;
	/*<thisrel this+0xc8>*/ /*|0x8|*/ __int64 MLNextExp;
	/*<thisrel this+0xd0>*/ /*|0x4|*/ int MLPoint;
	/*<thisrel this+0xd4>*/ /*|0x4|*/ int MLInfoLoad;
	/*<thisrel this+0xd8>*/ /*|0x4|*/ int Money;
	/*<thisrel this+0xdc>*/ /*|0x2|*/ short Strength;
	/*<thisrel this+0xde>*/ /*|0x2|*/ short Dexterity;
	/*<thisrel this+0xe0>*/ /*|0x2|*/ short Vitality;
	/*<thisrel this+0xe2>*/ /*|0x2|*/ short Energy;
	/*<thisrel this+0xe4>*/ /*|0x4|*/ float Life;
	/*<thisrel this+0xe8>*/ /*|0x4|*/ float MaxLife;
	/*<thisrel this+0xec>*/ /*|0x4|*/ int m_iScriptMaxLife;
	/*<thisrel this+0xf0>*/ /*|0x4|*/ float FillLife;
	/*<thisrel this+0xf4>*/ /*|0x4|*/ float FillLifeMax;
	/*<thisrel this+0xf8>*/ /*|0x4|*/ float Mana;
	/*<thisrel this+0xfc>*/ /*|0x4|*/ float MaxMana;
	/*<thisrel this+0x100>*/ /*|0x2|*/ unsigned short Leadership;
	/*<thisrel this+0x102>*/ /*|0x2|*/ unsigned short AddLeadership;
	/*<thisrel this+0x104>*/ /*|0x2|*/ unsigned short ChatLimitTime;
	/*<thisrel this+0x106>*/ /*|0x1|*/ unsigned char ChatLimitTimeSec;
	/*<thisrel this+0x107>*/ /*|0x1|*/ unsigned char FillLifeCount;
	/*<thisrel this+0x108>*/ /*|0x2|*/ short AddStrength;
	/*<thisrel this+0x10a>*/ /*|0x2|*/ short AddDexterity;
	/*<thisrel this+0x10c>*/ /*|0x2|*/ short AddVitality;
	/*<thisrel this+0x10e>*/ /*|0x2|*/ short AddEnergy;
	/*<thisrel this+0x110>*/ /*|0x4|*/ int BP;
	/*<thisrel this+0x114>*/ /*|0x4|*/ int MaxBP;
	/*<thisrel this+0x118>*/ /*|0x4|*/ int AddBP;
	/*<thisrel this+0x11c>*/ /*|0x4|*/ float VitalityToLife;
	/*<thisrel this+0x120>*/ /*|0x4|*/ float EnergyToMana;
	/*<thisrel this+0x124>*/ /*|0x1|*/ char m_PK_Count;
	/*<thisrel this+0x125>*/ /*|0x1|*/ char m_PK_Level;
	/*<thisrel this+0x128>*/ /*|0x4|*/ int m_PK_Time;
	/*<thisrel this+0x12c>*/ /*|0x2|*/ short X;
	/*<thisrel this+0x12e>*/ /*|0x2|*/ short Y;
	/*<thisrel this+0x130>*/ /*|0x1|*/ unsigned char Dir;
	/*<thisrel this+0x131>*/ /*|0x1|*/ unsigned char MapNumber;
	/*<thisrel this+0x134>*/ /*|0x4|*/ int AddLife;
	/*<thisrel this+0x138>*/ /*|0x4|*/ int AddMana;
	/*<thisrel this+0x13c>*/ /*|0x4|*/ int iShield;
	/*<thisrel this+0x140>*/ /*|0x4|*/ int iMaxShield;
	/*<thisrel this+0x144>*/ /*|0x4|*/ int iAddShield;
	/*<thisrel this+0x148>*/ /*|0x4|*/ int iFillShieldMax;
	/*<thisrel this+0x14c>*/ /*|0x4|*/ int iFillShield;
	/*<thisrel this+0x150>*/ /*|0x4|*/ int iFillShieldCount;
	/*<thisrel this+0x154>*/ /*|0x4|*/ unsigned long dwShieldAutoRefillTimer;
	/*<thisrel this+0x158>*/ /*|0x1|*/ unsigned char DamageMinus;
	/*<thisrel this+0x159>*/ /*|0x1|*/ unsigned char DamageReflect;
	/*<thisrel this+0x15A>*/ /*|0x2|*/ short MonsterDieGetMoney;
	/*<thisrel this+0x15C>*/ /*|0x1|*/ unsigned char MonsterDieGetLife;
	/*<thisrel this+0x15D>*/ /*|0x1|*/ unsigned char MonsterDieGetMana;
	/*<thisrel this+0x15e>*/ /*|0x1|*/ unsigned char StartX;
	/*<thisrel this+0x15f>*/ /*|0x1|*/ unsigned char StartY;
	/*<thisrel this+0x160>*/ /*|0x2|*/ short m_OldX;
	/*<thisrel this+0x162>*/ /*|0x2|*/ short m_OldY;
	/*<thisrel this+0x164>*/ /*|0x2|*/ short TX;
	/*<thisrel this+0x166>*/ /*|0x2|*/ short TY;
	/*<thisrel this+0x168>*/ /*|0x2|*/ short MTX;
	/*<thisrel this+0x16a>*/ /*|0x2|*/ short MTY;
	/*<thisrel this+0x16c>*/ /*|0x4|*/ int PathCount;
	/*<thisrel this+0x170>*/ /*|0x4|*/ int PathCur;
	/*<thisrel this+0x174>*/ /*|0x1|*/ char PathStartEnd;
	/*<thisrel this+0x176>*/ /*|0x1e|*/ short PathOri[15];
	/*<thisrel this+0x194>*/ /*|0x1e|*/ short PathX[15];
	/*<thisrel this+0x1b2>*/ /*|0x1e|*/ short PathY[15];
	/*<thisrel this+0x1d0>*/ /*|0xf|*/ char PathDir[15];
	/*<thisrel this+0x1e0>*/ /*|0x4|*/ unsigned long PathTime;
	/*<thisrel this+0x1e4>*/ /*|0x1|*/ char m_MoveGateNumber;
	/*<thisrel this+0x1e6>*/ /*|0x2|*/ short m_wRaiseTalismanSaveX;
	/*<thisrel this+0x1e8>*/ /*|0x2|*/ short m_wRaiseTalismanSaveY;
	/*<thisrel this+0x1ea>*/ /*|0x2|*/ short m_wRaiseTalismanSaveMap;
	/*<thisrel this+0x1ec>*/ /*|0x2|*/ short m_wTeleportTalismanSaveX;
	/*<thisrel this+0x1ee>*/ /*|0x2|*/ short m_wTeleportTalismanSaveY;
	/*<thisrel this+0x1f0>*/ /*|0x2|*/ short m_wTeleportTalismanSaveMap;
	/*<thisrel this+0x1f4>*/ /*|0x4|*/ unsigned long Authority;
	/*<thisrel this+0x1f8>*/ /*|0x4|*/ unsigned long Penalty;
	/*<thisrel this+0x1fc>*/ /*|0x1|*/ unsigned char m_cAccountItemBlock;
	/*<thisrel this+0x200>*/ /*|0x4|*/ struct tagActionState m_ActState;
	/*<thisrel this+0x204>*/ /*|0x1|*/ unsigned char m_ActionNumber;
	/*<thisrel this+0x208>*/ /*|0x4|*/ unsigned long m_State;
	/*<thisrel this+0x20c>*/ /*|0x1|*/ char m_StateSub;
	/*<thisrel this+0x20d>*/ /*|0x1|*/ unsigned char m_Rest;
	/*<thisrel this+0x20e>*/ /*|0x1|*/ char m_ViewState;
	/*<thisrel this+0x210>*/ /*|0x140|*/ char ukn_210[0x14][0x10];
	/*<thisrel this+0x350>*/ /*|0x4|*/ unsigned long m_LastMoveTime;
	/*<thisrel this+0x354>*/ /*|0x4|*/ unsigned long m_LastAttackTime;
	/*<thisrel this+0x358>*/ /*|0x1|*/ unsigned char m_FriendServerOnline;
	/*<thisrel this+0x35c>*/ /*|0x4|*/ int m_DetectSpeedHackTime;
	/*<thisrel this+0x360>*/ /*|0x4|*/ unsigned long m_SumLastAttackTime;
	/*<thisrel this+0x364>*/ /*|0x4|*/ unsigned long m_DetectCount;
	/*<thisrel this+0x368>*/ /*|0x4|*/ int m_DetectedHackKickCount;
	/*<thisrel this+0x36c>*/ /*|0x4|*/ int m_SpeedHackPenalty;
	/*<thisrel this+0x370>*/ /*|0x1|*/ unsigned char m_AttackSpeedHackDetectedCount;
	/*<thisrel this+0x374>*/ /*|0x4|*/ unsigned long m_PacketCheckTime;
	/*<thisrel this+0x378>*/ /*|0x1|*/ unsigned char m_ShopTime;
	/*<thisrel this+0x37c>*/ /*|0x4|*/ unsigned long m_TotalAttackTime;
	/*<thisrel this+0x380>*/ /*|0x4|*/ int m_TotalAttackCount;
	/*<thisrel this+0x384>*/ /*|0x4|*/ unsigned long TeleportTime;
	/*<thisrel this+0x388>*/ /*|0x1|*/ char Teleport;
	/*<thisrel this+0x389>*/ /*|0x1|*/ char KillerType;
	/*<thisrel this+0x38a>*/ /*|0x1|*/ char DieRegen;
	/*<thisrel this+0x38b>*/ /*|0x1|*/ char RegenOk;
	/*<thisrel this+0x38c>*/ /*|0x1|*/ unsigned char RegenMapNumber;
	/*<thisrel this+0x38d>*/ /*|0x1|*/ unsigned char RegenMapX;
	/*<thisrel this+0x38e>*/ /*|0x1|*/ unsigned char RegenMapY;
	/*<thisrel this+0x390>*/ /*|0x4|*/ unsigned long RegenTime;
	/*<thisrel this+0x394>*/ /*|0x4|*/ unsigned long MaxRegenTime;
	/*<thisrel this+0x398>*/ /*|0x2|*/ short m_PosNum;
	/*<thisrel this+0x39c>*/ /*|0x4|*/ unsigned long LifeRefillTimer;
	/*<thisrel this+0x3a0>*/ /*|0x4|*/ unsigned long CurActionTime;
	/*<thisrel this+0x3a4>*/ /*|0x4|*/ unsigned long NextActionTime;
	/*<thisrel this+0x3a8>*/ /*|0x4|*/ unsigned long DelayActionTime;
	/*<thisrel this+0x3ac>*/ /*|0x1|*/ char DelayLevel;
	/*<thisrel this+0x3ad>*/ /*|0x1|*/ char m_PoisonType;
	/*<thisrel this+0x3ae>*/ /*|0x1|*/ char m_IceType;
	/*<thisrel this+0x3af>*/ /*|0x1|*/ char ukn_3af;
	/*<thisrel this+0x3b0>*/ /*|0x1|*/ char ukn_3b0;
	/*<thisrel this+0x3b1>*/ /*|0x1|*/ char m_iMonsterBattleDelay;
	/*<thisrel this+0x3b2>*/ /*|0x1|*/ char m_cKalimaGateExist;
	/*<thisrel this+0x3b4>*/ /*|0x4|*/ int m_iKalimaGateIndex;
	/*<thisrel this+0x3b8>*/ /*|0x1|*/ char m_cKalimaGateEnterCount;
	/*<thisrel this+0x3bc>*/ /*|0x4|*/ struct OBJECTSTRUCT* lpAttackObj;
	/*<thisrel this+0x3c0>*/ /*|0x2|*/ short m_SkillNumber;
	/*<thisrel this+0x3c4>*/ /*|0x4|*/ unsigned long m_SkillTime;
	/*<thisrel this+0x3c8>*/ /*|0x1|*/ unsigned char m_bAttackerKilled;
	/*<thisrel this+0x3c9>*/ /*|0x1|*/ char m_ManaFillCount;
	/*<thisrel this+0x3ca>*/ /*|0x1|*/ char m_LifeFillCount;
	/*<thisrel this+0x3cc>*/ /*|0x1C|*/ int SelfDefense[7];
	/*<thisrel this+0x3e8>*/ /*|0x1C|*/ unsigned long SelfDefenseTime[7];
	/*<thisrel this+0x404>*/ /*|0x4|*/ unsigned long MySelfDefenseTime;
	/*<thisrel this+0x408>*/ /*|0x1|*/ char m_Drink;
	/*<thisrel this+0x40c>*/ /*|0x4|*/ int m_SkillAttack;
	/*<thisrel this+0x410>*/ /*|0x1|*/ char m_SkillAttackTime;
	/*<thisrel this+0x414>*/ /*|0x4|*/ int PartyNumber;
	/*<thisrel this+0x418>*/ /*|0x4|*/ int PartyTargetUser;
	/*<thisrel this+0x41c>*/ /*|0x4|*/ int GuildNumber;
	/*<thisrel this+0x420>*/ /*|0x4|*/ struct _GUILD_INFO_STRUCT* lpGuild;
	/*<thisrel this+0x424>*/ /*|0xb|*/ char GuildName[11];
	/*<thisrel this+0x430>*/ /*|0x4|*/ int GuildStatus;
	/*<thisrel this+0x434>*/ /*|0x4|*/ int iGuildUnionTimeStamp;
	/*<thisrel this+0x438>*/ /*|0x4|*/ int m_RecallMon;
	/*<thisrel this+0x43c>*/ /*|0x4|*/ int m_Change;
	/*<thisrel this+0x440>*/ /*|0x2|*/ short TargetNumber;
	/*<thisrel this+0x442>*/ /*|0x2|*/ short TargetShopNumber;
	/*<thisrel this+0x444>*/ /*|0x2|*/ short ShopNumber;
	/*<thisrel this+0x446>*/ /*|0x2|*/ short LastAttackerID;
	/*<thisrel this+0x448>*/ /*|0x4|*/ int m_AttackDamageMin;
	/*<thisrel this+0x44c>*/ /*|0x4|*/ int m_AttackDamageMax;
	/*<thisrel this+0x450>*/ /*|0x4|*/ int m_MagicDamageMin;
	/*<thisrel this+0x454>*/ /*|0x4|*/ int m_MagicDamageMax;
	/*<thisrel this+0x458>*/ /*|0x4|*/ int m_AttackDamageLeft;
	/*<thisrel this+0x45c>*/ /*|0x4|*/ int m_AttackDamageRight;
	/*<thisrel this+0x460>*/ /*|0x4|*/ int m_AttackDamageMaxLeft;
	/*<thisrel this+0x464>*/ /*|0x4|*/ int m_AttackDamageMinLeft;
	/*<thisrel this+0x468>*/ /*|0x4|*/ int m_AttackDamageMaxRight;
	/*<thisrel this+0x46c>*/ /*|0x4|*/ int m_AttackDamageMinRight;
	/*<thisrel this+0x470>*/ /*|0x4|*/ int m_AttackRating;
	/*<thisrel this+0x474>*/ /*|0x4|*/ int m_AttackSpeed;
	/*<thisrel this+0x478>*/ /*|0x4|*/ int m_MagicSpeed;
	/*<thisrel this+0x47c>*/ /*|0x4|*/ int m_Defense;
	/*<thisrel this+0x480>*/ /*|0x4|*/ int m_MagicDefense;
	/*<thisrel this+0x484>*/ /*|0x4|*/ int m_SuccessfulBlocking;
	/*<thisrel this+0x488>*/ /*|0x4|*/ int NewVar_488;
	/*<thisrel this+0x48c>*/ /*|0x4|*/ int NewVar_48c;
	/*<thisrel this+0x490>*/ /*|0x4|*/ int NewVar_490;
	/*<thisrel this+0x494>*/ /*|0x2|*/ short m_MoveSpeed;
	/*<thisrel this+0x496>*/ /*|0x2|*/ short m_MoveRange;
	/*<thisrel this+0x498>*/ /*|0x2|*/ short m_AttackRange;
	/*<thisrel this+0x49a>*/ /*|0x2|*/ short m_AttackType;
	/*<thisrel this+0x49c>*/ /*|0x2|*/ short m_ViewRange;
	/*<thisrel this+0x49e>*/ /*|0x2|*/ short m_Attribute;
	/*<thisrel this+0x4a0>*/ /*|0x2|*/ short m_ItemRate;
	/*<thisrel this+0x4a2>*/ /*|0x2|*/ short m_MoneyRate;
	/*<thisrel this+0x4a4>*/ /*|0x4|*/ int m_CriticalDamage;
	/*<thisrel this+0x4a8>*/ /*|0x4|*/ int m_ExcelentDamage;
	/*<thisrel this+0x4ac>*/ /*|0x4|*/ class CMagicInf* m_lpMagicBack;
	/*<thisrel this+0x4b0>*/ /*|0x4|*/ class CMagicInf* Magic;
	/*<thisrel this+0x4b4>*/ /*|0x1|*/ char MagicCount;
	/*<thisrel this+0x4b5>*/ /*|0x1|*/ unsigned char UseMagicNumber;
	/*<thisrel this+0x4b8>*/ /*|0x4|*/ unsigned long UseMagicTime;
	/*<thisrel this+0x4bc>*/ /*|0x1|*/ char UseMagicCount;
	/*<thisrel this+0x4be>*/ /*|0x2|*/ short OSAttackSerial;
	/*<thisrel this+0x4c0>*/ /*|0x1|*/ unsigned char SASCount;
	/*<thisrel this+0x4c4>*/ /*|0x4|*/ unsigned long SkillAttackTime;
	/*<thisrel this+0x4c8>*/ /*|0x70|*/ unsigned char NewVar_4c8[0x70];
	/*<thisrel this+0x538>*/ /*|0x4|*/ int NewVar_538;
	/*<thisrel this+0x53c>*/ /*|0x2|*/ short NewVar_53c;
	/*<thisrel this+0x53e>*/ /*|0x12|*/ unsigned char CharSet[18];
	/*<thisrel this+0x550>*/ /*|0x7|*/ unsigned char m_Resistance[7];
	/*<thisrel this+0x557>*/ /*|0x7|*/ unsigned char m_AddResistance[7];
	/*<thisrel this+0x560>*/ /*|0x10|*/ int FrustrumX[4];
	/*<thisrel this+0x570>*/ /*|0x10|*/ int FrustrumY[4];
	/*<thisrel this+0x580>*/ /*|0x384|*/ struct VIEWPORT_STRUCT VpPlayer[75];
	/*<thisrel this+0x904>*/ /*|0x384|*/ struct VIEWPORT_PLAYER_STRUCT VpPlayer2[75];
	/*<thisrel this+0xc88>*/ /*|0x4|*/ int VPCount;
	/*<thisrel this+0xc8c>*/ /*|0x4|*/ int VPCount2;
	/*<thisrel this+0xc90>*/ /*|0x1e0|*/ struct HITDAMAGE_STRUCT sHD[40];
	/*<thisrel this+0xe70>*/ /*|0x2|*/ short sHDCount;
	/*<thisrel this+0xe74>*/ /*|0x4|*/ struct tagInterfaceState m_IfState;
	/*<thisrel this+0xe78>*/ /*|0x4|*/ unsigned long m_InterfaceTime;
	/*<thisrel this+0xe7c>*/ /*|0x4|*/ class CItem* pInventory;
	/*<thisrel this+0xe80>*/ /*|0x4|*/ unsigned char* pInventoryMap;
	/*<thisrel this+0xe84>*/ /*|0x4|*/ char* pInventoryCount;
	/*<thisrel this+0xe88>*/ /*|0x1|*/ char pTransaction;
	/*<thisrel this+0xe8c>*/ /*|0x4|*/ class CItem* Inventory1;
	/*<thisrel this+0xe90>*/ /*|0x4|*/ unsigned char* InventoryMap1;
	/*<thisrel this+0xe94>*/ /*|0x1|*/ char InventoryCount1;
	/*<thisrel this+0xe98>*/ /*|0x4|*/ class CItem* Inventory2;
	/*<thisrel this+0xe9c>*/ /*|0x4|*/ unsigned char* InventoryMap2;
	/*<thisrel this+0xea0>*/ /*|0x1|*/ char InventoryCount2;
	/*<thisrel this+0xea4>*/ /*|0x4|*/ class CItem* Trade;
	/*<thisrel this+0xea8>*/ /*|0x4|*/ unsigned char* TradeMap;
	/*<thisrel this+0xeac>*/ /*|0x4|*/ int TradeMoney;
	/*<thisrel this+0xeb0>*/ /*|0x1|*/ unsigned char TradeOk;
	/*<thisrel this+0xeb4>*/ /*|0x4|*/ class CItem* pWarehouse;
	/*<thisrel this+0xeb8>*/ /*|0x4|*/ unsigned char* pWarehouseMap;
	/*<thisrel this+0xebc>*/ /*|0x1|*/ char WarehouseCount;
	/*<thisrel this+0xebe>*/ /*|0x2|*/ short WarehousePW;
	/*<thisrel this+0xec0>*/ /*|0x1|*/ unsigned char WarehouseLock;
	/*<thisrel this+0xec1>*/ /*|0x1|*/ unsigned char WarehouseUnfailLock;
	/*<thisrel this+0xec4>*/ /*|0x4|*/ int WarehouseMoney;
	/*<thisrel this+0xec8>*/ /*|0x4|*/ int WarehouseSave;
	/*<thisrel this+0xecc>*/ /*|0x4|*/ class CItem* pChaosBox;
	/*<thisrel this+0xed0>*/ /*|0x4|*/ unsigned char* pChaosBoxMap;
	/*<thisrel this+0xed4>*/ /*|0x4|*/ int ChaosMoney;
	/*<thisrel this+0xed8>*/ /*|0x4|*/ int ChaosSuccessRate;
	/*<thisrel this+0xedc>*/ /*|0x4|*/ int ChaosLock;
	/*<thisrel this+0xee0>*/ /*|0x4|*/ unsigned long m_Option;
	/*<thisrel this+0xee4>*/ /*|0x4|*/ int m_nEventScore;
	/*<thisrel this+0xee8>*/ /*|0x4|*/ int m_nEventExp;
	/*<thisrel this+0xeec>*/ /*|0x4|*/ int m_nEventMoney;
	/*<thisrel this+0xef0>*/ /*|0x1|*/ unsigned char m_bDevilSquareIndex;
	/*<thisrel this+0xef1>*/ /*|0x1|*/ unsigned char m_bDevilSquareAuth;
	/*<thisrel this+0xef2>*/ /*|0x1|*/ char m_cBloodCastleIndex;
	/*<thisrel this+0xef3>*/ /*|0x1|*/ char m_cBloodCastleSubIndex;
	/*<thisrel this+0xef4>*/ /*|0x4|*/ int m_iBloodCastleEXP;
	/*<thisrel this+0xef8>*/ /*|0x1|*/ unsigned char m_bBloodCastleComplete;
	/*<thisrel this+0xef9>*/ /*|0x1|*/ char m_cChaosCastleIndex;
	/*<thisrel this+0xefa>*/ /*|0x1|*/ char m_cChaosCastleSubIndex;
	/*<thisrel this+0xefc>*/ /*|0x4|*/ int m_iChaosCastleBlowTime;
	/*<thisrel this+0xf00>*/ /*|0x1|*/ char m_cKillUserCount;
	/*<thisrel this+0xf01>*/ /*|0x1|*/ char m_cKillMonsterCount;
	/*<thisrel this+0xf04>*/ /*|0x4|*/ int m_iDuelUserReserved;
	/*<thisrel this+0xf08>*/ /*|0x4|*/ int m_iDuelUserRequested;
	/*<thisrel this+0xf0c>*/ /*|0x4|*/ int m_iDuelUser;
	/*<thisrel this+0xf10>*/ /*|0x1|*/ unsigned char m_btDuelScore;
	/*<thisrel this+0xf14>*/ /*|0x4|*/ int m_iDuelTickCount;
	/*<thisrel this+0xf18>*/ /*|0x1|*/ unsigned char m_bPShopOpen;
	/*<thisrel this+0xf19>*/ /*|0x1|*/ unsigned char m_bPShopTransaction;
	/*<thisrel this+0xf1a>*/ /*|0x1|*/ unsigned char m_bPShopItemChange;
	/*<thisrel this+0xf1b>*/ /*|0x1|*/ unsigned char m_bPShopRedrawAbs;
	/*<thisrel this+0xf1c>*/ /*|0x24|*/ char m_szPShopText[36];
	/*<thisrel this+0xf40>*/ /*|0x1|*/ unsigned char m_bPShopWantDeal;
	/*<thisrel this+0xf44>*/ /*|0x4|*/ int m_iPShopDealerIndex;
	/*<thisrel this+0xf48>*/ /*|0xa|*/ char m_szPShopDealerName[10];
	/*<thisrel this+0xf54>*/ /*|0x18|*/ struct _RTL_CRITICAL_SECTION m_critPShopTrade;
	/*<thisrel this+0xf6c>*/ /*|0x12c|*/ int m_iVpPShopPlayer[75];
	/*<thisrel this+0x1098>*/ /*|0x2|*/ unsigned short m_wVpPShopPlayerCount;
	/*<thisrel this+0x109a>*/ /*|0x1|*/ unsigned char IsInBattleGround;
	/*<thisrel this+0x109b>*/ /*|0x1|*/ unsigned char HaveWeaponInHand;
	/*<thisrel this+0x109c>*/ /*|0x2|*/ short EventChipCount;
	/*<thisrel this+0x10a0>*/ /*|0x4|*/ int MutoNumber;
	/*<thisrel this+0x10a4>*/ /*|0x4|*/ int UseEventServer;
	/*<thisrel this+0x10a8>*/ /*|0x1|*/ unsigned char LoadWareHouseInfo;
	/*<thisrel this+0x10ac>*/ /*|0x4|*/ int iStoneCount;
	/*<thisrel this+0x10b0>*/ /*|0x4|*/ int ukn_10b0;
	/*<thisrel this+0x10b4>*/ /*|0x4|*/ int ukn_10b4;
	/*<thisrel this+0x10b8>*/ /*|0x28|*/ MONSTERKILLINFO m_QuestMonsterKillInfo[5];
	/*<thisrel this+0x10e0>*/ /*|0x32|*/ unsigned char m_Quest[50];
	/*<thisrel this+0x1112>*/ /*|0x1|*/ unsigned char m_SendQuestInfo;
	/*<thisrel this+0x1114>*/ /*|0x4|*/ int m_SkyBossMonSheildLinkIndex;
	/*<thisrel this+0x1118>*/ /*|0x4|*/ int m_SkyBossMonSheild;
	/*<thisrel this+0x111c>*/ /*|0x4|*/ int m_SkyBossMonSheildTime;
	/*<thisrel this+0x1120>*/ /*|0x4|*/ int m_MaxLifePower;
	/*<thisrel this+0x1124>*/ /*|0x4|*/ int m_PacketChecksumTime;
	/*<thisrel this+0x1128>*/ /*|0x4|*/ int m_CheckLifeTime;
	/*<thisrel this+0x112c>*/ /*|0x1|*/ unsigned char m_MoveOtherServer;
	/*<thisrel this+0x112d>*/ /*|0xb|*/ char BackName[11];
	/*<thisrel this+0x1138>*/ /*|0x1|*/ char m_BossGoldDerconMapNumber;
	/*<thisrel this+0x1139>*/ /*|0x1|*/ unsigned char m_InWebzen;
	/*<thisrel this+0x113a>*/ /*|0x1|*/ char m_LastTeleportTime;
	/*<thisrel this+0x113b>*/ /*|0x1|*/ unsigned char m_ClientHackLogCount;
	/*<thisrel this+0x113c>*/ /*|0x4|*/ int m_bIsInMonsterHerd;
	/*<thisrel this+0x1140>*/ /*|0x4|*/ int m_bIsMonsterAttackFirst;
	/*<thisrel this+0x1144>*/ /*|0x4|*/ class MonsterHerd* m_lpMonsterHerd;
	/*<thisrel this+0x1148>*/ /*|0x2c|*/ char NPggCSAuth[0x2c];
	/*<thisrel this+0x1174>*/ /*|0x1|*/ unsigned char m_bSentGGAuth;
	/*<thisrel this+0x1178>*/ /*|0x4|*/ unsigned long m_NPggCheckSumSendTime;
	/*<thisrel this+0x117c>*/ /*|0x10|*/ int fSkillFrustrumX[4];
	/*<thisrel this+0x118c>*/ /*|0x10|*/ int fSkillFrustrumY[4];
	/*<thisrel this+0x119c>*/ /*|0x1|*/ unsigned char SkillHellFire2State;
	/*<thisrel this+0x119d>*/ /*|0x1|*/ unsigned char SkillHellFire2Count;
	/*<thisrel this+0x11a0>*/ /*|0x4|*/ unsigned long SkillHellFire2Time;
	/*<thisrel this+0x11a4>*/ /*|0x1|*/ unsigned char m_ReqWarehouseOpen;
	/*<thisrel this+0x11a8>*/ /*|0x4|*/ int m_NotAttackAreaCount;
	/*<thisrel this+0x11ac>*/ /*|0x2|*/ short SetOpAddSkillAttack;
	/*<thisrel this+0x11ae>*/ /*|0x2|*/ short SetOpAddExDamage;
	/*<thisrel this+0x11b0>*/ /*|0x2|*/ short SetOpAddExDamageSuccessRate;
	/*<thisrel this+0x11b2>*/ /*|0x2|*/ short SetOpAddCriticalDamage;
	/*<thisrel this+0x11b4>*/ /*|0x2|*/ short SetOpAddCriticalDamageSuccessRate;
	/*<thisrel this+0x11b6>*/ /*|0x2|*/ short SetOpIncAGValue;
	/*<thisrel this+0x11b8>*/ /*|0x2|*/ short SetOpAddDamage;
	/*<thisrel this+0x11ba>*/ /*|0x2|*/ short SetOpAddMinAttackDamage;
	/*<thisrel this+0x11bc>*/ /*|0x2|*/ short SetOpAddMaxAttackDamage;
	/*<thisrel this+0x11be>*/ /*|0x2|*/ short SetOpAddAttackDamage;
	/*<thisrel this+0x11c0>*/ /*|0x2|*/ short SetOpAddDefence;
	/*<thisrel this+0x11c2>*/ /*|0x2|*/ short SetOpAddDefenceRate;
	/*<thisrel this+0x11c4>*/ /*|0x2|*/ short SetOpAddMagicPower;
	/*<thisrel this+0x11c6>*/ /*|0x1|*/ unsigned char SetOpIgnoreDefense;
	/*<thisrel this+0x11c7>*/ /*|0x1|*/ unsigned char SetOpDoubleDamage;
	/*<thisrel this+0x11c8>*/ /*|0x1|*/ unsigned char SetOpTwoHandSwordImproveDamage;
	/*<thisrel this+0x11c9>*/ /*|0x1|*/ unsigned char SetOpImproveSuccessAttackRate;
	/*<thisrel this+0x11ca>*/ /*|0x1|*/ unsigned char SetOpReflectionDamage;
	/*<thisrel this+0x11cb>*/ /*|0x1|*/ unsigned char SetOpImproveSheldDefence;
	/*<thisrel this+0x11cc>*/ /*|0x1|*/ unsigned char SetOpDecreaseAG;
	/*<thisrel this+0x11cd>*/ /*|0x1|*/ unsigned char SetOpImproveItemDropRate;
	/*<thisrel this+0x11ce>*/ /*|0x1|*/ unsigned char IsFullSetItem;
	/*<thisrel this+0x11d0>*/ /*|0x12c|*/ class TDurMagicKeyChecker DurMagicKeyChecker;
	/*<thisrel this+0x12fc>*/ /*|0x2|*/ unsigned short SkillRecallParty_Time;
	/*<thisrel this+0x12fe>*/ /*|0x1|*/ unsigned char SkillRecallParty_MapNumber;
	/*<thisrel this+0x12ff>*/ /*|0x1|*/ unsigned char SkillRecallParty_X;
	/*<thisrel this+0x1300>*/ /*|0x1|*/ unsigned char SkillRecallParty_Y;
	/*<thisrel this+0x1301>*/ /*|0x1|*/ unsigned char bIsChaosMixCompleted;
	/*<thisrel this+0x1302>*/ /*|0x1|*/ unsigned char SkillLongSpearChange;
	/*<thisrel this+0x1304>*/ /*|0x964|*/ class CSkillDelay SkillDelay;
	/*<thisrel this+0x1c68>*/ /*|0x4|*/ int iObjectSecTimer;
	/*<thisrel this+0x1c6c>*/ /*|0x1|*/ unsigned char m_bMapSvrMoveQuit;
	/*<thisrel this+0x1c6d>*/ /*|0x1|*/ unsigned char m_bMapSvrMoveReq;
	/*<thisrel this+0x1c70>*/ /*|0x4|*/ unsigned long m_dwMapSvrQuitTick;
	/*<thisrel this+0x1c74>*/ /*|0x2|*/ short m_sPrevMapSvrCode;
	/*<thisrel this+0x1c76>*/ /*|0x2|*/ short m_sDestMapNumber;
	/*<thisrel this+0x1c78>*/ /*|0x1|*/ unsigned char m_btDestX;
	/*<thisrel this+0x1c79>*/ /*|0x1|*/ unsigned char m_btDestY;
	/*<thisrel this+0x1c7a>*/ /*|0x1|*/ unsigned char m_bIsCastleNPCUpgradeCompleted;
	/*<thisrel this+0x1c7b>*/ /*|0x1|*/ unsigned char m_btWeaponState;
	/*<thisrel this+0x1c7c>*/ /*|0x4|*/ int m_iWeaponUser;
	/*<thisrel this+0x1c80>*/ /*|0x1|*/ unsigned char m_btKillCount;
	/*<thisrel this+0x1c84>*/ /*|0x4|*/ int m_iAccumulatedDamage;
	/*<thisrel this+0x1c88>*/ /*|0x4|*/ int m_iAccumulatedCrownAccessTime;
	/*<thisrel this+0x1c8c>*/ /*|0x50|*/ class TMonsterSkillElementInfo m_MonsterSkillElementInfo;
	/*<thisrel this+0x1cdc>*/ /*|0x4|*/ int m_Ukn_1cdc;
	/*<thisrel this+0x1ce0>*/ /*|0x4|*/ int m_iBasicAI;
	/*<thisrel this+0x1ce4>*/ /*|0x4|*/ int m_iCurrentAI;
	/*<thisrel this+0x1ce8>*/ /*|0x4|*/ int m_iCurrentAIState;
	/*<thisrel this+0x1cec>*/ /*|0x4|*/ int m_iLastAIRunTime;
	/*<thisrel this+0x1cf0>*/ /*|0x4|*/ int m_iGroupNumber;
	/*<thisrel this+0x1cf4>*/ /*|0x4|*/ int m_iSubGroupNumber;
	/*<thisrel this+0x1cf8>*/ /*|0x4|*/ int m_iGroupMemberGuid;
	/*<thisrel this+0x1cfc>*/ /*|0x4|*/ int m_iRegenType;
	/*<thisrel this+0x1d00>*/ /*|0x324|*/ class TMonsterAIAgro m_Agro;
	/*<thisrel this+0x2024>*/ /*|0x4|*/ int m_iLastAutomataRuntime;
	/*<thisrel this+0x2028>*/ /*|0x4|*/ int m_iLastAutomataDelay;
	/*<thisrel this+0x202c>*/ /*|0x4|*/ int m_iCrywolfMVPScore;
	/*<thisrel this+0x2030>*/ /*|0x4|*/ int m_iPCRoom;
	/*<thisrel this+0x2034>*/ /*|0x4|*/ unsigned long m_dwLastCheckTick;
	/*<thisrel this+0x2038>*/ /*|0x4|*/ int m_iAutoRecuperationTime;
	/*<thisrel this+0x203c>*/ /*|0x4|*/ int m_iSkillDistanceErrorCount;
	/*<thisrel this+0x2040>*/ /*|0x4|*/ unsigned long m_dwSkillDistanceErrorTick;
	/*<thisrel this+0x2044>*/ /*|0x26|*/ struct JEWELOFHARMONY_ITEM_EFFECT m_JewelOfHarmonyEffect;
	/*<thisrel this+0x206a>*/ /*|0x10|*/ struct ITEMOPTION_FOR380ITEM_EFFECT m_ItemOptionExFor380;
	/*<thisrel this+0x207c>*/ /*|0x4|*/ int m_bKanturuEntranceByNPC;
	/*<thisrel this+0x2080>*/ /*|0x2|*/ unsigned short m_wItemEffectType;
	/*<thisrel this+0x2084>*/ /*|0x4|*/ int m_iItemEffectValidTime;
	/*<thisrel this+0x2088>*/ /*|0x2|*/ unsigned short m_wCashPoint;
	/*<thisrel this+0x208C>*/ /*|0x4|*/ int m_bIsPcBangPoint;	  
	/*<thisrel this+0x2090>*/ /*|0x2|*/ WORD m_wExprienceRate;
	/*<thisrel this+0x2092>*/ /*|0x2|*/ WORD m_wItemDropRate;
};

struct _GUILD_INFO_STRUCT
{
	/*<thisrel this+0x0>*/ /*|0x4|*/ int Number;	//0
	/*<thisrel this+0x4>*/ /*|0x9|*/ char Name[9];	//4
	/*<thisrel this+0xd>*/ /*|0x20|*/ unsigned char Mark[32];	//D
	/*<thisrel this+0x2d>*/ /*|0x1|*/ BYTE Count;	//2D
	/*<thisrel this+0x2e>*/ /*|0x1|*/ BYTE TotalCount;	//2E
	/*<thisrel this+0x2f>*/ /*|0x371|*/ char Names[20][11];	//2F
	/*<thisrel this+0x3a0>*/ /*|0xa0|*/ short Index[20];	//3A0
	/*<thisrel this+0x440>*/ /*|0x50|*/ unsigned char Use[20];	//440
	/*<thisrel this+0x490>*/ /*|0x50|*/ char pServer[20];	//490
	/*<thisrel this+0x4e0>*/ /*|0xa|*/ char TargetGuildName[9];	//4E0
	/*<thisrel this+0x4ea>*/ /*|0xa2|*/ short TargetIndex[20];	//4EA
	/*<thisrel this+0x58c>*/ /*|0x4|*/ struct _GUILD_INFO_STRUCT* lpTargetGuildNode;	//58C
	/*<thisrel this+0x590>*/ /*|0x1|*/ unsigned char WarDeclareState;	//590
	/*<thisrel this+0x591>*/ /*|0x1|*/ unsigned char WarState;	//591
	/*<thisrel this+0x592>*/ /*|0x1|*/ unsigned char WarType;	//592
	/*<thisrel this+0x593>*/ /*|0x1|*/ unsigned char BattleGroundIndex;	//593
	/*<thisrel this+0x594>*/ /*|0x1|*/ unsigned char BattleTeamCode;	//594
	/*<thisrel this+0x595>*/ /*|0x3|*/ unsigned char PlayScore;	//595
	/*<thisrel this+0x598>*/ /*|0x4|*/ int TotalScore;	//598
	/*<thisrel this+0x59c>*/ /*|0x3c|*/ char Notice[60];	//59c
	/*<thisrel this+0x5d8>*/ /*|0x140|*/ int GuildStatus[20];	//5D8
	/*<thisrel this+0x718>*/ /*|0x4|*/ BYTE btGuildType;	//718
	/*<thisrel this+0x71c>*/ /*|0x4|*/ int iGuildUnion;	//71C
	/*<thisrel this+0x720>*/ /*|0x4|*/ int iGuildRival;	//720
	/*<thisrel this+0x724>*/ /*|0x4|*/ int iTimeStamp;	//724
	/*<thisrel this+0x728>*/ /*|0xc|*/ char szGuildRivalName[9];	//728
	/*<thisrel this+0x734>*/ /*|0x4|*/ struct _GUILD_INFO_STRUCT* back;	//734
	/*<thisrel this+0x738>*/ /*|0x4|*/ struct _GUILD_INFO_STRUCT* next;	//738



	// #error Deathay Fix her
	_GUILD_INFO_STRUCT(){return;};
	int  GetGuildUnion(){return this->iGuildUnion;};
	int  GetGuildRival(){return this->iGuildRival;};


	void SetGuildUnion(int iGuildNumber)	// line : 102
	{
		this->iGuildUnion = iGuildNumber;
		this->SetTimeStamp();
	};	// line : 105

	void SetGuildRival(int iGuildNumber)	// line : 108
	{
		this->iGuildRival = iGuildNumber;
		this->SetTimeStamp();
	};	// line : 111

	void SetTimeStamp()	// line : 117
	{
		this->iTimeStamp++;
	};	// line : 119

	int GetTimeStamp()	// line : 122
	{
		return this->iTimeStamp;
	};	// line : 124

	BOOL CheckTimeStamp(int iTime)	// line : 127
	{
		return (iTime == this->iTimeStamp )? TRUE : FALSE;
	}	// line : 129
};

enum PLAYER_WARDROBE
{  
	SLOT_EMPTY  = -1,
	WEAPON_01	= 0,
	WEAPON_02	= 1,
	HELMET		= 2,
	ARMOR		= 3,
	PANTS		= 4,
	GLOVES		= 5,
	BOOTS		= 6,
	WINGS		= 7,
	GUARDIAN	= 8,
	PENDANT		= 9,
	RING_01		= 10,
	RING_02		= 11,
};

class CItem 
{
public:
	/*<thisrel this+0x0>*/ /*|0x4|*/ unsigned long m_Number;
	/*<thisrel this+0x4>*/ /*|0x1|*/ char m_serial;
	/*<thisrel this+0x6>*/ /*|0x2|*/ short m_Type;
	/*<thisrel this+0x8>*/ /*|0x2|*/ short m_Level;
	/*<thisrel this+0xa>*/ /*|0x1|*/ unsigned char m_Part;
	/*<thisrel this+0xb>*/ /*|0x1|*/ unsigned char m_Class;
	/*<thisrel this+0xc>*/ /*|0x1|*/ unsigned char m_TwoHand;
	/*<thisrel this+0xd>*/ /*|0x1|*/ unsigned char m_AttackSpeed;
	/*<thisrel this+0xe>*/ /*|0x1|*/ unsigned char m_WalkSpeed;
	/*<thisrel this+0x10>*/ /*|0x2|*/ unsigned short m_DamageMin;
	/*<thisrel this+0x12>*/ /*|0x2|*/ unsigned short m_DamageMax;
	/*<thisrel this+0x14>*/ /*|0x1|*/ unsigned char m_SuccessfulBlocking;
	/*<thisrel this+0x16>*/ /*|0x2|*/ unsigned short m_Defense;
	/*<thisrel this+0x18>*/ /*|0x2|*/ unsigned short m_MagicDefense;
	/*<thisrel this+0x1a>*/ /*|0x1|*/ unsigned char m_Speed;
	/*<thisrel this+0x1c>*/ /*|0x2|*/ unsigned short m_DamageMinOrigin;
	/*<thisrel this+0x1e>*/ /*|0x2|*/ unsigned short m_DefenseOrigin;
	/*<thisrel this+0x20>*/ /*|0x2|*/ unsigned short m_Magic;
	/*<thisrel this+0x24>*/ /*|0x4|*/ float m_Durability;
	/*<thisrel this+0x28>*/ /*|0x2|*/ unsigned short m_DurabilitySmall;
	/*<thisrel this+0x2c>*/ /*|0x4|*/ float m_BaseDurability;
	/*<thisrel this+0x30>*/ /*|0x1|*/ unsigned char m_SpecialNum;
	/*<thisrel this+0x31>*/ /*|0x8|*/ unsigned char m_Special[8];
	/*<thisrel this+0x39>*/ /*|0x8|*/ unsigned char m_SpecialValue[8];
	/*<thisrel this+0x42>*/ /*|0x2|*/ unsigned short m_RequireStrength;
	/*<thisrel this+0x44>*/ /*|0x2|*/ unsigned short m_RequireDexterity;
	/*<thisrel this+0x46>*/ /*|0x2|*/ unsigned short m_RequireEnergy;
	/*<thisrel this+0x48>*/ /*|0x2|*/ unsigned short m_RequireLevel;
	/*<thisrel this+0x4a>*/ /*|0x2|*/ unsigned short m_RequireVitality;
	/*<thisrel this+0x4c>*/ /*|0x2|*/ unsigned short m_RequireLeaderShip;
	/*<thisrel this+0x4e>*/ /*|0x2|*/ unsigned short m_Leadership;
	/*<thisrel this+0x50>*/ /*|0x6|*/ unsigned char m_RequireClass[6];
	/*<thisrel this+0x56>*/ /*|0x7|*/ unsigned char m_Resistance[7];
	/*<thisrel this+0x60>*/ /*|0x4|*/ int m_Value;
	/*<thisrel this+0x64>*/ /*|0x4|*/ unsigned long m_SellMoney;
	/*<thisrel this+0x68>*/ /*|0x4|*/ unsigned long m_BuyMoney;
	/*<thisrel this+0x6c>*/ /*|0x4|*/ int m_iPShopValue;
	/*<thisrel this+0x70>*/ /*|0x1|*/ unsigned char m_bItemExist;
	/*<thisrel this+0x74>*/ /*|0x4|*/ int m_OldSellMoney;
	/*<thisrel this+0x78>*/ /*|0x4|*/ int m_OldBuyMoney;
	/*<thisrel this+0x7c>*/ /*|0x1|*/ unsigned char m_Option1;
	/*<thisrel this+0x7d>*/ /*|0x1|*/ unsigned char m_Option2;
	/*<thisrel this+0x7e>*/ /*|0x1|*/ unsigned char m_Option3;
	/*<thisrel this+0x7f>*/ /*|0x1|*/ unsigned char m_NewOption;
	/*<thisrel this+0x80>*/ /*|0x10|*/ float m_DurabilityState[4];
	/*<thisrel this+0x90>*/ /*|0x4|*/ float m_CurrentDurabilityState;
	/*<thisrel this+0x94>*/ /*|0x1|*/ unsigned char m_SkillChange;
	/*<thisrel this+0x95>*/ /*|0x1|*/ unsigned char m_QuestItem;
	/*<thisrel this+0x96>*/ /*|0x1|*/ unsigned char m_SetOption;
	/*<thisrel this+0x97>*/ /*|0x1|*/ unsigned char m_SetAddStat;
	/*<thisrel this+0x98>*/ /*|0x1|*/ unsigned char m_IsValidItem;
	/*<thisrel this+0x99>*/ /*|0x7|*/ unsigned char m_SkillResistance[7];
	/*<thisrel this+0xa0>*/ /*|0x4|*/ int m_IsLoadPetItemInfo;
	/*<thisrel this+0xa4>*/ /*|0x4|*/ int m_PetItem_Level;
	/*<thisrel this+0xa8>*/ /*|0x4|*/ int m_PetItem_Exp;
	/*<thisrel this+0xac>*/ /*|0x1|*/ unsigned char m_JewelOfHarmonyOption;
	/*<thisrel this+0xae>*/ /*|0x2|*/ unsigned short m_HJOpStrength;
	/*<thisrel this+0xb0>*/ /*|0x2|*/ unsigned short m_HJOpDexterity;
	/*<thisrel this+0xb2>*/ /*|0x1|*/ unsigned char m_ItemOptionEx;
	/*<thisrel this+0xb3>*/ /*|0x1|*/ unsigned char m_OptionIndex;
	/*<thisrel this+0xb4>*/ /*|0x5|*/ unsigned char m_SocketOption[5];
	/*<thisrel this+0xb9>*/ /*|0x1|*/ unsigned char m_UknB9;
};
						
#endif