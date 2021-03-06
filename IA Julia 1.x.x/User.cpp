// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#include "StdAfx.h"
#include "User.h" 
#include "Configs.h"
#include "GMSystem.h"
#include "ChatCommands.h"
#include "Logger.h"
#include "MapSystem.h"
#include "PCPoint.h"
#include "MossGambler.h"
#include "Query.h"
#include "DuelManager.h"
#include "Vip.h"
#include "HappyHour.h"
#include "PlayerSystem.h"
sAddTab AddTab[OBJECT_MAX]; 
cUser User;

bool cUser::CheckMaxPoints(BYTE type, OBJECTSTRUCT* lpObj)
{			 
	bool bResult = false;

	int MaxPoints = 32000;
	if(Configs.Enable65kStats >= 1)
		MaxPoints = -536;	

	int Stats;
	switch (type)
	{
	case 0x00:
		Stats = lpObj->Strength;   
		break;
	case 0x01:
		Stats = lpObj->Dexterity;   
		break;
	case 0x02:	  
		Stats = lpObj->Vitality; 
		break;	  
	case 0x03:
		Stats = lpObj->Energy;	 			 
		break;
	case 0x04:
		Stats = lpObj->Leadership;	 
		MaxPoints = 32000;
		break;
	}

	if(MaxPoints > 32767)
		MaxPoints = -32768 + (MaxPoints - 32767);

	if((MaxPoints > 0) && (Stats >= MaxPoints || Stats < 0))
		bResult = true;								 
	else if ((MaxPoints < 0) && (Stats < 0) && (Stats >= MaxPoints))
		bResult = true;

	return bResult;
}

void gObjLevelUpPointAddEx(BYTE type, OBJECTSTRUCT* lpObj)
{
	bool Error = false;

	if(User.CheckMaxPoints(type, lpObj))
		Error = true;

	if (Error == true)
	{
		BYTE cShowAddPointError[0x30] ={0xC1,0x30,0x0D,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 }  ;
		sprintf((char *)&cShowAddPointError[0x0D], "Error in Stats adding"); 
	}
	else
		gObjLevelUpPointAdd(type, lpObj);
}

void cUser::gObjCharacterWingsSetPreview(short ItemType, unsigned char *CharSet,int Type,OBJECTSTRUCT *lpObj) 
{	
	if(Type == GUARDIAN)
	{
		register unsigned char PreviewPet = 0;

		switch(ItemType)
		{
		case 0x1A50: //Panda
			PreviewPet = 224;
			break;

		case 0x1A43:
			PreviewPet = 0x80;
			break;

		case 0x1A41:
			PreviewPet = 0x40;
			break;

		case 0x1A40:
			PreviewPet = 0x20;
			break;
		
		case 0x1A7B: //Skeleton
			CharSet[5] -= 3;
			PreviewPet = 96; //112
			break; 

		default:
			PreviewPet = 0;
			break;
		}
		
		CharSet[16] |= PreviewPet;
	}
}
		   
void __stdcall gObjViewportPatchExecute(OBJECTSTRUCT *lpObj) {

	if(lpObj->pInventory[GUARDIAN].m_Type != SLOT_EMPTY)
		User.gObjCharacterWingsSetPreview(lpObj->pInventory[GUARDIAN].m_Type, lpObj->CharSet, GUARDIAN,lpObj);
}
						  
#define CS_SET_BOOTS1(x) ( ((x) & 0x1E0) >> 5 )
#define CS_SET_BOOTS2(x) ( ((x) & 0x10 ) >> 1 )
#define CS_SET_BOOTS3(x) ( ((x) & 0x0F ) << 4 )

