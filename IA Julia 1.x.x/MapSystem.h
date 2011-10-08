// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
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