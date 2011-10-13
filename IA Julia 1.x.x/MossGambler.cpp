// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#include "StdAfx.h"
#include "Configs.h"
#ifdef _GS
#include "MossGambler.h" 
#include "Prodef.h"
#include "User.h"
#include "ChatCommands.h" 
#include "Utilits.h"
#include "PCPoint.h"
#include "Monster.h"
cMoss Moss;
#define ITEMGET(x,y) ((x)*512+(y))
enum {OPT_LEVEL,OPT_SKILL,OPT_LUCK,OPT_ADD,OPT_EXC,OPT_ANC};

struct PMSG_BUYRESULT
{
	PBMSG_HEAD h;	// C1:32
	BYTE Result;	// 3
	BYTE ItemInfo[12];	// 4
};

void MossBuyDelay(void *lparam)
{
	Moss.Delay = true;
	Sleep(1500);
	Moss.Delay = false;
	_endthread();
}

cMoss::cMoss() {}
cMoss::~cMoss() {}

void cMoss::DataSendMoss(int Index)
{
	PMSG_BUYRESULT pRez;
	pRez.h.c=0xC1;
	pRez.h.headcode=0x32;
	pRez.h.size=16;
	DataSend(Index,(LPBYTE)&pRez,pRez.h.size);
}

void cMoss::Load()
{
	Config.Enable		= Configs.GetInt(0,1,0,"Moss","EnableMoss",IAJuliaMossGambler); 
	if(!Config.Enable)return;

	Config.EnableTimer		= Configs.GetInt(0,1,0,"Moss","EnableTimer",IAJuliaMossGambler);
	Config.UsePK			= Configs.GetInt(0,1,0,"Moss","UsePK",IAJuliaMossGambler);

	Config.PriceZen			= Configs.GetInt(0,2000000000,100000000,"Moss","PriceZen",IAJuliaMossGambler);
	Config.PricePCPoint		= Configs.GetInt(0,1000,10,"Moss","PricePCPoint",IAJuliaMossGambler);
	Config.PriceWCoin		= Configs.GetInt(0,1000,10,"Moss","PriceWCoin",IAJuliaMossGambler);

	Config.RandExc			= Configs.GetInt(0,100,50,"Random","RandExc",IAJuliaMossGambler);
	Config.MaxExcOpt		= Configs.GetInt(0,100,6,"Random","MaxExcOpt",IAJuliaMossGambler);

	Config.RandLevel		= Configs.GetInt(0,100,85,"Random","RandLevel",IAJuliaMossGambler);
	Config.MaxLevel			= Configs.GetInt(0,100,13,"Random","MaxLevel",IAJuliaMossGambler);

	Config.RandOptAdd		= Configs.GetInt(0,100,85,"Random","RandOptAdd",IAJuliaMossGambler);
	Config.MaxOptAdd		= Configs.GetInt(0,100,7,"Random","MaxOptAdd",IAJuliaMossGambler);

	Config.RandLuck			= Configs.GetInt(0,100,50,"Random","RandLuck",IAJuliaMossGambler);
	Config.RandSkill		= Configs.GetInt(0,100,50,"Random","RandSkill",IAJuliaMossGambler);
	Config.RandAncient		= Configs.GetInt(0,100,20,"Random","RandAncient",IAJuliaMossGambler);
	Moss.LoadItemInfo();
	if (Config.EnableTimer) Moss.LoadTimeConfig();
}

void cMoss::LoadItemInfo()
{
	FILE *file;
	file = fopen("..\\Data\\Lang\\Kor\\Item(Kor).txt","r");

	if (file == NULL)
	{
		Log.ConsoleOutPut(0,c_Red,t_NULL,"[Moss The Gambler] Cant Find Item(kor).txt, Moss The Gambler Disabled");
		Config.Enable = 0;
		return;
	}

	char zBuf[1024];
	int group = 0;
	int j = 0;

	while (!feof(file))
	{
		fgets(zBuf,1024,file);

		if (!strncmp(zBuf,"//",strlen("//")) || !strncmp(zBuf,"end",strlen("end")) || zBuf[0] == 0xA ) continue;

		char c[2];
		sprintf(c,"%d",group);

		if (!strncmp(zBuf,c,strlen(c)))
		{
			group++;
			if (group > 6) break;
			j = 0;
			continue;
		}
		if (group > 0)
		{
			sscanf(zBuf,"%d ",	&ItemInfo[group-1][j].Index);	
			OrderItems[group-1] = ++j;
		}
	}
	Log.ConsoleOutPut(1,c_Blue,t_NULL,"[Moss The Gambler] Loaded successfully");
	fclose(file);
}