//Fix Visual in Select Character
void __stdcall JGPGetCharInfoEx(int Loop, unsigned char *DSPacket, unsigned char *toClientPacket) {
	unsigned char ItemType = DSPacket[76 + (66 * Loop)]; //72 - Wings, 76 - Guardian
	register int nCol = (Loop * 34) + 38;
	register int CharSet5 = (Loop * 34) + 27; //CharSet[5]
	register int CharSet9 = (Loop * 34) + 31; //CharSet[9]
	register int CharSet10 = (Loop * 34) + 32; //CharSet[10]
	register int CharSet15 = (Loop * 34) + 37; //CharSet[15]
	register int CharSet17 = (Loop * 34) + 39; //CharSet[17]

	unsigned char Boots = DSPacket[68 + (66 * Loop)]; //Boots on Char
	unsigned char BootsLvl = DSPacket[69 + (66 * Loop)]; //Boots Level

	switch(ItemType)
	{
	case 0x50:
		toClientPacket[nCol] |= 224;
		break;
	case 0x7B:
		toClientPacket[CharSet5]  -= 3;
		toClientPacket[nCol] |= 96;

		break;
	}

	if(Boots == 255)
	{
		toClientPacket[CharSet5] |= CS_SET_BOOTS3(-1);
	}
}		

void GCKillPlayerExpSendHook(int aIndex, int TargetIndex, int exp, int AttackDamage, BOOL MSBFlag)
{   
	// -----
	// -----
    OBJECTSTRUCT * lpObj = (OBJECTSTRUCT*) OBJECT_POINTER (aIndex);
	unsigned int Proc = 0;
	unsigned int ProcML = 0;
	// ----
	if(lpObj->pInventory[8].m_Type == 0x1A50) // Panda
	{          
		Proc	+= Configs.Panda.PetPandaExpirence;
		ProcML	+= Configs.Panda.PetPandaMLExpirence;
	}

	if(lpObj->pInventory[10].m_Type == 0x1A4C || lpObj->pInventory[11].m_Type == 0x1A4C) // Panda Ring
	{          
		Proc	+= Configs.Panda.PandaRingExpirence;
		ProcML	+= Configs.Panda.PandaRingMLExpirence;
    }

	//VIPSystem	Exp
	if(Vip.Config.Enabled && AddTab[lpObj->m_Index].VIP_Type > 0)
	{
		int VIPInfo = AddTab[lpObj->m_Index].VIP_Type;
		Proc	+= Vip.Config.VIPState[VIPInfo].BonusExp;
		ProcML	+= Vip.Config.VIPState[VIPInfo].BonusExp;
	}

	//MapSystem Exp
	if(MapSystem.Enabled && MapSystem.Maps[lpObj->MapNumber].Exp != 0)
	{
		Proc	+= MapSystem.Maps[lpObj->MapNumber].Exp;
		ProcML	+= MapSystem.Maps[lpObj->MapNumber].Exp;
	}

	//HappyHour Exp
	int IsHappyHour = HappyHour.IsHappyHour(lpObj->MapNumber);
	if(IsHappyHour)
	{
		Proc	+= HappyHour.HappyStruct[IsHappyHour].P_Exp;
		ProcML	+= HappyHour.HappyStruct[IsHappyHour].P_Exp;
	}
	
	//Player System
	if(PlayerSystem.Enabled)
	{
		int bonus = PlayerSystem.GetBonus(lpObj, PlayerSystem.Exp);
		Proc	+= bonus;
		ProcML	+= bonus;
	}
	// ----
	unsigned int NewExp		= exp + ((exp * Proc) / 100);
	unsigned int NewExpML	= exp + ((exp * ProcML) / 100);
	lpObj->Experience		+= NewExp;
	lpObj->MLExp			+= NewExpML;

	while(NewExp > 65535)
	{
		GCKillPlayerExpSend(aIndex , TargetIndex , 65535 , AttackDamage , MSBFlag);
		NewExp -= 65535;
	}
    GCKillPlayerExpSend(aIndex , TargetIndex , NewExp , AttackDamage , MSBFlag);
}

