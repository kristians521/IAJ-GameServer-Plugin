// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#include "PCPoint.h"
#include "Logger.h"
#include "Configs.h"
#include <Windows.h>
#include "ChatCommands.h"
#include "Query.h"

cPCPoint PCPoint;

cPCPoint::cPCPoint() {}
cPCPoint::~cPCPoint() {}

void cPCPoint::LoadIniConfigs()
{
	Config.MaximumWCPoints		= Configs.GetInt(0, 32000,					10000,	"WCoins",		"MaximumWCPoints",			IAJuliaPcPoints);

	Config.Enabled				= Configs.GetInt(0, 1	,					1,		"PCPoints",		"Enabled",					IAJuliaPcPoints);
	if (!Config.Enabled) return;

	Config.MaximumPCPoints		= Configs.GetInt(0, 32000,					10000,	"PCPoints",		"MaximumPCPoints",			IAJuliaPcPoints);
	Config.AddPCPointsSec		= Configs.GetInt(0, 6000000,					3600,	"PCPoints",		"AddPCPointsSec",			IAJuliaPcPoints);
	Config.AddPCPointsCount	= Configs.GetInt(0, Config.MaximumPCPoints,	10,		"PCPoints",		"AddPCPointsCount",			IAJuliaPcPoints);	
	this->LoadConfigs();
}

void cPCPoint::LoadConfigs()
{
	this->ChekingCallInitItemsInShop = false;

	FILE *file;
	file = fopen(IAJuliaPcPoints,"r");

	if (file == NULL)
	{
		Log.ConsoleOutPut(1,c_Red,t_Error,"[PC Point] Can`t Find %s",IAJuliaPcPoints);
		return;
	}

	char szBuf[256];
	int shBuff = -1;
	short Counter = 0;

	while (!feof(file))
	{
		fgets(szBuf,256,file);

		if (!strncmp(szBuf,"//",strlen("//"))) continue;

		if( !strncmp(szBuf,"end",strlen("end")))
		{
			shBuff = -1;
			continue;
		}

		if(shBuff == -1 && strlen(szBuf) < 3)
		{
			sscanf(szBuf,"%d",&shBuff);
			Counter = 0;
			continue;
		}
		if (shBuff == 0)
		{
			sscanf(szBuf,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d",&PCShop[Counter].Index,&PCShop[Counter].ID,
				&PCShop[Counter].Level,&PCShop[Counter].Opt,&PCShop[Counter].Luck,&PCShop[Counter].Skill,
				&PCShop[Counter].Exc,&PCShop[Counter].Ancient,&PCShop[Counter].NewOpt1,&PCShop[Counter].NewOpt2,
				&PCShop[Counter].NewOpt3,&PCShop[Counter].NewOpt4,&PCShop[Counter].NewOpt5,&PCShop[Counter].Cost);
			if (Counter  >= 119) continue;
			AmountRecords[0] = ++Counter;
		}
		if (shBuff == 1)
		{
			sscanf(szBuf,"%d %d",&PCMonsters[Counter].MobID,&PCMonsters[Counter].PCPoints);
			AmountRecords[1] = ++Counter;
		}
		if (shBuff == 2)
		{
			sscanf(szBuf,"%d %d",&WCoinMonsters[Counter].MobID,&WCoinMonsters[Counter].WCoins);
			AmountRecords[2] = ++Counter;
		}
	}
	fclose(file);
	Log.ConsoleOutPut(1,c_Cyan,t_PCPOINT,"[PC Point] PC Shop Item Loaded [%d]",AmountRecords[0]);
	Log.ConsoleOutPut(1,c_Cyan,t_PCPOINT,"[PC Point] Total Monster Loaded [%d]",AmountRecords[1]);
	Log.ConsoleOutPut(1,c_Cyan,t_PCPOINT,"[WCoin] Total Monster Loaded [%d]",AmountRecords[2]);
}

