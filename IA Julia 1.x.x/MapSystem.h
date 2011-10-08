// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # GreatDevelop 1.1.0 beta						# //
// ================================================== //
#ifndef MAPSYSTEM_H
#define MAPSYSTEM_H

class cMapSystem
{
public:
	cMapSystem();
	~cMapSystem();
	void Load();

	int Enabled;
	struct Conf	  
	{
		int Zen;
		int Drop;
		int Exp;
		int PVP;		
	} Maps[255];

};

extern cMapSystem MapSystem;

#endif