void MyObjCalCharacter(int aIndex)
{
     OBJECTSTRUCT * lpObj = (OBJECTSTRUCT*) OBJECT_POINTER (aIndex);
     // -----
     gObjCalCharacter(aIndex);
     // -----
     if(lpObj->pInventory[8].m_Type == 0x1A50) //Panda
     {
          lpObj->m_Defense                  += Configs.Panda.PetPandaDefense;
          lpObj->m_AttackDamageMinLeft		+= Configs.Panda.PetPandaAttackDamageMinLeft;
          lpObj->m_AttackDamageMaxLeft		+= Configs.Panda.PetPandaAttackDamageMaxLeft;
          lpObj->m_AttackDamageMinRight     += Configs.Panda.PetPandaAttackDamageMinRight;
          lpObj->m_AttackDamageMaxRight     += Configs.Panda.PetPandaAttackDamageMaxRight;
          lpObj->m_MagicDamageMin           += Configs.Panda.PetPandaMagicDamageMin;
          lpObj->m_MagicDamageMax           += Configs.Panda.PetPandaMagicDamageMax;
          lpObj->m_MagicSpeed               += Configs.Panda.PetPandaMagicSpeed;
          lpObj->m_AttackSpeed              += Configs.Panda.PetPandaAttackSpeed;
     }
   if(lpObj->pInventory[10].m_Type == 0x1A4C || lpObj->pInventory[11].m_Type == 0x1A4C) // Panda Ring
    {
         lpObj->m_Defense                   += Configs.Panda.PandaRingDefense;
         lpObj->m_AttackDamageMinLeft		+= Configs.Panda.PandaRingAttackDamageMinLeft;
         lpObj->m_AttackDamageMaxLeft		+= Configs.Panda.PandaRingAttackDamageMaxLeft;
         lpObj->m_AttackDamageMinRight		+= Configs.Panda.PandaRingAttackDamageMinRight;
         lpObj->m_AttackDamageMaxRight		+= Configs.Panda.PandaRingAttackDamageMaxRight;
         lpObj->m_MagicDamageMin            += Configs.Panda.PandaRingMagicDamageMin;
         lpObj->m_MagicDamageMax            += Configs.Panda.PandaRingMagicDamageMax;
         lpObj->m_MagicSpeed                += Configs.Panda.PandaRingMagicSpeed;
         lpObj->m_AttackSpeed               += Configs.Panda.PandaRingAttackSpeed;	
    }
}
//Wait 2 miliseconds for update
void TradeSystem__Cancel(void * lpParam)
{	
	Sleep(200);
	for(int i=OBJECT_MIN;i<OBJECT_MAX;i++)
	{									  
		OBJECTSTRUCT * gObj = (OBJECTSTRUCT*)OBJECT_POINTER(i);
		if(gObj->Connected == PLAYER_PLAYING)
		{
			if(gObj->pInventory[RING_01].m_Type == 0x1A7A 
				|| gObj->pInventory[RING_02].m_Type == 0x1A7A) //Skeleton Ring
			{
				gObj->m_Change = 14;

				gObjViewportListProtocolCreate(gObj);
			}
		}
	}
	_endthread();
}

bool cUser::CharacterCreate(PMSG_CHARCREATE* lpMsg, int aIndex)
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

bool cUser::GuildMasterInfoSave(int aIndex,PMSG_GUILDINFOSAVE* lpMsg)
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
void GCEquipmentSendHook(int aIndex)
{							
	OBJECTSTRUCT * gObj = (OBJECTSTRUCT*)OBJECT_POINTER(aIndex);

	GCEquipmentSend(aIndex);
	
	//Fix Double Pets on trade cancel/ok/success
	if(gObj->pInventory[8].m_Type != SLOT_EMPTY)
	{
		if(gObj->pInventory[8].m_Type == 0x1A50 || gObj->pInventory[8].m_Type == 0x1A7B)
		{	
			CItem OldItem;
			OldItem = gObj->pInventory[8];
			gObj->pInventory[8].m_Type = -1;

			gObjMakePreviewCharSet(aIndex);
			GCItemListSend(aIndex);
			gObjViewportListProtocolCreate(gObj);

			gObj->pInventory[8] = OldItem;

			gObjMakePreviewCharSet(aIndex);
			GCItemListSend(aIndex);
			gObjViewportListProtocolCreate(gObj);
		}
	}
	
	if(gObj->pInventory[RING_01].m_Type == 0x1A7A 
		|| gObj->pInventory[RING_02].m_Type == 0x1A7A) //Skeleton Ring
		_beginthread( TradeSystem__Cancel, 0, NULL  );
}