void cPCPoint::Tick(LPOBJ gObj)
{
	if (PCPoint.Config.Enabled && PCPoint.Config.AddPCPointsSec > 0)
	{
		AddTab[gObj->m_Index].PC_OnlineTimer++;
		if (AddTab[gObj->m_Index].PC_OnlineTimer == PCPoint.Config.AddPCPointsSec)
		{
			AddTab[gObj->m_Index].PC_OnlineTimer = 0;
			PCPoint.UpdatePoints(gObj,PCPoint.Config.AddPCPointsCount,PLUS,PCPOINT);
			Chat.Message(gObj->m_Index,"[PointShop] You earned %d Points for being online!", PCPoint.Config.AddPCPointsCount);
			Chat.Message(gObj->m_Index,"[PointShop] You have been online %d Hours!", AddTab[gObj->m_Index].ON_Hour);
		}
	} 
}
void cPCPoint::CreatePacketShop()
{
	int PacketSize = 0;
	int PacketFlag = 0;
	int Size       = 0;
	BYTE Packet1[11] = {0xC1, 0x05, 0xD0, 0x06, 0x00, 0xC2, 0x00, 0x36, 0x31, HIBYTE(AmountRecords[0]), LOBYTE(AmountRecords[0])};
	BYTE Packet2[4680];
#pragma warning(disable: 4244)
	for (int i = 0; i < AmountRecords[0]; i++)
	{
		BYTE ItemInfo[13];
		ItemInfo[0] = PCShop[i].SlotX;
		ItemInfo[1] = PCShop[i].Reward & 0xFF;
		ItemInfo[2] = (PCShop[i].Level * 8) | (PCShop[i].Skill * 128) | (PCShop[i].Luck * 4) | (PCShop[i].Opt & 3);
		ItemInfo[3] = PCShop[i].Dur;
		ItemInfo[4] = (PCShop[i].Reward & 0x100) >> 1;
		if ( PCShop[i].Opt > 3 ) ItemInfo[4]  |= 0x40;
		ItemInfo[4] |= PCShop[i].Exc;
		ItemInfo[5] = PCShop[i].Ancient;
		ItemInfo[6] = (PCShop[i].Reward & 0x1E00 ) >> 5;
		ItemInfo[7] = PCShop[i].NewOpt1;
		ItemInfo[8] = PCShop[i].NewOpt2;
		ItemInfo[9] = PCShop[i].NewOpt3;
		ItemInfo[10] = PCShop[i].NewOpt4;
		ItemInfo[11] = PCShop[i].NewOpt5;
		ItemInfo[12] = PCShop[i].Cost;

		PacketSize = (sizeof(ItemInfo) * (i + 1));
		memcpy(&Packet2[PacketFlag], ItemInfo, PacketSize);
		PacketFlag = PacketSize;
	}
	Size = (sizeof(Packet1) + PacketSize);
	memcpy(&PCPointPacket, Packet1, sizeof(Packet1));
	memcpy(&PCPointPacket[sizeof(Packet1)], Packet2, sizeof(Packet1) + PacketSize);
	PCPointPacket[6]    = HIBYTE(Size);
	PCPointPacket[7]    = LOBYTE(Size);
	this->PacketSizes = (sizeof(Packet1) + PacketSize);
}

void cPCPoint::InitItemShop()
{
	this->ChekingCallInitItemsInShop = true;

	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 8; j++)
			this->ArrayFiledItems[i][j] = false;

	for (int k = 0; k <  AmountRecords[0]; k++)
	{
		PCShop[k].Reward = PCShop[k].Index * 512 + PCShop[k].ID;
		PCShop[k].Dur = ItemGetDurability(PCShop[k].Reward,PCShop[k].Level,PCShop[k].Exc,0);
		GSItemGetSize(PCShop[k].Reward,PCShop[k].X,PCShop[k].Y);
		PCShop[k].SlotX = this->gObjCalcItems(PCShop[k].X,PCShop[k].Y);
	}
	this->CreatePacketShop();
}

