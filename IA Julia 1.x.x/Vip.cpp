// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#include "StdAfx.h"
#include "Vip.h"
#include "User.h"
#include "Configs.h"
#include "Query.h"
#include "ChatCommands.h"
#include "Logger.h"
#include "PCPoint.h"

cVip Vip;

void cVip::Load()
{
	Config.Enabled = Configs.GetInt(0, 1, 1, "VipSystem", "EnableVip", IAJuliaVIP); 
	if(!Config.Enabled)return;

	GetPrivateProfileString("VipSystem","VIPColumn",	"VIP",		Config.Column,		sizeof(Config.Column),		IAJuliaVIP);
	GetPrivateProfileString("VipSystem","VIPColumnDate","VIP_DATE", Config.ColumnDate,	sizeof(Config.ColumnDate),  IAJuliaVIP);	 				 

	MuOnlineQuery.CheckColumn(Config.Column,	 "Character", "ALTER TABLE Character ADD [%s][int] DEFAULT (0) NOT NULL", Config.Column);
	MuOnlineQuery.CheckColumn(Config.ColumnDate, "Character", "ALTER TABLE Character ADD [%s][int] DEFAULT (0) NOT NULL", Config.ColumnDate);

	Config.NumStates = Configs.GetInt(0, 10, 3, "VipSystem", "NumStates", IAJuliaVIP);	

	char PState[10]; 
	for(int i = 1; i <= Config.NumStates; i++)
	{
		wsprintf(PState, "State%d", i);

		GetPrivateProfileString(PState,"VIPStateName","bronze",Config.VIPState[i].VIPName,sizeof(Config.VIPState[i].VIPName), IAJuliaVIP);	  
		Config.VIPState[i].EnabledCmd	= Configs.GetInt(0, 1,								 1,		PState, "AllowAutoBuy",	IAJuliaVIP); 
		Config.VIPState[i].CostPCPoints	= Configs.GetInt(0, PCPoint.Config.MaximumPCPoints, 5,		PState,	"CostPCPoints",	IAJuliaVIP);
		Config.VIPState[i].CostWCoins	= Configs.GetInt(0, PCPoint.Config.MaximumWCPoints, 5,		PState,	"CostWCoins",	IAJuliaVIP);
		Config.VIPState[i].CostZen		= Configs.GetInt(0, 2000000000,						 5000,	PState,	"CostZen",		IAJuliaVIP);

		Config.VIPState[i].BonusExp		= Configs.GetInt(0, 9999,							 5,		PState,	"BonusExp",		IAJuliaVIP);
		Config.VIPState[i].BonusZen		= Configs.GetInt(0, 9999,							 5,		PState,	"BonusZen",		IAJuliaVIP);
		Config.VIPState[i].BonusDrop	= Configs.GetInt(0, 9999,							 5,		PState,	"BonusDrop",	IAJuliaVIP);

		Config.VIPState[i].MinHours		= Configs.GetInt(0, 32000,							 1,		PState,	"MinHours",		IAJuliaVIP);
		Config.VIPState[i].MaxHours		= Configs.GetInt(Config.VIPState[i].MinHours, 32000, 200,	PState,	"MaxHours",		IAJuliaVIP);
	}
}

void cVip::Connect(LPOBJ gObj)
{
	if(Config.Enabled)
	{												
		MuOnlineQuery.ExecQuery("SELECT %s, %s FROM Character WHERE Name = '%s'", Config.Column, Config.ColumnDate, gObj->Name);
		MuOnlineQuery.Fetch();
		AddTab[gObj->m_Index].VIP_Type = MuOnlineQuery.GetAsInteger(Config.Column);
		AddTab[gObj->m_Index].VIP_Min = MuOnlineQuery.GetAsInteger(Config.ColumnDate);
		MuOnlineQuery.Close();

		AddTab[gObj->m_Index].VIP_Sec = 0; // Обнуление секунд при входе
		if(AddTab[gObj->m_Index].VIP_Min > 0)
		{											 
			Chat.MessageLog(1, c_Red, t_VIP, gObj, "[VIP] Left %d minutes of VIP.", AddTab[gObj->m_Index].VIP_Min);
		} 
	}
}

void cVip::Tick(LPOBJ gObj)
{
	if(AddTab[gObj->m_Index].VIP_Type > 0 && Config.Enabled)
	{ 
		AddTab[gObj->m_Index].VIP_Sec++;
		if(AddTab[gObj->m_Index].VIP_Sec >= 60)
		{
			AddTab[gObj->m_Index].VIP_Min--;
			AddTab[gObj->m_Index].VIP_Sec = 0;

			if(CheckVipTime(AddTab[gObj->m_Index].VIP_Min))
			{			
				Chat.MessageLog(1, c_Red, t_VIP, gObj, "[VIP] You have %d more vip minutes", AddTab[gObj->m_Index].VIP_Min); 
			}
			if(AddTab[gObj->m_Index].VIP_Min <= 0)
			{
				Chat.MessageLog(1, c_Red, t_VIP, gObj, "[VIP] Your vip time is over! You are normal player again."); 
				AddTab[gObj->m_Index].VIP_Type = 0;
				AddTab[gObj->m_Index].VIP_Min = 0;
				MuOnlineQuery.ExecQuery("UPDATE Character SET %s = 0, %s = 0 WHERE Name = '%s'", Config.Column, Config.ColumnDate, gObj->Name);
				MuOnlineQuery.Fetch();
				MuOnlineQuery.Close();			
				MuOnlineQuery.ExecQuery("SELECT %s, %s FROM Character WHERE Name = '%s'", Config.Column, Config.ColumnDate, gObj->Name);
				MuOnlineQuery.Fetch();
				AddTab[gObj->m_Index].VIP_Type = MuOnlineQuery.GetAsInteger(Config.Column);
				AddTab[gObj->m_Index].VIP_Min = MuOnlineQuery.GetAsInteger(Config.ColumnDate);
				MuOnlineQuery.Close();
			}
			else
			{
				MuOnlineQuery.ExecQuery("UPDATE Character SET %s = (%s - 1) WHERE Name = '%s'", Config.ColumnDate, Config.ColumnDate, gObj->Name);
				MuOnlineQuery.Fetch();
				MuOnlineQuery.Close();
				MuOnlineQuery.ExecQuery("SELECT %s, %s FROM Character WHERE Name = '%s'", Config.Column, Config.ColumnDate, gObj->Name);
				MuOnlineQuery.Fetch();
				AddTab[gObj->m_Index].VIP_Type = MuOnlineQuery.GetAsInteger(Config.Column);
				AddTab[gObj->m_Index].VIP_Min = MuOnlineQuery.GetAsInteger(Config.ColumnDate);
				MuOnlineQuery.Close();
			}
		}
	}
}

bool cVip::CheckVipTime(int TimeInMin)
{
	switch(TimeInMin)
	{
	case 1:
	case 2:
	case 3:
	case 5:
	case 15:
	case 30:
	case 60:
	case 120:
	case 340:
	case 680:
		return true;
	}
	return false;
}