void cUser::RingSkin(LPOBJ gObj)
{   
	if(gObj->pInventory[RING_01].m_Type == 0x1A4C && gObj->m_Change != 503 ||
		gObj->pInventory[RING_02].m_Type == 0x1A4C && gObj->m_Change != 503)
	{
		gObj->m_Change = 503;
		gObjViewportListProtocolCreate(gObj);
	}
}

void cUser::CheckRingSend(LPOBJ gObj, unsigned char* aSend)
{
	if(aSend[4] == RING_01 || aSend[4] == RING_02) 
		if(gObj->pInventory[RING_01].m_Type == 0x1A4C && gObj->m_Change != 503 ||
			gObj->pInventory[RING_02].m_Type == 0x1A4C && gObj->m_Change != 503)
		{
			gObj->m_Change = 503;
			gObjViewportListProtocolCreate(gObj);	       
		}
}

void cUser::CheckRing(LPOBJ gObj, LPBYTE aRecv)
{
	if((aRecv[4] == RING_01 && gObj->pInventory[RING_02].m_Type != 0x1A4C) 
		|| (aRecv[4] == RING_02 && gObj->pInventory[RING_01].m_Type != 0x1A4C))
		if(gObj->m_Change == 503)
		{
			gObj->m_Change = -1;	
			gObjViewportListProtocolCreate(gObj);	
		}	
}	

bool cUser::CGPartyRequestRecv(PMSG_PARTYREQUEST * lpMsg, int aIndex)
{	
	int number =  MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(aIndex);	 
	OBJECTSTRUCT *pObj = (OBJECTSTRUCT*)OBJECT_POINTER(number);

	if(gObj->Level > pObj->Level && gObj->Level - pObj->Level >= Configs.PartyGapLvl)
	{	
		Chat.MessageLog(1, c_Red, t_Default, gObj, "[Party] You can't stay with %s in party! %s needs %d more lvl.", pObj->Name, pObj->Name, gObj->Level-Configs.PartyGapLvl - pObj->Level);
		return true;
	}

	if(gObj->Level < pObj->Level && pObj->Level - gObj->Level >= Configs.PartyGapLvl)
	{																													
		Chat.MessageLog(1, c_Red, t_Default, gObj, "[Party] You can't stay with %s in party! You need %d more lvl.", pObj->Name, pObj->Level - Configs.PartyGapLvl - gObj->Level);
		return true;
	}	   
	return false;
}

