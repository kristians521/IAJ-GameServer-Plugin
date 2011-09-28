// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # GreatDevelop 1.0.x								# //
// ================================================== //

#include "StdAfx.h"
#include "Protocol.h"
#include "ChatCommands.h"
#include "User.h"
#include "IpBlock.h"
#include "DuelManager.h"
#include "GMSystem.h"
#include "Utilits.h"
#include "Prodef.h"
#include "PCPoint.h"
#include "Archer.h"
#include "MossGambler.h"
#include "Monster.h"
#include "MapSystem.h"
#include "Query.h"

cProtoFunc Protocol;

BYTE RecvTable[256] = {

		0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
		0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0xD7,0x1E,0x1F,
		0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
		0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
		0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
		0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
		0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
		0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,
		0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
		0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
		0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
		0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
		0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
		0xD0,0xD1,0xD2,0x1D,0xD4,0xD5,0xD6,0xDC,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xD6,
		0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,
		0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF 
};	

void gObjPlayerKiller(LPOBJ lpObj, LPOBJ lpTargetObj)
{												
	if(MapSystem.Maps[lpObj->MapNumber].PVP == 3 && lpTargetObj->Type == OBJECT_USER && lpObj->Type == OBJECT_USER)
		return;
	GCgObjPlayerKiller(lpObj, lpTargetObj);
}

BOOL gObjAttack(LPOBJ lpObj, LPOBJ lpTargetObj, class CMagicInf* lpMagic, int magicsend, unsigned char MSBFlag, int AttackDamage, BOOL bCombo)
{		
	if(MapSystem.Maps[lpObj->MapNumber].PVP == 1 && lpTargetObj->Type == OBJECT_USER && lpObj->Type == OBJECT_USER)
		return FALSE;
	return GCgObjAttack(lpObj, lpTargetObj, lpMagic, magicsend, MSBFlag, AttackDamage, bCombo);
}

bool ProtocolCore (BYTE protoNum, LPBYTE aRecv, DWORD aLen, int aIndex, DWORD Encrypt, int Serial)
{
	OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(aIndex);

	if(!Config.Unicode32)
	{
		if(protoNum == 0xF3)
		{	 													   
			PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;
			switch ( lpDef->subcode )
			{
			case 0x01:
				{		   
					if(Protocol.CharacterCreate((PMSG_CHARCREATE *)aRecv, aIndex))
					{
						JGCharacterCreateFailSend(aIndex, gObj->Name);
						return true;
					}
				}
			}
		}
	}
	switch(BYTE(protoNum))
	{
		case 0x00: // Player Connected Protocol
		{
			if(Chat.ChatDataSend(gObj, aRecv))
			return true;
		}
		break; 
		case 0x03: // Player Connected Protocol
		{
			Protocol.PlayerConnect(gObj);
		}
		break;

		case 0x24: // Enquip Guardian
		{
			Protocol.CheckRing(gObj, aRecv);
		}
		break;

		case 0xD0: // PC Points
		{
			if(aRecv[3] == 0x05)
			{
				PCPoint.BuyItem(aIndex, aRecv[4]);
				return true;
			}
			if(aRecv[3] == 0x06 && aRecv[1] == 0x04)
			{
				if(gObj->MapNumber == 0 || gObj->MapNumber == 2 || gObj->MapNumber == 3|| gObj->MapNumber == 51)
				{
					PCPoint.OpenShop(aIndex);
				}
				else
				{
					Chat.Message(aIndex,"[PointShop] Allow Only in Lorencia,Devias,Noria,Elbeland");
					BYTE pNewProtocol[0x05] = {0xC1, 0x05, 0x18, 0x01, 0x7A};
					DataRecv(RecvTable[pNewProtocol[2]], pNewProtocol, pNewProtocol[1], aIndex, Encrypt, Serial );
				}
				return true;
			}
		}
		break;
		if(Config.LahapDupe)
		{
			case 0xBC: // Lahap Dupe BUG FIX
			{ 
				Protocol.LahapDupeBug(gObj);
			}
			break;
		}
		case 0x30: // Click NPC Protocol	  
		{
			bool bResult = Protocol.NPCTalkEx(gObj, (aRecv[4] + aRecv[3] * 256));
			if(bResult) return true;
		}
		break;
		case 0x40:
		{							 
			if(Config.IsPartyGap)
				if(Protocol.CGPartyRequestRecv((PMSG_PARTYREQUEST*) aRecv,aIndex))
					return true;
		}
		break;
		case 0x55:
			{
				if(Protocol.GuildMasterInfoSave(aIndex, (PMSG_GUILDINFOSAVE *)aRecv))
					return true;
			}
			break;

		case 0xAA:	
			{  		
				if(Config.Duel.Enabled)
				{
#ifdef _GS 
					g_DuelSystem.DuelProtocolCore(gObj, aRecv);
					return true;
#else 
					GCServerMsgStringSend("You can't use duel on CS server!", aIndex, 1);	 		 
#endif	
				}
			}
		break;
#ifdef _GS
		case 0x32:
			{
				if (moss.MossConfig.EnableMoss)
				{
					if (moss.BuyItem(aIndex,aRecv) == TRUE)
					{
						moss.DataSendMoss(aIndex);
						return true;
					}
				}
			}
			break;	
#endif

		case 0xF1: // IP Connect Protocol
		{
			if (aRecv[3] == 0x03)
			{
				Log.ConsoleOutPut(1,c_Red,t_Error,"error-L1 : HackCheck [%s][%s] 0x0 0x1e",gObj->AccountID,gObj->Name);
				Chat.Message(gObj->m_Index,"[PC Point] Cannot be attack with opening shop. You are disconnect");
				CloseClient(gObj->m_Index);
			}
			if(aRecv[3] == 0x01)
			{
				if(!IpBlock.CheckIp(aIndex))return true;
			}				
		}
		break;
	}
	DataRecv(protoNum, aRecv, aLen, aIndex, Encrypt, Serial);
	return true;
}

