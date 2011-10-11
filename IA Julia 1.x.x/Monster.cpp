// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#include "StdAfx.h"
#include "Monster.h"
#include "User.h"
#include "DropSystem.h"
#include "Utilits.h"
#include "PCPoint.h"
#include "Configs.h"
#include "Logger.h"
#include "ChatCommands.h"
#include "MapSystem.h"
#include "DuelManager.h"
#include "MossGambler.h"
#include "Archer.h"

cMonster Monster;

void cMonster::LahapDupeBug(LPOBJ gObj)
{
	int Error = 0; 
	for(int i = OBJECT_MIN; i<OBJECT_MAX; i++) 
	{ 
		OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(i); 

		if((gObj->TargetNumber == i) && (gObj->pTransaction==1))
		{
			Error = 1;
			Chat.MessageLog(1, c_Red, t_Default, gObj, "[AntiHack][%s] Lahap Trade-Dupe Attempt, Trade: %s[%d], Action: %d",gObj->Name,gObj[i].Name,gObj->TargetNumber,gObj->pTransaction);
		}

		if(Error == 1) break; 
	} 
}

// Monster die Handler (hooked replacement for gObjMonsterDieGiveItem)
void __cdecl MonsterDie(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	PCPoint.RewardsPointsKillMob(lpTargetObj,lpObj,PCPOINT);
	PCPoint.RewardsPointsKillMob(lpTargetObj,lpObj,WCOIN);
	// Original function
	gObjMonsterDieGiveItem(lpObj, lpTargetObj);

	//MapSystem Module Drop
	if(MapSystem.Maps[lpTargetObj->MapNumber].Drop != 0)
	{
		lpObj->m_wItemDropRate += MapSystem.Maps[lpObj->MapNumber].Drop;
	}
}

#ifdef _GS
int cMonster::MonsterAddAndSpawn(WORD Monster,BYTE Speed,BYTE Map,BYTE X, BYTE Y)
{
	int MobCount = *(DWORD *)(MonsterCount);

	if((MobCount + 1) < MonsterCount)
	{
		MobCount++;
		*(DWORD *)(MonsterCount) = MobCount;
		*(WORD *)(12 * MobCount + MonsterReads) = Monster;
		*(BYTE *)(12 * MobCount + MonsterReads+2) = Map;
		*(BYTE *)(12 * MobCount + MonsterReads+3) = Speed;
		*(BYTE *)(12 * MobCount + MonsterReads+4) = X;
		*(BYTE *)(12 * MobCount + MonsterReads+5) = Y;
		*(BYTE *)(12 * MobCount + MonsterReads+6) = 2;
		*(BYTE *)(12 * MobCount + MonsterReads+7) = X;
		*(BYTE *)(12 * MobCount + MonsterReads+8) = Y;

		int MobID = gObjAddMonster(*(BYTE *)(12 * MobCount + (MonsterReads+2)));
		if(MobID>=0)
		{
			int MobNr = *(WORD *)(12 * MobCount + MonsterReads);
			gObjSetPosMonster(MobID, MobCount);
			gObjSetMonster(MobID, MobNr);
		}

		return MobID;
	}else
	{		
		MessageBox(NULL,"Monster attribute max over!!", "Monsters overflow", 0);
		return -1;
	}
}

void cMonster::ReadMonsterAdd()
{ 
		FILE* MonsterFile;
		if((MonsterFile = fopen( IAJuliaMobAdd, "r")) == NULL)
		{						   
			MessageBox(NULL, "Cant Find MonsterSpawn.ini", "Error", 0);
			return;
		}
								
		rewind(MonsterFile);
		char Buff[255];	
		while(!feof(MonsterFile))
		{							
			Buff[0] = 0;
			fgets (Buff, 255, MonsterFile);	 
			if(Buff[0] == '/' || Buff[0] == '#' || Buff[0] == ' ' || strlen(Buff) < 9)
				continue;			

			int Mob = -1, Cnt = -1, Map = -1, Speed = -1, X = -1, Y = -1;
			sscanf(Buff, "%d %d %d %d %d %d", &Mob, &Cnt, &Speed, &Map, &X, &Y);

			if(Mob == -1 || Cnt == -1 || Map == -1 || X == -1 || Y == -1)
				continue;
								  
			for(int i = 0; i < Cnt; i++)
				MonsterAddAndSpawn(Mob,Speed,Map,X,Y);
		}	 
		fclose(MonsterFile); 
}
#endif

