// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#ifndef PLAYERSYSTEM_H
#define PLAYERSYSTEM_H

#include "User.h"

class cPlayerSystem
{
public:
	enum eType {Zen, Drop, Exp};

	void Load();
	int GetBonus(LPOBJ gObj, eType Type);
	int Enabled;

	struct Conf	  
	{
		int Class[6];
		int First;
		int Second;
		int Third;
		int Zen;
		int Drop;
		int Exp;
	} Config[255];
	int ConfigNum;
};

extern cPlayerSystem PlayerSystem;

#endif