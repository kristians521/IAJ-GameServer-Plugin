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

cVip Vip;
// VIP CONNECT
void cVip::Connect(LPOBJ gObj)
{
	if(Configs.VIP.Enabled)
	{												
		MuOnlineQuery.ExecQuery("SELECT %s, %s FROM Character WHERE Name = '%s'", Configs.VIP.Column, Configs.VIP.ColumnDate, gObj->Name);
		MuOnlineQuery.Fetch();
		AddTab[gObj->m_Index].VIP_Type = MuOnlineQuery.GetAsInteger(Configs.VIP.Column);
		AddTab[gObj->m_Index].VIP_Min = MuOnlineQuery.GetAsInteger(Configs.VIP.ColumnDate);
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
	if(AddTab[gObj->m_Index].VIP_Type > 0 && Configs.VIP.Enabled)
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
				MuOnlineQuery.ExecQuery("UPDATE Character SET %s = 0, %s = 0 WHERE Name = '%s'", Configs.VIP.Column, Configs.VIP.ColumnDate, gObj->Name);
				MuOnlineQuery.Fetch();
				MuOnlineQuery.Close();			
				MuOnlineQuery.ExecQuery("SELECT %s, %s FROM Character WHERE Name = '%s'", Configs.VIP.Column, Configs.VIP.ColumnDate, gObj->Name);
				MuOnlineQuery.Fetch();
				AddTab[gObj->m_Index].VIP_Type = MuOnlineQuery.GetAsInteger(Configs.VIP.Column);
				AddTab[gObj->m_Index].VIP_Min = MuOnlineQuery.GetAsInteger(Configs.VIP.ColumnDate);
				MuOnlineQuery.Close();
			}
			else
			{
				MuOnlineQuery.ExecQuery("UPDATE Character SET %s = (%s - 1) WHERE Name = '%s'", Configs.VIP.ColumnDate, Configs.VIP.ColumnDate, gObj->Name);
				MuOnlineQuery.Fetch();
				MuOnlineQuery.Close();
				MuOnlineQuery.ExecQuery("SELECT %s, %s FROM Character WHERE Name = '%s'", Configs.VIP.Column, Configs.VIP.ColumnDate, gObj->Name);
				MuOnlineQuery.Fetch();
				AddTab[gObj->m_Index].VIP_Type = MuOnlineQuery.GetAsInteger(Configs.VIP.Column);
				AddTab[gObj->m_Index].VIP_Min = MuOnlineQuery.GetAsInteger(Configs.VIP.ColumnDate);
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