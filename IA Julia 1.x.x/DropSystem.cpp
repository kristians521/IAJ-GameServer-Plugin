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
			int n[12];
			sscanf(zBuf,"%d %d %d %d %d %d %d %d %d %d %d %d", &n[0], &n[1], &n[2], &n[3], &n[4], &n[5], &n[6], &n[7], &n[8], &n[9], &n[10], &n[11]);
			
			if(n[0] == -1)
			{
				int j = AllMobArrayMaxItem;
				if(j >= 1000)
					continue;
				AllMobItemsDrop[j].Map			= n[1];
				AllMobItemsDrop[j].MinLvl		= n[2];
				AllMobItemsDrop[j].MaxLvl		= n[3];
				AllMobItemsDrop[j].RateItem		= n[4];
				AllMobItemsDrop[j].Group		= n[5];
				AllMobItemsDrop[j].Index		= n[6];
				AllMobItemsDrop[j].Level		= n[7];
				AllMobItemsDrop[j].Option		= n[8];
				AllMobItemsDrop[j].Skill		= n[9];
				AllMobItemsDrop[j].Luck			= n[10];
				AllMobItemsDrop[j].Exc			= n[11];

				AllMobArrayMaxItem = ++j;
			}
			else
			{
				int MobId = n[0];
				int j = ArrayMaxItem[MobId];
				if(j >= MAX_ITEM_FOR_MONSTER)
					continue;
				ItemsDrop[MobId][j].Map			= n[1];
				ItemsDrop[MobId][j].MinLvl		= n[2];
				ItemsDrop[MobId][j].MaxLvl		= n[3];
				ItemsDrop[MobId][j].RateItem	= n[4];
				ItemsDrop[MobId][j].Group		= n[5];
				ItemsDrop[MobId][j].Index		= n[6];
				ItemsDrop[MobId][j].Level		= n[7];
				ItemsDrop[MobId][j].Option		= n[8];
				ItemsDrop[MobId][j].Skill		= n[9];
				ItemsDrop[MobId][j].Luck		= n[10];
				ItemsDrop[MobId][j].Exc			= n[11];

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
	short MapArrayItem[MAX_ITEM_FOR_MONSTER];
	short CountArrayItem = 0;
	short LvlArrayItem[MAX_ITEM_FOR_MONSTER];
	short CountLvlArrayItem = 0;

	for(int i = 0; i < MaxItem; i++)
	{
		if(ItemDrop[i].Map == mObj->MapNumber || ItemDrop[i].Map == -1)
		{
			MapArrayItem[CountArrayItem] = i;
			CountArrayItem++;
		}
	}

	if(CountArrayItem == 0) return false;

	for(int j = 0; j < CountArrayItem; j++)
	{
		if((ItemDrop[MapArrayItem[j]].MinLvl <= mObj->Level &&
			ItemDrop[MapArrayItem[j]].MaxLvl >= mObj->Level) ||
			ItemDrop[MapArrayItem[j]].MaxLvl == 0)
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

	short RateArrayItem[MAX_ITEM_FOR_MONSTER];
	short CountRateItem = 0;

	for(int f = 0; f < CountLvlArrayItem; f++)
	{
		if(ItemDrop[LvlArrayItem[f]].RateItem >= RandomValue)
		{
			RateArrayItem[CountRateItem] = LvlArrayItem[f];
			CountRateItem++;
		}
	}

	if(CountRateItem == 0) return false;

	int RandomItem = rand() % CountRateItem;	

	int Level,Skill,Luck,Opt,Exc,Group,Index;

	Group	= ItemDrop[RateArrayItem[RandomItem]].Group;
	Index	= ItemDrop[RateArrayItem[RandomItem]].Index;
	Level	= ItemDrop[RateArrayItem[RandomItem]].Level;
	Opt		= ItemDrop[RateArrayItem[RandomItem]].Option;
	Luck	= ItemDrop[RateArrayItem[RandomItem]].Luck;
	Skill	= ItemDrop[RateArrayItem[RandomItem]].Skill; 
	Exc		= ItemDrop[RateArrayItem[RandomItem]].Exc;

	int Item = ITEMGET(Group,Index);

	ItemSerialCreateSend (mObj->m_Index,mObj->MapNumber,(BYTE)mObj->X,(BYTE)mObj->Y,Item,Level,0,Skill,Luck,Opt,pObj->m_Index,Exc,0);

	Log.ConsoleOutPut(1,c_Green,t_DROP,"[Drop System] [R=%d] Near %s drop %d %d %d %d %d %d %d",RandomValue,pObj->Name,Group,Index,Level,Luck,Skill,Opt,Exc);

	return true;
} 