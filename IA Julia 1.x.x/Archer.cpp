// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#include "Archer.h"
#include "StdAfx.h"
#include "Logger.h"
#include "Configs.h"
#include "ChatCommands.h"
#include "Utilits.h"
#include "PCPoint.h"


cGoldenArcher GoldenArcher;

enum {INV_CHECK,INV_DEL};
#define ITEMGET(x,y) ((x)*512+(y))

void cGoldenArcher::Load()
{
	Config.Enabled				= Configs.GetInt(0, 1, 1, "GoldenArcher", "ArcherEnabled", IAJuliaArcher);	

	if(!Config.Enabled)
		return;
		
	Config.NeedRenaAmount	= Configs.GetInt(0, 250,								7,		"GoldenArcher", "RenasCount",		IAJuliaArcher); 	   
	Config.WCoinsReward		= Configs.GetInt(0, PCPoint.Config.MaximumWCPoints,	1,		"GoldenArcher", "WCoinsReward",		IAJuliaArcher);
	Config.PCPointsReward	= Configs.GetInt(0, PCPoint.Config.MaximumPCPoints,	1,		"GoldenArcher", "PCPointsReward",	IAJuliaArcher);	   
	Config.ZenReward		= Configs.GetInt(0, 2000000000,							100000, "GoldenArcher", "ZenReward",		IAJuliaArcher);

	FILE *file;
	file = fopen(IAJuliaArcher,"r");

	if (file == NULL)
	{
		Log.ConsoleOutPut(1,c_Red,t_Default,"[Golden Archer] Cant Find MonsterSpawn.ini! Archer disabled!",IAJuliaArcher);
		Config.Enabled = 0;
		return;
	}

	char buffer[256];
	bool flag = false;
	counter = 0;

	while (!feof(file))
	{
		fgets(buffer,256,file);

		if (!strncmp(buffer,"//",strlen("//")) || !strncmp(buffer,"end",strlen("end")) || buffer[0] == 0xA) continue;

		if (!strncmp(buffer,"0",strlen("0")) ) {flag = true; continue;}

		if (flag)
		{
			sscanf(buffer,"%d %d %d %d %d %d %d %d %d",&ItemsPrize[counter].Index,&ItemsPrize[counter].ItemID,&ItemsPrize[counter].RateItem,
				&ItemsPrize[counter].MaxLvl,&ItemsPrize[counter].RateSkill,&ItemsPrize[counter].RateLuck,&ItemsPrize[counter].MaxOpt,
				&ItemsPrize[counter].RateExc,&ItemsPrize[counter].MaxExcOpt);
			if (MaxRateItem < ItemsPrize[counter].RateItem) MaxRateItem = ItemsPrize[counter].RateItem;
			counter++;
		}
	}

	fclose(file);
	Log.ConsoleOutPut(1,c_Magenta,t_NULL,"[GoldenArcher] Golden Archer Loaded %d Items",counter);
}

void cGoldenArcher::GoldenArcherClick(LPOBJ gObj)
{
	unsigned short *CurrentRena;
	CurrentRena = (unsigned short*)&gObj->m_Quest[29];

	if (*CurrentRena > Config.NeedRenaAmount ) *CurrentRena = 0;

	if (!this->ChekingRena(gObj,INV_CHECK))
	{
		Chat.Message(gObj->m_Index,"[Golden Archer] Your haven't renas in Inventory. Search renas and come back to me.");
		return;
	}

	++*CurrentRena;
	this->ChekingRena(gObj,INV_DEL);

	if (*CurrentRena < Config.NeedRenaAmount )
		Chat.Message(gObj->m_Index,"[Golden Archer] Your Rena accepted. You have %d renas, for reward need %d more",
			*CurrentRena,Config.NeedRenaAmount - *CurrentRena);

	if (*CurrentRena == Config.NeedRenaAmount)
	{
		int ArrayItemsIndex[MAX_ITEM_PRIZE];
		int g = -1;

		short RandValue = rand()%100 + 1;
		if (RandValue > MaxRateItem) RandValue = MaxRateItem;

		for (int i = 0; i < counter; i++)
			if (RandValue <= ItemsPrize[i].RateItem)
				ArrayItemsIndex[++g] = i;

		RandValue = rand()% (g + 1);
		int PrizeIndex = ArrayItemsIndex[RandValue];

		int Level,Skill,Luck,Opt,Exc;

		Level = rand()%(ItemsPrize[PrizeIndex].MaxLvl + 1);
		Opt   = rand()%(ItemsPrize[PrizeIndex].MaxOpt + 1);

		Skill = rand() % 100 + 1 < ItemsPrize[PrizeIndex].RateSkill ? Skill = 1 : Skill = 0;
		Luck  = rand() % 100 + 1 < ItemsPrize[PrizeIndex].RateLuck  ? Luck = 1  : Luck = 0;
		Exc   = rand() % 100 + 1 < ItemsPrize[PrizeIndex].RateExc   ? Exc = Utilits.GenExcOpt(rand()%(ItemsPrize[PrizeIndex].MaxExcOpt + 1)) : Exc = 0;
		
		*CurrentRena = 0;
		Chat.Message(gObj->m_Index,"[Golden Archer] Thank you for renas! Take my rewards for you.");
		Log.ConsoleOutPut(1,c_Magenta,t_DROP,"[Golden Archer] Golden Archer drop %d %d %d %d %d %d %d 0",ItemsPrize[PrizeIndex].Index,
			ItemsPrize[PrizeIndex].ItemID,Level,Skill,Luck,Opt,Exc);

		int Item = ITEMGET(ItemsPrize[PrizeIndex].Index,ItemsPrize[PrizeIndex].ItemID);
		ItemSerialCreateSend (gObj->m_Index,gObj->MapNumber,(BYTE)gObj->X,(BYTE)gObj->Y,Item,Level,0,Skill,Luck,Opt,gObj->m_Index,Exc,0);

		if (Config.ZenReward > 0)
		{
			int UpdateZen = gObj->Money + Config.ZenReward;

			if (UpdateZen > 2000000000)
				UpdateZen = 2000000000;
			gObj->Money = UpdateZen;
			GCMoneySend(gObj->m_Index,UpdateZen);
			Chat.Message(gObj->m_Index,"[Golden Archer] Added Zen:%d", Config.ZenReward);
		}
		if (Config.WCoinsReward > 0)
		{
			PCPoint.UpdatePoints(gObj,Config.WCoinsReward,PLUS,WCOIN);
			Chat.Message(gObj->m_Index,"[Golden Archer] Added Coins:%d", Config.WCoinsReward);
		}
		if (Config.PCPointsReward > 0)
		{
			PCPoint.UpdatePoints(gObj,Config.PCPointsReward,PLUS,PCPOINT);
			Chat.Message(gObj->m_Index,"[Golden Archer] Added PCPoints:%d", Config.PCPointsReward);
		}
	}
}

bool cGoldenArcher::ChekingRena(LPOBJ gObj,int Mode)
{
	for (BYTE i = 12; i<76; i++ )
	{
		if (gObj->pInventory[i].m_Type == 0x01C15 && gObj->pInventory[i].m_Level == 0 )
		{
			switch (Mode)
			{
			case INV_CHECK:
				return true;
			case INV_DEL:
				gObjInventoryDeleteItem ( gObj->m_Index , i );
				GCInventoryItemDeleteSend ( gObj->m_Index , i  , 1 );
				return true;
			}
		}
	}
	return false;
}