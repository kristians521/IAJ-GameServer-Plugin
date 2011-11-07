// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#include "StdAfx.h"
#include "Utilits.h"
#include "User.h"
#include "GMSystem.h"
cUtilits Utilits;

void cUtilits::HookThis(DWORD dwMyFuncOffset, DWORD dwJmpOffset)
{
	*(DWORD*)(dwJmpOffset + 1) = dwMyFuncOffset-(dwJmpOffset+5);
}

void cUtilits::SetNop(DWORD dwOffset, int Size)
{
	for(int n=0; n < Size; n++)
		*(BYTE*)(dwOffset+n) = 0x90;
}

void cUtilits::SetRetn(DWORD dwOffset)
{
	*(BYTE*)(dwOffset) = 0xC3;
}

void cUtilits::SetRRetn(DWORD dwOffset)
{
	*(BYTE*)(dwOffset)=0xC3;
	*(BYTE*)(dwOffset+1)=0x90;
	*(BYTE*)(dwOffset+2)=0x90;
	*(BYTE*)(dwOffset+3)=0x90;
	*(BYTE*)(dwOffset+4)=0x90;
}

void cUtilits::SetByte(DWORD dwOffset, BYTE btValue)
{
	*(BYTE*)(dwOffset) = btValue;
}

int cUtilits::GetPlayerIndex(char *Name)
{			 
	for(int i = OBJECT_MIN; i <= OBJECT_MAX; i++)
	{  	 
		OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(i);	 
		if(gObj->Connected < PLAYER_PLAYING) continue; 	   
		if(!strcmp(gObj->Name, Name)) return i;
	}
	return -1;
}

int cUtilits::gObjIsConnected(int Index)
{
	OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);  
	if(gObj->Connected >= PLAYER_PLAYING) 
		return 1;     
	return 0;
}

int cUtilits::GetNumberByPercent(int Proc, int Min, int Max)
{
	int Random = rand()%100;
	int Number;

	if(Proc == 0 || Max == Min)
		return Min;
	
	if(Random <= Proc)
		Number = Max;
	else
	{	
		// TODO: work on it
		//if(Proc > 50)
		//	Proc -= (proc%50)/(MaxOption-MinOption+1);
		//else
		//	Proc += 50/(MaxOption-MinOption) - (proc%50)/(MaxOption-MinOption+1);
		Number = GetNumberByPercent(Proc, Min, Max-1);
	}
	return Number;
}

int cUtilits::GenExcOpt(int amount)
{
	// User input errors
	if (amount > 6) amount = 6;
	if (amount < 1) amount = 1;

	int opt_db[6]  = {1, 2, 4, 8, 16, 32};
	int exc = 0;

	std::random_shuffle(opt_db, opt_db + 6);

	for(int n=0; n < amount; n++)
		exc += opt_db[n];

	return exc;
}

int cUtilits::gObjZenSingle(OBJECTSTRUCT* gObj,OBJECTSTRUCT* tObj, int dmg, int tot_dmg)
{		  
	int exp;
	int maxexp = 0;
	int level = (tObj->Level+25)*tObj->Level/3;

	if((tObj->Level+10) < gObj->Level)
	{
		level = level*(tObj->Level+10)/gObj->Level;
	}

	if(tObj->Level >= 65)
	{
		level = level + (tObj->Level-64)*(tObj->Level/4);
	}

	if(level > 0)
	{
		maxexp = level/2;
	}
	else
	{
		level = 0;
	}

	if(maxexp < 1)
	{
		exp = level;
	}
	else
	{
		exp = level + rand()%maxexp;
	}

	exp = dmg * exp / tot_dmg;

	exp *= 10000;

	return exp;
}


char* cUtilits::GetMapName(int MapId)
{
	switch (MapId)
	{
	case -1:
		return "All Maps";
	case 0:
		return "Lorencia";
	case 1:
		return "Dungeon";
	case 2:
		return "Devais";
	case 3:
		return "Noria";
	case 4:
		return "Lost Tower";
	case 5:
		return "Null";
	case 6:
		return "Arena";
	case 7:
		return "Atlans";
	case 8:
		return "Tarkan";
	case 9:
		return "Devil Square";
	case 10:
		return "Icarus";
	case 30:
		return "Valley of Loren";
	case 31:
		return "Land of Trial";
	case 32:
		return "Devil Square";
	case 33:
		return "Aida";
	case 34:
		return "Crywolf";
	case 35:
		return "Null";
	case 36:
		return "Kalima";
	case 37:
		return "Kantru1";
	case 38:
		return "Kantru2";
	case 39:
		return "Kantru3";
	case 40:
		return "Silent";
	case 41:
		return "Barracks";
	case 42:
		return "Refuge";
	case 43:
		return "Null";
	case 44:
		return "Null";
	case 51:
		return "Elbeland";
	case 52:
		return "Blood Castle";
	case 53:
		return "Chaos Castle";
	case 56:
		return "Swamp of Calmness";
	case 57:
		return "Raklion";
	case 58:
		return "Raklion Boss";
	case 62:
		return "Santa Town";
	case 63:
		return "Vulcanus";
	case 64:
		return "Coliseum";
	}
	if (MapId >= 11 && MapId <= 17)
		return "Blood Castle";

	if (MapId >= 18 && MapId <= 23)
		return "Chaos Castle";

	if (MapId >= 24 && MapId <= 29)
		return "Kalima";

	if (MapId >= 45 && MapId <= 50)
		return "Illusion Temple";

	return "Unknown";
}

bool cUtilits::IsBadFileLine(char *FileLine, int *Flag)
{
	if(*Flag == 0)
	{
		if(isdigit(FileLine[0]))
		{
			*Flag = FileLine[0] - 48;
			return true;
		}
	}
	if(!strncmp(FileLine, "end", 3))
	{
		*Flag = 0;
		return true;
	}

	if(FileLine[0] == '/' || FileLine[0] == '\n')
		return true;

	for(UINT i = 0; i < strlen(FileLine); i++)
	{
		if(isalnum(FileLine[i]))
			return false;
	}
	return true;
}