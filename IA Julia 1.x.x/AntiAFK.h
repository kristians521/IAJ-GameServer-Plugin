// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#ifndef AntiAFK_H
#define AntiAFK_H

#include "User.h"
class cAntiAFK
{
public:
	void Tick(LPOBJ gObj);
	void Load();

private:
	struct sConfig
	{
		int Enabled;
		int Time;
		int Warnings;
	}Config;

	int ArrayMap[64];
	int ReWarning;
};

extern cAntiAFK AntiAFK;

#endif