//Monster Death Control
int MygEventMonsterItemDrop(BYTE *b_MonsterDataAddr,BYTE *a_gObjAddr)
{
	// Get mIndex, aIndex & Monster & Player Objects
	PBYTE a_aIndex = 0;
	PBYTE b_mIndex = 0;

	a_aIndex = (PBYTE)a_gObjAddr;
	b_mIndex = (PBYTE)b_MonsterDataAddr;

	WORD mIndex = 0;
	WORD aIndex = 0;

	memcpy(&mIndex, b_mIndex+0x00,sizeof(WORD));
	memcpy(&aIndex, a_aIndex+0x00,sizeof(WORD));

	OBJECTSTRUCT *mObj = (OBJECTSTRUCT*) OBJECT_POINTER (mIndex);
	OBJECTSTRUCT *pObj = (OBJECTSTRUCT*) OBJECT_POINTER (aIndex);

	
	int NewMoney = Utilits.gObjZenSingle(pObj,mObj,500,700);
	mObj->Money = (NewMoney/1000) * Config.Zen.NormalZen; 

	if(pObj->PartyNumber != -1)
	{    
			int Count = 0;
			for(int i=OBJECT_MIN;i<OBJECT_MAX;i++)
			{
				OBJECTSTRUCT *gObj = (OBJECTSTRUCT*) OBJECT_POINTER (i); 
				if(gObj->Connected == PLAYER_PLAYING && gObj->PartyNumber == pObj->PartyNumber)Count++;  
			} 
			//Count = GetPartyMemberCount(pObj); //To Psycho: Ты нашел не ту функцию, я нашел правильную все равно крошит. Пока будем юзать старый фикс.
			
			switch(Count)
			{ 
				case 1: // 2 Persons in Party
					mObj->Money = (mObj->Money * Config.Zen.ZenInParty) + (((mObj->Money * Config.Zen.ZenInParty) / 100) * 20);
				break;

				case 2: // 3 Persons in Party
					mObj->Money = (mObj->Money * Config.Zen.ZenInParty) + (((mObj->Money * Config.Zen.ZenInParty) / 100) * 25);
				break;

				case 3: // 4 Persons in Party
					mObj->Money = (mObj->Money * Config.Zen.ZenInParty) + (((mObj->Money * Config.Zen.ZenInParty) / 100) * 35);
				break;

				case 4: // 5 Persons in Party
					mObj->Money = (mObj->Money * Config.Zen.ZenInParty) + (((mObj->Money * Config.Zen.ZenInParty) / 100) * 40);
				break;
			}
	}
	
	//MapSystem Module Zen
	if(MapSystem.Enabled && MapSystem.Maps[pObj->MapNumber].Zen != 0)
	{
		mObj->Money += ((mObj->Money/ 100) * MapSystem.Maps[mObj->MapNumber].Zen);
	} 

	//VIP System 
	if(Config.VIP.Enabled && AddTab[pObj->m_Index].VIP_Type > 0)
	{
		int VIPInfo = AddTab[pObj->m_Index].VIP_Type;
		mObj->Money += ((mObj->Money/ 100) * Config.VIP.VIPState[VIPInfo].BonusZen);
	} 

	// Drop System
	if(DropSystem.DropItem(mObj,pObj))
	{
		return 1;
	}
	else // Default drop
	{
		int rValue = gEventMonsterItemDrop(b_MonsterDataAddr, a_gObjAddr);
		return rValue;
	}

	return 1;
}
 
char Messages1[1024];

