// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # GreatDevelop 1.1.0 beta						# //
// ================================================== // 

#ifndef MoveReq_H
#define MoveReq_H

class cMoveReq
{
public:
	cMoveReq();
	~cMoveReq(); 

	void MoveReqLoad();
	char* GetMapName(int MapId);

	int n[5];
	struct sMoveReq
	{
		int Index, Zen, Level, Gate, VIP;
		char MapName1[50], MapName2[50];
	}MoveReqInfo[255];
	int Count;
};
extern cMoveReq MoveReq;
#endif