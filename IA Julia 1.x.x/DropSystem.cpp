// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#include "DropSystem.h"
#include "StdAfx.h"
#include "Logger.h"
#include "Utilits.h"
#include "Configs.h"

#define ITEMGET(x,y) ((x)*512+(y))

cDropSystem DropSystem;

cDropSystem::cDropSystem() {};
cDropSystem::~cDropSystem() {};

void cDropSystem::LoadDropItems()
{
	FILE *file;
	file = fopen(IAJuliaDropSystem,"r");

	if (file == NULL)
	{
		Log.ConsoleOutPut(1,c_Green,t_NULL,"[Drop System] Drop System not loaded");
		return;
	}

	char zBuf[256];
	bool flag = false;

	while(!feof(file))
	{
		fgets(zBuf,256,file);

		if (!strncmp(zBuf,"//",strlen("//")) || !strncmp(zBuf,"end",strlen("end")) || zBuf[0] == '\0' ) continue;

		if (!strncmp(zBuf,"0",strlen("0")) ) {flag = true; continue;}

		if (flag)
		{
			int n[22];
			sscanf(zBuf, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", &n[0], &n[1], &n[2], &n[3], &n[4], &n[5], &n[6], &n[7], &n[8], &n[9], &n[10], &n[11], &n[12], &n[13], &n[14], &n[15], &n[16], &n[17], &n[18], &n[19], &n[20], &n[21]);
			
			if(n[0] == -1)
			{
				int j = AllMobArrayMaxItem;
				if(j >= 1000)
					continue;
				AllMobItemsDrop[j].MMap			= n[1];
				AllMobItemsDrop[j].MMinLvl		= n[2];
				AllMobItemsDrop[j].MMaxLvl		= n[3];
				AllMobItemsDrop[j].IDropRate	= n[4];
				AllMobItemsDrop[j].IGroup		= n[5];
				AllMobItemsDrop[j].IIndex		= n[6];
				AllMobItemsDrop[j].IMinLvl		= n[7];
				AllMobItemsDrop[j].IMaxLvl		= n[8];
				AllMobItemsDrop[j].ILvlRate		= n[9];
				AllMobItemsDrop[j].IMinOpt		= n[10];
				AllMobItemsDrop[j].IMaxOpt		= n[11];
				AllMobItemsDrop[j].IOptRate		= n[12];
				AllMobItemsDrop[j].ISkill		= n[13];
				AllMobItemsDrop[j].ISkillRate	= n[14];
				AllMobItemsDrop[j].ILuck		= n[15];
				AllMobItemsDrop[j].ILuckRate	= n[16];
				AllMobItemsDrop[j].IMinExc		= n[17];
				AllMobItemsDrop[j].IMaxExc		= n[18];
				AllMobItemsDrop[j].IExcRate		= n[19];
				AllMobItemsDrop[j].IAnc			= n[20];
				AllMobItemsDrop[j].IAncRate		= n[21];

				AllMobArrayMaxItem = ++j;
			}
			else
			{
				int MobId = n[0];
				int j = ArrayMaxItem[MobId];
				if(j >= MAX_ITEM_FOR_MONSTER)
					continue;
				ItemsDrop[MobId][j].MMap		= n[1];
				ItemsDrop[MobId][j].MMinLvl		= n[2];
				ItemsDrop[MobId][j].MMaxLvl		= n[3];
				ItemsDrop[MobId][j].IDropRate	= n[4];
				ItemsDrop[MobId][j].IGroup		= n[5];
				ItemsDrop[MobId][j].IIndex		= n[6];
				ItemsDrop[MobId][j].IMinLvl		= n[7];
				ItemsDrop[MobId][j].IMaxLvl		= n[8];
				ItemsDrop[MobId][j].ILvlRate	= n[9];
				ItemsDrop[MobId][j].IMinOpt		= n[10];
				ItemsDrop[MobId][j].IMaxOpt		= n[11];
				ItemsDrop[MobId][j].IOptRate	= n[12];
				ItemsDrop[MobId][j].ISkill		= n[13];
				ItemsDrop[MobId][j].ISkillRate	= n[14];
				ItemsDrop[MobId][j].ILuck		= n[15];
				ItemsDrop[MobId][j].ILuckRate	= n[16];
				ItemsDrop[MobId][j].IMinExc		= n[17];
				ItemsDrop[MobId][j].IMaxExc		= n[18];
				ItemsDrop[MobId][j].IExcRate	= n[19];
				ItemsDrop[MobId][j].IAnc		= n[20];
				ItemsDrop[MobId][j].IAncRate	= n[21];

				ArrayMaxItem[MobId] = ++j;
			}			
		}
	}

	fclose(file);
	Log.ConsoleOutPut(1,c_Green,t_NULL,"[Drop System] Drop System Loaded");
}

bool cDropSystem::DropItem(LPOBJ mObj,LPOBJ pObj)
{
	if(ArrayMaxItem[mObj->Class] == 0)
		return DropItem2(mObj, pObj, AllMobItemsDrop, AllMobArrayMaxItem);
	else
		return DropItem2(mObj, pObj, ItemsDrop[mObj->Class], ArrayMaxItem[mObj->Class]);
}

bool cDropSystem::DropItem2(LPOBJ mObj,LPOBJ pObj, sItemsDrop ItemDrop[], unsigned int MaxItem)
{
	int MapArrayItem[1000];
	int CountArrayItem = 0;
	int LvlArrayItem[1000];
	int CountLvlArrayItem = 0;

	for(unsigned int i = 0; i < MaxItem; i++)
	{
		if(ItemDrop[i].MMap == mObj->MapNumber || ItemDrop[i].MMap == -1)
		{
			MapArrayItem[CountArrayItem] = i;
			CountArrayItem++;
		}
	}

	if(CountArrayItem == 0) return false;

	for(int j = 0; j < CountArrayItem; j++)
	{
		if((ItemDrop[MapArrayItem[j]].MMinLvl <= mObj->Level &&
			ItemDrop[MapArrayItem[j]].MMaxLvl >= mObj->Level) ||
			ItemDrop[MapArrayItem[j]].MMaxLvl == 0)
		{
			LvlArrayItem[CountLvlArrayItem] = MapArrayItem[j];
			CountLvlArrayItem++;
		}
	}

	if(CountLvlArrayItem == 0) return false;
	
	int RandomValue = rand() % 10000 + 1;
	if(LastRandomValue == RandomValue)
		return false;
	else
		LastRandomValue = RandomValue;

	int RateArrayItem[1000];
	int CountRateItem = 0;

	for(int f = 0; f < CountLvlArrayItem; f++)
	{
		if(ItemDrop[LvlArrayItem[f]].IDropRate >= RandomValue)
		{
			RateArrayItem[CountRateItem] = LvlArrayItem[f];
			CountRateItem++;
		}
	}

	if(CountRateItem == 0) return false;

	int RandomItem = rand() % CountRateItem;	

	int Level=0, Skill=0, Luck=0, Opt=0, Exc=0, ExcNum=0, Group=0, Index=0, Anc=0;

	Index	= ItemDrop[RateArrayItem[RandomItem]].IIndex;
	Group	= ItemDrop[RateArrayItem[RandomItem]].IGroup;
	if(Group == -1)
	{	
		if(Index == 15 || Index == 20 || Index == 23 || Index == 32 || Index == 37 || Index == 47 || Index == 48)
			Group = rand()%4+8;
		else
			Group = rand()%5+7;
	}
	Level	= Utilits.GetNumberByPercent(ItemDrop[RateArrayItem[RandomItem]].ILvlRate,
										ItemDrop[RateArrayItem[RandomItem]].IMinLvl,
										ItemDrop[RateArrayItem[RandomItem]].IMaxLvl);
	Opt		= Utilits.GetNumberByPercent(ItemDrop[RateArrayItem[RandomItem]].IOptRate,
										ItemDrop[RateArrayItem[RandomItem]].IMinOpt,
										ItemDrop[RateArrayItem[RandomItem]].IMaxOpt);
	Skill	= Utilits.GetNumberByPercent(ItemDrop[RateArrayItem[RandomItem]].ISkillRate, 0, 
										ItemDrop[RateArrayItem[RandomItem]].ISkill);
	Luck	= Utilits.GetNumberByPercent(ItemDrop[RateArrayItem[RandomItem]].ILuckRate, 0, 
										ItemDrop[RateArrayItem[RandomItem]].ILuck);

	if(ItemDrop[RateArrayItem[RandomItem]].IExcRate > 0)
	{
		ExcNum	= Utilits.GetNumberByPercent(ItemDrop[RateArrayItem[RandomItem]].IExcRate,
											ItemDrop[RateArrayItem[RandomItem]].IMinExc,
											ItemDrop[RateArrayItem[RandomItem]].IMaxExc);
		Exc		= Utilits.GenExcOpt(ExcNum);
	}

	if(ItemDrop[RateArrayItem[RandomItem]].IAnc > 0)
		if(ItemDrop[RateArrayItem[RandomItem]].IAncRate > rand()%100)
			Anc = ItemDrop[RateArrayItem[RandomItem]].IAnc;

	int Item = ITEMGET(Group,Index);

	ItemSerialCreateSend (mObj->m_Index,mObj->MapNumber,(BYTE)mObj->X,(BYTE)mObj->Y,Item,Level,0,Skill,Luck,Opt,pObj->m_Index,Exc,Anc);

	Log.ConsoleOutPut(1,c_Green,t_DROP,"[Drop System] [R=%d] Near %s drop %d %d %d %d %d %d %d %d",RandomValue,pObj->Name,Group,Index,Level,Luck,Skill,Opt,Exc,Anc);

	return true;
} 