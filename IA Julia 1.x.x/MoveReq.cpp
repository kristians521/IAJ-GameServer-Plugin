// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#include "StdAfx.h"
#include "MoveReq.h"
#include "User.h" 
cMoveReq MoveReq;

cMoveReq::cMoveReq()
{

}

cMoveReq::~cMoveReq()
{

}

void cMoveReq::Load()
{
	FILE *fp;
	BOOL bRead = FALSE;
	DWORD dwArgv = 0;
	char sLineTxt[255] = {0};
	int MoveReqCount = 1;

	fp = fopen("..\\IA Julia\\MoveReq.txt","r");

	if(!fp)
	{
		MessageBoxA(NULL, "Cant Find MoveReq.txt!", "Error!", MB_OK);
		::ExitProcess(0);
	}
	
	rewind(fp);
	int leestatus=-1;
	
	while(fgets(sLineTxt, 255, fp) != NULL)
	{
		if(sLineTxt[0] == '/')continue;
		if(sLineTxt[0] == ';')continue; 

		char Name[50], Name2[50];

		sscanf(sLineTxt, "%d \"%50[^\"]\" \"%50[^\"]\" %d %d %d %d", &n[0], &Name, &Name2, &n[1], &n[2], &n[3], &n[4]); 

		MoveReqInfo[MoveReqCount].Index	= n[0];				 
		strcpy(MoveReqInfo[MoveReqCount].MapName1, Name);
		strcpy(MoveReqInfo[MoveReqCount].MapName2, Name2);
		MoveReqInfo[MoveReqCount].Zen	= n[1];
		MoveReqInfo[MoveReqCount].Level	= n[2];
		MoveReqInfo[MoveReqCount].Gate	= n[3];
		MoveReqInfo[MoveReqCount].VIP	= n[4];
		MoveReqCount++;
	}

	Count = MoveReqCount;
	rewind(fp);
	fclose(fp);
	
	return;
}