void cMoss::LoadTimeConfig()
{ 
	FILE * file;

	file = fopen(IAJuliaEventTime,"r");

	if (file == NULL)
	{
		Log.ConsoleOutPut(0,c_Red,t_NULL,"[Moss The Gambler] Cant Find EventTime.dat, Timer in Moss Disabled");
		Config.EnableTimer = 0;
		return;
	}

	char zbuf[1024]; bool flag = false; int j = 0;

	while (!feof(file))
	{
		fgets(zbuf,1024,file);

		if (!strncmp(zbuf,"//",strlen("//")) || zbuf[0] == 0xA || !strncmp(zbuf,"end",strlen("end"))) continue;

		if (!strncmp(zbuf,"0",strlen("0")))
		{
			flag = true;
			continue;
		}
		if (flag)
		{
			int indexEvent,closeHour,closeMin;
			sscanf(zbuf,"%d %d %d %d",&indexEvent,&Timer[j].hour,&Timer[j].minute,&Timer[j].delay);
			if (indexEvent != 1) break;

			closeMin = Timer[j].minute + Timer[j].delay;
			closeHour = Timer[j].hour;

			while (closeMin >= 60)	{ closeMin -=60; closeHour++; }

			Timer[j].closehour = closeHour;
			Timer[j].closemin = closeMin;
			j++;
		}
	}
	this->AmountTimers = j;
	Opened = FALSE; 
}

void cMoss::CheckTime()
{
	if(!Config.Enable)
		return;

	if(!Config.EnableTimer)
	{
		Moss.Opened = TRUE;
		return;
	}

	CTime t = CTime::GetCurrentTime();

	short hour,min;

	min = t.GetMinute();
	hour = t.GetHour();

	if (this->Opened == FALSE)
		this->Disappear();

	if (t.GetSecond() == 0)
	{
		for (int i=0; i<AmountTimers; i++)
		{
			for (int j=1; j<6; j++)
			{
				int BeforeOpenMin, BeforeOpenHour;
				BeforeOpenMin = Timer[i].minute - j;
				BeforeOpenHour = Timer[i].hour;

				if (BeforeOpenMin < 0 ) {BeforeOpenMin += 60; BeforeOpenHour --;}

				if (hour == BeforeOpenHour && min == BeforeOpenMin)
				{
					Chat.MessageAll(0,0,NULL,"Moss The Gambler will arrive after %d minute(s)",j);
					break;
				}
			}

			if (hour == Timer[i].hour && min == Timer[i].minute)
			{
				Chat.MessageAll(0,0,NULL,"Moss The Gambler is arrived!");
				this->Spawn();
				this->Opened = TRUE;
			}

			if (hour == Timer[i].closehour && min == Timer[i].closemin)
			{
				Chat.MessageAll(0,0,NULL,"Moss The Gambler is closed!");
				this->Disappear();
				this->Opened = FALSE;
			}
		}
	}
}

BOOL cMoss::GetStatusMoss()
{
	if(Timer)
		return this->Opened;
	else
		return TRUE;
}

void cMoss::Spawn()
{
	Monster.MonsterAddAndSpawn(492,51,0,22,225,22,225,2);
}

void cMoss::Disappear()
{
	for(int iIndex = 0; iIndex < OBJ_MAXMONSTER; iIndex++)
	{
		OBJECTSTRUCT *mObj = (OBJECTSTRUCT*)OBJECT_POINTER(iIndex);
		if (mObj == NULL) return;
		if(mObj->Class == 492)	gObjDel(iIndex);

	}
	int MobCount = *(DWORD *)(MonsterCount);
	MobCount--;
}