void ProtocolCoreSend(int aIndex, PBYTE aSend, int aLen)
{
	OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(aIndex);

	if(aSend[0] == 0xC1 || aSend[0] == 0xC3)
	{
		switch(aSend[2])
		{
            case 0x24: 
            {   
				Protocol.CheckRingSend(gObj, aSend);
            }
			break; 
		}
	} 

	DataSend(aIndex,aSend,aLen);
}

void cProtoFunc::PlayerConnect(LPOBJ gObj)
{	
	LoginMsg(gObj);
	RingSkin(gObj);
	PCPoint.InitPCPointForPlayer(gObj); 

	Me_MuOnlineQuery.ExecQuery("SELECT cspoints FROM MEMB_INFO WHERE memb___id = '%s'", gObj->AccountID);
		Me_MuOnlineQuery.Fetch();
		gObj->m_wCashPoint = Me_MuOnlineQuery.GetAsInteger("cspoints");
		Me_MuOnlineQuery.Close();

	MuOnlineQuery.ExecQuery("SELECT %s FROM Character WHERE Name = '%s'", Config.ResetColumn, gObj->Name);
		MuOnlineQuery.Fetch();
		AddTab[gObj->m_Index].Resets = MuOnlineQuery.GetAsInteger(Config.ResetColumn);
		MuOnlineQuery.Close();

	AddTab[gObj->m_Index].ON_Min			= 0;   
	AddTab[gObj->m_Index].ON_Sek			= 0;
	AddTab[gObj->m_Index].ON_Hour			= 0;
	AddTab[gObj->m_Index].PC_OnlineTimer	= 0;

#ifdef _GS 
	if(Config.Duel.Enabled)
	{
		if(Config.Duel.Ranking)
		{
			g_DuelSystem.DuelSetInfo(gObj->m_Index);
		}
				
		if((!g_DuelSystem.IsOnDuel(gObj->m_Index)) && gObj->MapNumber == 64)
		{
			gObjMoveGate(gObj->m_Index, 294);
			Log.ConsoleOutPut(1, c_Blue ,t_Duel, "[Duel System][%s][%s] Spawn on duel map after duel is not allowed", gObj->AccountID, gObj->Name);
		}
		g_DuelSystem.UserDuelInfoReset(gObj);
	}
#endif
		if(Config.VIP.Enabled)
		{												
			MuOnlineQuery.ExecQuery("SELECT %s, %s FROM Character WHERE Name = '%s'", Config.VIP.Column, Config.VIP.ColumnDate, gObj->Name);
				MuOnlineQuery.Fetch();
				AddTab[gObj->m_Index].VIP_Type = MuOnlineQuery.GetAsInteger(Config.VIP.Column);
				AddTab[gObj->m_Index].VIP_Min = MuOnlineQuery.GetAsInteger(Config.VIP.ColumnDate);
				MuOnlineQuery.Close();

			AddTab[gObj->m_Index].VIP_Sec = 0; // ��������� ������ ��� �����
			if(AddTab[gObj->m_Index].VIP_Min > 0)
			{											 
				Chat.MessageLog(1, c_Red, t_VIP, gObj, "[VIP] Left %d minutes of VIP.", AddTab[gObj->m_Index].VIP_Min);
			} 
		}
}

void cProtoFunc::RingSkin(LPOBJ gObj)
{   
    if(gObj->pInventory[RING_01].m_Type == 0x1A4C && gObj->m_Change != 503 ||
        gObj->pInventory[RING_02].m_Type == 0x1A4C && gObj->m_Change != 503)
        {
            gObj->m_Change = 503;
            gObjViewportListProtocolCreate(gObj);
        }
}