void cPCPoint::OpenShop(int Index)
{
	if (this->ChekingCallInitItemsInShop == false)
		this->InitItemShop();
	DataSend(Index,this->PCPointPacket,this->PacketSizes);
	Chat.Message(Index,"");
}

int cPCPoint::gObjCalcItems(int X, int Y)
{
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 8; j++)
			if (ArrayFiledItems[i][j] == false)
			{
				bool bFlag = true;

				for (short kX = 0; kX < X; kX++)
					for(short kY = 0; kY < Y; kY++)
						if ( i + kY > 14 || j + kX > 7 || ArrayFiledItems[i+kY][j+kX])
							bFlag = false;

				if (bFlag)
				{
					for (short kX = 0; kX < X; kX++)
						for(short kY = 0; kY < Y; kY++)
							ArrayFiledItems[i+kY][j+kX] = true;
					return i*8 + j;
				}
			}
			return -1;
}

void cPCPoint::BuyItem(int Index,int Position)
{
	int IndexItem = -1;

	for(int i = 0; i < AmountRecords[0]; i++)
		if (Position == PCShop[i].SlotX ) IndexItem = i;

	OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);

	if(IndexItem != -1)
	{
		if (PCShop[IndexItem].Cost <= AddTab[gObj->m_Index].PC_PlayerPoints)
		{
			if ( GSCheckInventoryEmptySpace(gObj,PCShop[IndexItem].Y,PCShop[IndexItem].X) != 0 )
			{
				ItemSerialCreateSend(gObj->m_Index,236,0,0,PCShop[IndexItem].Reward,PCShop[IndexItem].Level,PCShop[IndexItem].Dur,
					PCShop[IndexItem].Skill,PCShop[IndexItem].Luck,PCShop[IndexItem].Opt,-1,PCShop[IndexItem].Exc,PCShop[IndexItem].Ancient);

				Log.ConsoleOutPut(1,c_Cyan,t_PCPOINT,"[PointShop] Character [%s] Buy Item [%d %d] Cost [%d]",
					gObj->Name,PCShop[IndexItem].Index,PCShop[IndexItem].ID,PCShop[IndexItem].Cost);
				this->UpdatePoints(gObj,PCShop[IndexItem].Cost,MINUS,PCPOINT);
			}
			else
				Chat.Message(gObj->m_Index,"[PointShop] Your inventory is full !");
		}
		else
			Chat.Message(gObj->m_Index,"[PointShop] Can`t buy, need %d more PCPoints!", PCShop[IndexItem].Cost - AddTab[gObj->m_Index].PC_PlayerPoints);
	}
}

void cPCPoint::InitPCPointForPlayer(LPOBJ gObj)
{
	MuOnlineQuery.ExecQuery("SELECT PCPoint FROM Character WHERE Name = '%s'", gObj->Name);
		MuOnlineQuery.Fetch();
		int AmountPoints = MuOnlineQuery.GetAsInteger("PCPoint");
		MuOnlineQuery.Close();

	if (AmountPoints > Config.MaximumPCPoints) AmountPoints = Config.MaximumPCPoints;
	AddTab[gObj->m_Index].PC_PlayerPoints = AmountPoints;

	BYTE Packet[8] = {0xC1, 0x08 , 0xD0 , 0x04 , LOBYTE(AmountPoints), HIBYTE(AmountPoints),
		LOBYTE(Config.MaximumPCPoints), HIBYTE(Config.MaximumPCPoints)};

	DataSend(gObj->m_Index, (PBYTE)Packet, Packet[1]);
}

