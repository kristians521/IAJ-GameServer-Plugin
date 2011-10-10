// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#include "StdAfx.h"
#include "Protocol.h"
#include "ChatCommands.h"
#include "User.h"
#include "DuelManager.h"
#include "PCPoint.h"
#include "MossGambler.h"
#include "Monster.h"
#include "MapSystem.h"

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
					if(User.CharacterCreate((PMSG_CHARCREATE *)aRecv, aIndex))
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
			User.PlayerConnect(gObj);
		}
		break;

		case 0x24: // Enquip Guardian
		{
			User.CheckRing(gObj, aRecv);
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
				Monster.LahapDupeBug(gObj);
			}
			break;
		}
		case 0x30: // Click NPC Protocol	  
		{
			bool bResult = Monster.NPCTalkEx(gObj, (aRecv[4] + aRecv[3] * 256));
			if(bResult) return true;
		}
		break;
		case 0x40:
		{							 
			if(Config.IsPartyGap)
				if(User.CGPartyRequestRecv((PMSG_PARTYREQUEST*) aRecv,aIndex))
					return true;
		}
		break;
		case 0x55:
			{
				if(User.GuildMasterInfoSave(aIndex, (PMSG_GUILDINFOSAVE *)aRecv))
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
				User.CheckRingSend(gObj, aSend);
            }
			break; 
		}
	} 

	DataSend(aIndex,aSend,aLen);
}