BOOL cMoss::BuyItem(int aIndex, unsigned char * aRecv)
{
	OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(aIndex);

	if (gObj->TargetShopNumber != 492) return FALSE;
	
	if (Moss.GetStatusMoss() == FALSE)
	{
		Chat.Message(aIndex,"Moss The Gambler is closed");
		return TRUE;
	}

	if ( this->Delay == true )
	{
		Chat.Message(aIndex,"[Moss The Gambler] Please wait 1.5 second after a previous purchase");
		return TRUE;
	}

	if (gObj->Money < Config.PriceZen)
	{
		Chat.Message(gObj->m_Index,"[Moss The Gambler] You haven't got enough money");
		return TRUE;
	}

	if (AddTab[gObj->m_Index].PC_PlayerPoints < Config.PricePCPoint)
	{
		Chat.Message(gObj->m_Index,"[Moss The Gambler] You haven't got enough PCPoint");
		return TRUE;
	}

	if (gObj->m_wCashPoint < Config.PriceWCoin)
	{
		Chat.Message(gObj->m_Index,"[Moss The Gambler] You haven't got enough WCoin");
		return TRUE;
	}
	srand(GetTickCount());
	/*Узнаем какую группу оружия было выбрано*/
	if (aRecv[3] == 0 )  BuyType = 0;
	if (aRecv[3] == 2 )  { BuyType = rand()%2+1;	if (BuyType == 2) BuyType = 3;	}
	if (aRecv[3] == 4 )  BuyType = 2;
	if (aRecv[3] == 6 )  BuyType = 4;
	if (aRecv[3] == 24 ) BuyType = 5;

	int Index = rand()%OrderItems[BuyType];
	int Type = ITEMGET(BuyType,ItemInfo[BuyType][Index].Index);
	int Level	 = RandValue(OPT_LEVEL);
	int Option1	 = RandValue(OPT_SKILL);
	int Option2	 = RandValue(OPT_LUCK);
	int Option3	 = RandValue(OPT_ADD);
	int NewOption = RandValue(OPT_EXC);
	int SetOption = RandValue(OPT_ANC);

	int X,Y;
	GSItemGetSize(Type,X,Y);

	if (GSCheckInventoryEmptySpace(gObj,Y,X) == 0)
	{
		Chat.Message(gObj->m_Index,"[Moss The Gambler] You inventory is full");
		return TRUE;
	}

	int Dur = ItemGetDurability(Type,Level,NewOption,SetOption);
	ItemSerialCreateSend(gObj->m_Index,236,0,0,Type,Level,Dur,Option1,Option2,Option3,0,NewOption,SetOption);

	Log.ConsoleOutPut(0, c_Red,t_NULL,"[Moss The Gambler] %s Win [%d] %d %d %d %d %d %d",
		gObj->Name,Type,Level,Option1,Option2,Option3,NewOption,SetOption);

	if (NewOption > 0)
		Chat.Message(gObj->m_Index,"[Moss The Gambler] Congratulations, you are very lucky!!!");

	gObj->Money -= Config.PriceZen;
	GCMoneySend(gObj->m_Index,gObj->Money);
	if(Config.PricePCPoint > 0) PCPoint.UpdatePoints(gObj,Config.PricePCPoint,MINUS,PCPOINT);
	if(Config.PriceWCoin > 0)   PCPoint.UpdatePoints(gObj,Config.PriceWCoin  ,MINUS,WCOIN);

	_beginthread(MossBuyDelay,0,0);
	return TRUE;
}

int cMoss::RandValue(int IndexOption)
{
	int rValue;
	srand((unsigned) GetTickCount());
	switch(IndexOption)
	{
	case OPT_LEVEL:
		{
			rValue = rand()%100+1;
			rValue > Config.RandLevel ?	rValue =  0 : rValue =  rand()%(Config.MaxLevel + 1);
		}
		break;
	case OPT_SKILL:
		{
			rValue = rand()%100+1;
			rValue > Config.RandSkill ? rValue =  0 : rValue =  1;
		}
		break;
	case OPT_LUCK:
		{
			rValue = rand()%100+1;
			rValue > Config.RandLuck ? rValue =  0 : rValue =  1;
		}
		break;
	case OPT_ADD:
		{
			rValue = rand()%100+1;
			rValue > Config.RandOptAdd ? rValue =  0 : rValue =  rand()%(Config.MaxOptAdd + 1);
		}
		break;
	case OPT_EXC:
		{
			rValue = rand()%100+1;
			rValue > Config.RandExc ? rValue =  0 : rValue =  Utilits.GenExcOpt(rand()%(Config.MaxExcOpt + 1));
		}
		break;
	case OPT_ANC:
		{
			rValue = rand()%100+1;
			rValue > Config.RandAncient ? rValue =  0 : rValue =  10;
		}
		break;
	}
	return rValue;
}

#endif