void cPCPoint::UpdatePoints(LPOBJ gObj,int CountPoints,eModeUpdate Mode,eTypePoint Type)
{
	char ModeType;

	if (Type == PCPOINT)		
	{
		if(gObj->Connected < PLAYER_PLAYING)
		{
			Log.ConsoleOutPut(0, c_Yellow, t_SQL, "[SQL] PcPoint UPDATE Error (Player not playing!)");
			return;
		}
		if (Mode == PLUS && 
			AddTab[gObj->m_Index].PC_PlayerPoints + CountPoints > PCPoint.Config.MaximumPCPoints)
		{
			Chat.Message(gObj->m_Index,"[PCPoint] You have maximum PCPoints");
			return;
		}
	}
	if (Type ==  WCOIN ) 
	{
		if(gObj->Connected < PLAYER_LOGGED)
		{
			Log.ConsoleOutPut(0, c_Yellow, t_SQL, "[SQL] PcPoint UPDATE Error (Player not logged!)");
			return;
		}
		if (Mode == PLUS && 
			gObj->m_wCashPoint + CountPoints > PCPoint.Config.MaximumWCPoints)
		{
			Chat.Message(gObj->m_Index,"[WCoins] You have maximum WCoins");
			return;
		}
	}

	switch(Mode)
	{
	case PLUS:
		ModeType = '+';
		break;
	case MINUS:
		ModeType = '-';
		break;
	}

	if (Type == PCPOINT)
	{

		MuOnlineQuery.ExecQuery("UPDATE Character SET PCPoint = (PCPoint %c %d) WHERE Name = '%s'", ModeType, CountPoints, gObj->Name);
		MuOnlineQuery.Fetch();
		MuOnlineQuery.Close();

		MuOnlineQuery.ExecQuery("SELECT PCPoint FROM Character WHERE Name = '%s'", gObj->Name);
		MuOnlineQuery.Fetch();
		AddTab[gObj->m_Index].PC_PlayerPoints = MuOnlineQuery.GetAsInteger("PCPoint");
		MuOnlineQuery.Close();

		BYTE Packet[8] = {0xC1, 0x08 , 0xD0 , 0x04 , LOBYTE(AddTab[gObj->m_Index].PC_PlayerPoints), HIBYTE(AddTab[gObj->m_Index].PC_PlayerPoints),
			LOBYTE(Config.MaximumPCPoints), HIBYTE(Config.MaximumPCPoints)};
		DataSend(gObj->m_Index, (PBYTE)Packet, Packet[1]);
	}
	if (Type == WCOIN)
	{
		Me_MuOnlineQuery.ExecQuery("UPDATE MEMB_INFO SET cspoints = (cspoints %c %d) WHERE memb___id = '%s'", ModeType, CountPoints, gObj->AccountID);
		Me_MuOnlineQuery.Fetch();
		Me_MuOnlineQuery.Close();

		Me_MuOnlineQuery.ExecQuery("SELECT cspoints FROM MEMB_INFO WHERE memb___id = '%s'", gObj->AccountID);
		Me_MuOnlineQuery.Fetch();
		gObj->m_wCashPoint = Me_MuOnlineQuery.GetAsInteger("cspoints");
		Me_MuOnlineQuery.Close();
	}
}

void cPCPoint::RewardsPointsKillMob(LPOBJ gObj,LPOBJ mObj,eTypePoint Type)
{
	switch (Type)
	{
	case PCPOINT:
		{
			for (int i = 0; i < AmountRecords[1]; i++)
				if ( PCMonsters[i].MobID == mObj->Class )
				{
					this->UpdatePoints(gObj,PCMonsters[i].PCPoints,PLUS,Type);
					Chat.Message(gObj->m_Index,"[PCPoint] You earned %d PCPoints",PCMonsters[i].PCPoints);
					return;
				}
			return;
		}
	case WCOIN:
		{
			for (int i = 0; i < AmountRecords[2]; i++)
				if ( WCoinMonsters[i].MobID == mObj->Class )
				{
					this->UpdatePoints(gObj,WCoinMonsters[i].WCoins,PLUS,Type);
					Chat.Message(gObj->m_Index,"[WCoin] You earned %d WCoins",WCoinMonsters[i].WCoins);
					return;
				}
			return;
		}
	}
}
