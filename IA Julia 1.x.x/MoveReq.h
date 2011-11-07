// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#ifndef MoveReq_H
#define MoveReq_H

class cMoveReq
{
public:
	cMoveReq();
	~cMoveReq(); 

	void MoveReqLoad();

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