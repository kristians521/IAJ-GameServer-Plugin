// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#include "User.h"

#ifndef UTILITS_H
#define UTILITS_H

class cUtilits
{
public:
	void HookThis(DWORD dwMyFuncOffset, DWORD dwJmpOffset);
	void SetNop(DWORD dwOffset, int Size);
	void SetRetn(DWORD dwOffset);
	void SetRRetn(DWORD dwOffset);
	void SetByte(DWORD dwOffset, BYTE btValue);	 

	int gObjIsConnected(int Index);

	int GetPlayerIndex(char *Name);
	int GenExcOpt(int amount);	   
	int gObjZenSingle(LPOBJ gObj,LPOBJ tObj, int dmg, int tot_dmg);
	int GetNumberByPercent(int Proc, int Min, int Max);

	char* GetMapName(int MapId);

	bool IsBadFileLine(char *FileLine, int *Flag);
};
extern cUtilits Utilits;
#endif