void cMonster::NPCMessage(int IndexPlayer, LPOBJ mObj, char* Msg,...)
{						 
	Messages1[0] = 0;
	va_list pArguments1;
	va_start(pArguments1, Msg);
	vsprintf_s(Messages1, Msg, pArguments1);
	va_end(pArguments1);

	ChatTargetSend(mObj, Messages1, IndexPlayer);
}

void cMonster::NPCMessageLog(sColor LogColor, sLogType LogType, LPOBJ gObj, LPOBJ mObj, char* Msg,...)
{						  
	Messages1[0] = 0;
	va_list pArguments1;
	va_start(pArguments1, Msg);
	vsprintf_s(Messages1, Msg, pArguments1);
	va_end(pArguments1);

	ChatTargetSend(mObj, Messages1, gObj->m_Index); 
	Log.ConsoleOutPut(1, LogColor, LogType, "[%s]: %s", gObj->Name, Messages1);
}

void cMonster::NPCMessageNear(LPOBJ mObj, char* Msg,...)
{						   
	Messages1[0] = 0;
	va_list pArguments1;
	va_start(pArguments1, Msg);
	vsprintf_s(Messages1, Msg, pArguments1);
	va_end(pArguments1);

	for(int i = OBJECT_MIN; i <= OBJECT_MAX; i++)
	{						   
		OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(i);	   
		if(gObj->Connected < 3) continue; 	   
		if(mObj->MapNumber != gObj->MapNumber) continue;	 		 
		if(mObj->X <= gObj->X-10 || mObj->X >= gObj->X+10) continue;
		if(mObj->Y <= gObj->Y-10 || mObj->Y >= gObj->Y+10) continue;
		ChatTargetSend(mObj, Messages1, i);
	}
}


bool cMonster::NPCTalkEx(LPOBJ gObj, int NpcId)
{
	bool bResult = false;
	OBJECTSTRUCT *gObjNPC = (OBJECTSTRUCT*)OBJECT_POINTER(NpcId);
#ifdef _GS
	if (gObjNPC->Class == 479 && Config.Duel.Enabled)
	{
		PMSG_SEND_WINDOW aSend;
		// ----
		aSend.uHead			 = 0xC3;
		aSend.uSize			 = 0x04;
		aSend.uPacketType	 = 0x30;
		aSend.uNum			 = 0x21;
		gObj->m_IfState.use  = 479;
		gObj->m_IfState.type = 20;
		// ----
		DataSend(gObj->m_Index, (BYTE*)&aSend, 4);
		g_DuelSystem.SendDuelStatus(gObj);

		bResult = true;
	}
	if (gObjNPC->Class == 492 && moss.MossConfig.EnableMoss)
	{
		if (gObj->m_PK_Level > 3 && moss.MossConfig.UsePK == 0)
		{
			Chat.Message(gObj->m_Index,"[Moss The Gambler] PK player don`t use Moss The Gambler");
			return false;
		}
		if (moss.GetStatusMoss() == FALSE)
		{
			Chat.Message(gObj->m_Index,"[Moss The Gambler] Moss is closed");
			return false;
		}
		BYTE Send2[6] = {0xC3,0x06,0x30,0x00,0x27,0x00};
		BYTE Send[71] = {0xC2,0x00,71,0x31,0x00,5,0x00,71,0x00,0x01,0x00,0x00,13*16,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0x02,72,0x00,0x01,0x00,0x00,13*16,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0x04,73,0x00,0x01,0x00,0x00,13*16,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0x18,74,0x00,0x01,0x00,0x00,13*16,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0x06,75,0x00,0x01,0x00,0x00,13*16,0x00,0xFF,0xFF,0xFF,0xFF,0xFF};
		DataSend(gObj->m_Index,Send2,6);
		DataSend(gObj->m_Index,Send,71);
		gObj->TargetShopNumber = 492;
		gObj->m_IfState.use    = 1;
		gObj->m_IfState.type   = 3;
		bResult = true;
	}
	if ((gObjNPC->Class == Config.ClearNpc.NpcId) && (Config.ClearNpc.Enabled))
	{
		PkClear(gObj, gObjNPC);
		bResult = true;		
	}
	if(gObjNPC->Class == 236 && Config.Archer.Enabled)
	{
		GoldenArcher.GoldenArcherClick(gObj);
		bResult = true;
	}
	if (gObjNPC->Class == 241)
	{
		if( AddTab[gObj->m_Index].Resets < Config.GuildRes)
		{
			Chat.Message(1,gObj->m_Index,"You don't have enough Resets, you need %d more resets.", Config.GuildRes - AddTab[gObj->m_Index].Resets);
			bResult = true;
		}
		if( gObj->Level < Config.GuildLevel)
		{
			Chat.Message(1,gObj->m_Index,"You don't have enough Level, you need %d more Level.", Config.GuildLevel - gObj->Level);
			bResult = true;
		}
	} 
#endif
	return bResult;
}

