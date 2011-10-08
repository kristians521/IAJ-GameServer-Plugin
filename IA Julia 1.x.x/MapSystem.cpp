// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#include "StdAfx.h"
#include "MapSystem.h"
#include "Configs.h"
#include "Logger.h"

cMapSystem MapSystem;


cMapSystem::cMapSystem()
{
	for(int i = 0; i < 255; i++)
	{
		Maps[i].Drop	= 0;
		Maps[i].Exp		= 0;
		Maps[i].PVP		= 0;
		Maps[i].Zen		= 0;
	}
}

cMapSystem::~cMapSystem()
{
}

void cMapSystem::Load()
{
   	FILE *fp;
	fp = fopen(IAJuliaMapSystem, "r");

	if(fp == NULL)
	{
		Log.ConsoleOutPut(1, c_Cyan, t_Default, "[Map System] System not active. Config file not found: %s", IAJuliaMapSystem);
		return;
	}
	rewind(fp);

	int Type = -1;	 
	char fpLine[255];

	while(!feof(fp))
	{
		fgets(fpLine, 255, fp);

		// Skip comments & end's
		if(fpLine[0] == '/' || strcmp(fpLine, "\n") == 0) continue;

		// Config Type
		if ((strcmp(fpLine, "1") == 0 || strcmp(fpLine, "1\n") == 0) && Type != 1) { Type = 1; continue; }
					   
		if(Type == 1)
		{				
			if(fpLine[0] == 'e' && fpLine[1] == 'n' && fpLine[2] == 'd')
			{
				Type = 0;
				continue;
			}

			int temp[5];
			sscanf(fpLine, "%d %d %d %d %d", &temp[0], &temp[1], &temp[2], &temp[3], &temp[4]);
			Maps[temp[0]].Zen		= temp[1];
			Maps[temp[0]].Drop		= temp[2];
			Maps[temp[0]].Exp		= temp[3];
			Maps[temp[0]].PVP		= temp[4];
		}
	}
}