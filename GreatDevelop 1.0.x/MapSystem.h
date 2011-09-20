// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # GreatDevelop 1.0.2								# //
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
		int MapID;
		int Zen;
		int Drop;
		int Exp;
		int PVP;		
	} Config[255];

};

extern cMapSystem MapSystem;

#endif