void cUser::PlayerConnect(LPOBJ gObj)
{	
	LoginMsg(gObj);
	RingSkin(gObj);
	PCPoint.InitPCPointForPlayer(gObj); 

#pragma warning(disable: 4244)
	Me_MuOnlineQuery.ExecQuery("SELECT cspoints FROM MEMB_INFO WHERE memb___id = '%s'", gObj->AccountID);
	Me_MuOnlineQuery.Fetch();
	gObj->m_wCashPoint = Me_MuOnlineQuery.GetAsInteger("cspoints");
	Me_MuOnlineQuery.Close();
#pragma warning(default: 4244)

	if(PCPoint.Config.WebEnabled)
	{
		MuOnlineQuery.ExecQuery("SELECT %s FROM %s WHERE memb___id = '%s'",PCPoint.Config.WebTable ,PCPoint.Config.WebColumn , gObj->AccountID);
		MuOnlineQuery.Fetch();
		AddTab[gObj->m_Index].WEB_Points = MuOnlineQuery.GetAsInteger(PCPoint.Config.WebTable);
		MuOnlineQuery.Close();
		AddTab[gObj->m_Index].WEB_Points;
	}

	MuOnlineQuery.ExecQuery("SELECT %s FROM Character WHERE Name = '%s'", Configs.ResetColumn, gObj->Name);
	MuOnlineQuery.Fetch();
	AddTab[gObj->m_Index].Resets = MuOnlineQuery.GetAsInteger(Configs.ResetColumn);
	MuOnlineQuery.Close();

	AddTab[gObj->m_Index].ON_Min			= 0;   
	AddTab[gObj->m_Index].ON_Sek			= 0;
	AddTab[gObj->m_Index].ON_Hour			= 0;
	AddTab[gObj->m_Index].PC_OnlineTimer	= 0;
	AddTab[gObj->m_Index].ADD_Type			= -1;
	AddTab[gObj->m_Index].ADD_Value			= 0;

#ifdef _GS 
	if(DuelSystem.Config.Enabled)
	{
		if(DuelSystem.Config.Ranking)
		{
			DuelSystem.DuelSetInfo(gObj->m_Index);
		}

		if((!DuelSystem.IsOnDuel(gObj->m_Index)) && gObj->MapNumber == 64)
		{
			gObjMoveGate(gObj->m_Index, 294);
			Log.ConsoleOutPut(1, c_Blue ,t_Duel, "[Duel System][%s][%s] Spawn on duel map after duel is not allowed", gObj->AccountID, gObj->Name);
		}
		DuelSystem.UserDuelInfoReset(gObj);
	}
#endif
	Vip.Connect(gObj);
}

void cUser::LoginMsg(LPOBJ gObj)
{	
	Chat.Message(1, gObj->m_Index, "http://imaginationarts.net/forum/");
	Chat.Message(0, gObj->m_Index, Configs.ConnectNotice);
	if (Configs.ConnectInfo == 1)
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

BOOL gObjGameClose_Func(int aIndex)
{	 						
	OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(aIndex);	  

	MuOnlineQuery_Test.ExecQuery("UPDATE Character SET test_pcpoint = %d WHERE Name = '%s'", AddTab[aIndex].PC_POINT_TEST, gObj->Name);
	MuOnlineQuery_Test.Fetch();
	MuOnlineQuery_Test.Close();

	switch(GmSystem.IsAdmin(gObj->Name))
	{																									   
	case 1:
		Chat.MessageAllLog(0, 0, c_Green, t_GM, gObj, "[Admin] %s exit the game!", gObj->Name);
		break;
	case 2:
		Chat.MessageAllLog(0, 0, c_Green, t_GM, gObj, "[GM] %s exit the game!", gObj->Name);
		break;
	}  
	BOOL rValue = gObjGameClose(aIndex);
	return rValue;		  	
}

void cUser::OnlineTimeTick(LPOBJ gObj)
{
	AddTab[gObj->m_Index].ON_Sek++;
	if(AddTab[gObj->m_Index].ON_Sek >= 60)
	{
		AddTab[gObj->m_Index].ON_Sek = 0;
		AddTab[gObj->m_Index].ON_Min++;
	}
	if(AddTab[gObj->m_Index].ON_Min >= 60)
	{
		AddTab[gObj->m_Index].ON_Min = 0;
		AddTab[gObj->m_Index].ON_Hour++;
		Me_MuOnlineQuery.ExecQuery("UPDATE MEMB_STAT SET OnlineHours = (OnlineHours + 1) WHERE memb___id = '%s'", gObj->AccountID);
		Me_MuOnlineQuery.Fetch();
		Me_MuOnlineQuery.Close();
	}
}