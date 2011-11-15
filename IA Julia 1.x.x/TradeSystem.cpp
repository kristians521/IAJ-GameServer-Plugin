// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#include "StdAfx.h"
#include "TradeSystem.h"
#include "ChatCommands.h"
#include "Configs.h"
#include "Logger.h"
#include "Prodef.h"
#include "Utilits.h"

#define ITEMGET(x,y) ((x)*512+(y))
cTradeSystem TradeSystem;

void cTradeSystem::Load()
{
	Enabled = Configs.GetInt(0,1,1,"TradeSystem","EnableTradeSystem",IAJuliaTradeSystem);
	if (Enabled == 0) return;

	char Buff[256];
	int Flag = 0;
	TradeItemsCount = 0;
	
	FILE *conf = fopen(IAJuliaTradeSystem,"r");

	if ( conf != NULL)
	{
		while ( !feof(conf))
		{
			fgets(Buff,256,conf);

			if(Utilits.IsBadFileLine(Buff, &Flag))
				continue;

			if (Flag == 1)
			{
				int n[7];
				//	Group	Index	Level	Skill	Luck	Opt	Exc	Anc
				sscanf(Buff, "%d %d %d %d %d %d %d %d", &n[0], &n[1], &n[2], &n[3], &n[4], &n[5], &n[6], &n[7]);
				TradeItems[TradeItemsCount].Group	= n[0];
				TradeItems[TradeItemsCount].Index	= n[1];
				TradeItems[TradeItemsCount].Level	= n[2];
				TradeItems[TradeItemsCount].Skill	= n[3];
				TradeItems[TradeItemsCount].Luck	= n[4];
				TradeItems[TradeItemsCount].Opt		= n[5];
				TradeItems[TradeItemsCount].Exc		= n[6];
				TradeItems[TradeItemsCount].Anc		= n[7];

				TradeItemsCount++;
			}
		}
	}			  
	fclose(conf);

	Log.ConsoleOutPut(1, c_Magenta, t_Default, "[Trade System] Loaded %d items." ,TradeItemsCount);
}

bool cTradeSystem::CheckTradeItem(LPOBJ gObj)
{
	if(!Enabled)
		return false;

	for (int n=0;n<32;n++)
	{
		if (gObj->Trade[n].m_Type >= 0)
		{
			for(int i = 0; i < TradeItemsCount; i++)
			{
				if(TradeItems[i].Group != -1 && TradeItems[i].Index != -1)
				{
					if(gObj->Trade[n].m_Type != ITEMGET(TradeItems[i].Group,TradeItems[i].Index))
						continue;
				}
				if(TradeItems[i].Group == -1 && TradeItems[i].Index != -1)
				{
					int Temp = (int)floor(gObj->Trade[n].m_Type/512.0f);
					if(gObj->Trade[n].m_Type != ITEMGET(Temp, TradeItems[i].Index))
					{
						continue;
					}
				}
				if(TradeItems[i].Group != -1 && TradeItems[i].Index == -1)
				{
					if(gObj->Trade[n].m_Type < ITEMGET(TradeItems[i].Group, 0) ||
						gObj->Trade[n].m_Type >= ITEMGET(TradeItems[i].Group+1, 0))
						continue;
				}

				if(TradeItems[i].Level != -1 && gObj->Trade[n].m_Level < TradeItems[i].Level)
					continue;

				if(TradeItems[i].Skill != -1 && gObj->Trade[n].m_Option1 != TradeItems[i].Skill)
					continue;

				if(TradeItems[i].Luck != -1 && gObj->Trade[n].m_Option2 != TradeItems[i].Luck)
					continue;

				if(TradeItems[i].Opt != -1 && gObj->Trade[n].m_Option3 < TradeItems[i].Opt)
					continue;

				if(TradeItems[i].Exc != -1 && Utilits.TakeExcNum(gObj->Trade[n].m_NewOption) < TradeItems[i].Exc)
					continue;

				if(TradeItems[i].Anc != -1 && gObj->Trade[n].m_SetOption < TradeItems[i].Anc)
					continue;

				//# TODO Make MSG of what item can't be traded
				return true;
			}
		}
	}
	return false;
}