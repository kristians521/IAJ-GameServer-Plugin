// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # GreatDevelop 1.0.2								# //
// ================================================== //

#include "StdAfx.h"
#include "MapSystem.h"
#include "Configs.h"
#include "Logger.h"

cMapSystem MapSystem;


cMapSystem::cMapSystem()
{
}

cMapSystem::~cMapSystem()
{
}

void cMapSystem::Load()
{
   	FILE *fp;
	fp = fopen(GreatDevelopMapSystem, "r");
	rewind(fp);

	if(fp == NULL)
	{
		Log.ConsoleOutPut(1, c_Cyan, t_Default, "[Map System] System not active. Config file not found: %s", GreatDevelopMapSystem);
		return;
	}

	int Type = -1;	 
	char fpLine[255];

	while(!feof(fp))
	{
		fgets(fpLine, 255, fp);

		// Skip comments & end's
		if(fpLine[0] == '/' || strcmp(fpLine, "\n") == 0) continue;

		// Config Type
		if (strcmp(fpLine, "1") == 0 || strcmp(fpLine, "1\n") == 0) { Type = 1; continue; }
					   
		if(Type == 1)
		{				
			if(fpLine[0] == 'e' && fpLine[1] == 'n' && fpLine[2] == 'd')
			{
				Type = 0;
				continue;
			}

//#TODO Неведома хрень переписать
			int temp[5];
			sscanf(fpLine, "%d %d %d %d %d", &temp[0], &temp[1], &temp[2], &temp[3], &temp[4]);
			Config[temp[0]].MapID = temp[0];
			Config[temp[0]].Zen = temp[1];
			Config[temp[0]].Drop = temp[2];
			Config[temp[0]].Exp = temp[3];
			Config[temp[0]].PVP = temp[4];
		}
	}
}