void cProtoFunc::CheckRingSend(LPOBJ gObj, unsigned char* aSend)
{
    if(aSend[4] == RING_01 || aSend[4] == RING_02) 
        if(gObj->pInventory[RING_01].m_Type == 0x1A4C && gObj->m_Change != 503 ||
            gObj->pInventory[RING_02].m_Type == 0x1A4C && gObj->m_Change != 503)
		{
			gObj->m_Change = 503;
			gObjViewportListProtocolCreate(gObj);	       
		}
}

void cProtoFunc::CheckRing(LPOBJ gObj, LPBYTE aRecv)
{
	if((aRecv[4] == RING_01 && gObj->pInventory[RING_02].m_Type != 0x1A4C) 
		|| (aRecv[4] == RING_02 && gObj->pInventory[RING_01].m_Type != 0x1A4C))
		if(gObj->m_Change == 503)
		{
			gObj->m_Change = -1;	
			gObjViewportListProtocolCreate(gObj);	
		}	
}	

bool cProtoFunc::CGPartyRequestRecv(PMSG_PARTYREQUEST * lpMsg, int aIndex)
{	
	int number =  MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(aIndex);	 
	OBJECTSTRUCT *pObj = (OBJECTSTRUCT*)OBJECT_POINTER(number);
																			 
	if(gObj->Level > pObj->Level && gObj->Level - pObj->Level >= Config.PartyGapLvl)
	{	
		Chat.MessageLog(1, c_Red, t_Default, gObj, "[Party] You can't stay with %s in party! %s needs %d more lvl.", pObj->Name, pObj->Name, gObj->Level-Config.PartyGapLvl - pObj->Level);
		return true;
	}

	if(gObj->Level < pObj->Level && pObj->Level - gObj->Level >= Config.PartyGapLvl)
	{																													
		Chat.MessageLog(1, c_Red, t_Default, gObj, "[Party] You can't stay with %s in party! You need %d more lvl.", pObj->Name, pObj->Level - Config.PartyGapLvl - gObj->Level);
		return true;
	}	   
	return false;
}

void cProtoFunc::LahapDupeBug(LPOBJ gObj)
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

void cProtoFunc::LoginMsg(LPOBJ gObj)
{	
	Chat.Message(1, gObj->m_Index, "http://mu.greatgame.su/");
	Chat.Message(0, gObj->m_Index, Config.ConnectNotice);
	if (Config.ConnectInfo == 1)
	{
		Chat.Message(1, gObj->m_Index, "Total Online: %d/%d", Log.Online_All, Log.Online_Max);
																			  
		SYSTEMTIME t;
		GetLocalTime(&t);  
		Chat.Message(1, gObj->m_Index, "Server Time & Date: %02d:%02d:%02d %02d-%02d-%04d.", t.wHour, t.wMinute, t.wSecond, t.wDay, t.wMonth, t.wYear);
	} 

	switch(GmSystem.IsAdmin(gObj->Name))
	{																									   
	case 1:
		Chat.MessageAllLog(0, 0, c_Green, t_GM, gObj, "[Admin] %s join the game!", gObj->Name);
		break;
	case 2:
		Chat.MessageAllLog(0, 0, c_Green, t_GM, gObj, "[GM] %s join the game!", gObj->Name);
		break;
	}
}

bool cProtoFunc::NPCTalkEx(LPOBJ gObj, int NpcId)
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

void cProtoFunc::PkClear(LPOBJ gObj, LPOBJ NpcObj)
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

bool cProtoFunc::CharacterCreate(PMSG_CHARCREATE* lpMsg, int aIndex)
{
	bool bResult = false;
	for(int i = 0; i < sizeof(lpMsg->Name); i++)
	{
		if(!isalnum(lpMsg->Name[i]) && lpMsg->Name[i] != ' ' && lpMsg->Name[i] != NULL)
		{						
			bResult = true;
		}
	}
	return bResult;
}

bool cProtoFunc::GuildMasterInfoSave(int aIndex,PMSG_GUILDINFOSAVE* lpMsg)
{
	bool bResult = false;
	for(int i = 0; i < sizeof(lpMsg->GuildName); i++)
	{
		if(!isalnum(lpMsg->GuildName[i]) && lpMsg->GuildName[i] != ' ' && lpMsg->GuildName[i] != NULL)
		{						
			bResult = true;

		}
	}		
	if(bResult)
	{
		PMSG_GUILDCREATED_RESULT pMsg;

		PHeadSetB((LPBYTE)&pMsg, 0x56, sizeof(pMsg));
		pMsg.Result = 5;

		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}	   
	return bResult;
}