void cMonster::PkClear(LPOBJ gObj, LPOBJ NpcObj)
{							   			
	if (gObj->m_PK_Level < 4)
	{										 
		NPCMessageLog( c_Blue ,t_COMMANDS, gObj, NpcObj, "You are good player. God bless your soul.");
		return;
	}	

	int PriceZen, PricePcPoint, PriceWCoin;
	switch(Config.ClearNpc.Type)
	{	   
	case 1:	
		PriceZen = (Config.ClearNpc.PriceZen * gObj->m_PK_Count); 		 
		PricePcPoint = (Config.ClearNpc.PricePcPoints * gObj->m_PK_Count);
		PriceWCoin = (Config.ClearNpc.PriceWCoins * gObj->m_PK_Count);
		break;
	case 2:	
		PriceZen = Config.ClearNpc.PriceZenForAll;			 
		PricePcPoint = Config.ClearNpc.PricePcPoints;
		PriceWCoin = Config.ClearNpc.PriceWCoins;
		break;
	case 0: 
		PriceZen = 0;					 
		PricePcPoint = 0;
		PriceWCoin = 0;
		break;
	}

	if(gObj->Money < PriceZen)
	{
		NPCMessageLog( c_Blue ,t_COMMANDS, gObj, NpcObj, "You don't have enough Zen, you need %d more!", PriceZen - gObj->Money);
		return;
	}	   
	if(gObj->m_wCashPoint < PriceWCoin)
	{	 
		NPCMessageLog( c_Blue ,t_COMMANDS, gObj, NpcObj, "You don't have enough WCoin, you need %d more!", PriceWCoin - gObj->m_wCashPoint);
		return;
	}
	if(AddTab[gObj->m_Index].PC_PlayerPoints < PricePcPoint)
	{	 
		NPCMessageLog( c_Blue ,t_COMMANDS, gObj, NpcObj, "You don't have enough PcPoint, you need %d more!", PricePcPoint - AddTab[gObj->m_Index].PC_PlayerPoints);
		return;
	}
	if (PricePcPoint > 0)
	{
		PCPoint.UpdatePoints(gObj,PricePcPoint,MINUS,PCPOINT);
		Chat.MessageLog(1, c_Blue, t_PCPOINT, gObj,"[Guard] You pay %d PcPoints", PricePcPoint);
	}

	if (PriceWCoin > 0)
	{										
		PCPoint.UpdatePoints(gObj,PriceWCoin,MINUS,WCOIN);
		Chat.MessageLog(1, c_Blue, t_PCPOINT, gObj,"[Guard] You pay %d WCoin", PriceWCoin);
	}

	if (PriceZen > 0)
	{															 
		gObj->Money -= PriceZen; 
		GCMoneySend (gObj->m_Index, gObj->Money);
		Chat.MessageLog(1, c_Blue ,t_PCPOINT, gObj, "[Guard] You pay %d Zen", PriceZen);
	}

	NPCMessageLog( c_Blue ,t_COMMANDS, gObj, NpcObj,"Cleaned %d kills. Don't tell anyone!", gObj->m_PK_Count); 

	gObj->m_PK_Level = 3;
	gObj->m_PK_Count = 0;

	GCPkLevelSend (gObj->m_Index,3);
}