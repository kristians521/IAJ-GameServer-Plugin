// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#include "StdAfx.h"
#include "PlayerSystem.h"
#include "Prodef.h"
#include "Logger.h"
#include "Configs.h"
#include "Utilits.h"

cPlayerSystem PlayerSystem;

void cPlayerSystem::Load()
{
	Enabled				= Configs.GetInt(0, 1, 1,"PlayerSystem", "EnablePlayerSystem", IAJuliaPlayerSystem);	
	if(!Enabled)return; 

	FILE *fp;
	fp = fopen(IAJuliaPlayerSystem, "r");

	if(fp == NULL)
	{
		Log.ConsoleOutPut(1, c_Cyan, t_Default, "[Player System] System not active. Config file not found: %s", IAJuliaPlayerSystem);
		return;
	}
	rewind(fp);

	int Flag = -1;	 
	char Buff[256];
	ConfigNum = 0;

	while(!feof(fp))
	{
		fgets(Buff, 256, fp);

		if(Utilits.IsBadFileLine(Buff, &Flag))
			continue;

		if(Flag == 1)
		{				
			int temp[12];
			sscanf(Buff, "%d %d %d %d %d %d %d %d %d %d %d %d", &temp[0], &temp[1], &temp[2], &temp[3], &temp[4], &temp[5], &temp[6], &temp[7], &temp[8], &temp[9], &temp[10], &temp[11]);

			Config[ConfigNum].Class[0]	= temp[0];
			Config[ConfigNum].Class[1]	= temp[1];
			Config[ConfigNum].Class[2]	= temp[2];
			Config[ConfigNum].Class[3]	= temp[3];
			Config[ConfigNum].Class[4]	= temp[4];
			Config[ConfigNum].Class[5]	= temp[5];
			Config[ConfigNum].First		= temp[6];
			Config[ConfigNum].Second	= temp[7];
			Config[ConfigNum].Third		= temp[8];
			Config[ConfigNum].Zen		= temp[9];
			Config[ConfigNum].Drop		= temp[10];
			Config[ConfigNum].Exp		= temp[11];
			ConfigNum++;
		}
	}
	fclose(fp);

	Log.ConsoleOutPut(1, c_Magenta, t_Default, "[Player System] Loaded %d lines." ,ConfigNum);
}

int cPlayerSystem::GetBonus(LPOBJ gObj, eType Type)
{
	if(Enabled == 0)
		return 0;

	int Result = 0;
	for(int i = 0; i < ConfigNum; i++)
	{
		if(Config[i].Class[gObj->Class] == 0)
			continue;

		if((Config[i].First == 1 && gObj->ChangeUP == 0 && gObj->ChangeUP2 == 0) ||
			(Config[i].Second == 1 && gObj->ChangeUP == 1 && gObj->ChangeUP2 == 0) ||
			(Config[i].Third == 1 && gObj->ChangeUP == 1 && gObj->ChangeUP2 == 1))
		{
			switch(Type)
			{
			case Zen:
				Result += Config[i].Zen;
				break;
			case Drop:
				Result += Config[i].Drop;
				break;
			case Exp:
				Result += Config[i].Exp;
				break;
			}
		}
	}
	return Result;
}