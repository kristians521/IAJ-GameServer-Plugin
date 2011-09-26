#include "StdAfx.h"
#ifdef _GS
#include "User.h" 
#include "Logger.h"
#include "Configs.h"
#include "WZEventDrop.h"
#include "Utilits.h"
#include "Configs.h"

cDropEvent DropEvent;

// Handlers

void fHandlerBC(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level, BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption)
{
	if ((rand() % 100 + 1) <= DropEvent.dropCountBC.countRate)
	{
		for (int i=0; i < DropEvent.dropCountBC.countItems; i++)
		{
			DropEvent.handlerBC(aIndex, MapNumber, x, y, type, level, dur , Op1, Op2, Op3, LootIndex, NewOption, SetOption);
		}
	}
	else {
		DropEvent.handlerBC(aIndex, MapNumber, x, y, type, level, dur , Op1, Op2, Op3, LootIndex, NewOption, SetOption);
	}
}

void fHandlerIT(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level, BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption)
{
	if ((rand() % 100 + 1) <= DropEvent.dropCountIT.countRate)
	{
		for (int i=0; i < DropEvent.dropCountIT.countItems; i++)
		{
			DropEvent.handlerIT(aIndex, MapNumber, x, y, type, level, dur , Op1, Op2, Op3, LootIndex, NewOption, SetOption);
		}
	}
	else {
		DropEvent.handlerIT(aIndex, MapNumber, x, y, type, level, dur , Op1, Op2, Op3, LootIndex, NewOption, SetOption);
	}
}

void fHandlerWW(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level, BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption)
{
	if ((rand() % 100 + 1) <= DropEvent.dropCountWW.countRate)
	{
		for (int i=0; i < DropEvent.dropCountWW.countItems; i++)
		{
			DropEvent.handlerWW(aIndex, MapNumber, x, y, type, level, dur , Op1, Op2, Op3, LootIndex, NewOption, SetOption);
		}
	}
	else {
		DropEvent.handlerWW(aIndex, MapNumber, x, y, type, level, dur , Op1, Op2, Op3, LootIndex, NewOption, SetOption);
	}
}

void fHandlerCC(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level, BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption)
{
	if ((rand() % 100 + 1) <= DropEvent.dropCountCC.countRate)
	{
		for (int i=0; i < DropEvent.dropCountCC.countItems; i++)
		{
			DropEvent.handlerCC(aIndex, MapNumber, x, y, type, level, dur , Op1, Op2, Op3, LootIndex, NewOption, SetOption);
		}
	}
	else {
		DropEvent.handlerCC(aIndex, MapNumber, x, y, type, level, dur , Op1, Op2, Op3, LootIndex, NewOption, SetOption);
	}
}

void cDropEvent::asmHook()
{
	//BC Drop Replace Hook for GS
	BYTE BCEventReplaceJMP[] = {0xEB, 0x60, 0x90, 0x90, 0x90};
	memcpy((int*)0x005881FC, BCEventReplaceJMP, sizeof(BCEventReplaceJMP));
	BYTE BCEventReplaceCall[] = {0xFF, 0x15, 0x30, 0x00, 0x67, 0x00};
	memcpy((int*)0x0058825E , BCEventReplaceCall, sizeof(BCEventReplaceCall));
	BYTE BCEventReplaceJMP2[] = {0xEB, 0x9B};
	memcpy((int*)0x00588264 , BCEventReplaceJMP2, sizeof(BCEventReplaceJMP2));

	*(unsigned int*)0x00670030 = (unsigned int)fHandlerBC;

	//IT Drop Replace Hook for GS
	BYTE ITEventReplaceJMP[] = {0xEB, 0x15, 0x90, 0x90, 0x90};
	memcpy((int*)0x005E678C, ITEventReplaceJMP, sizeof(ITEventReplaceJMP));
	BYTE ITEventReplaceCall[] = {0xFF, 0x15, 0x40, 0x00, 0x67, 0x00};
	memcpy((int*)0x005E67A3, ITEventReplaceCall, sizeof(ITEventReplaceCall));
	BYTE ITEventReplaceJMP2[] = {0xEB, 0xE6};
	memcpy((int*)0x005E67A9, ITEventReplaceJMP2, sizeof(ITEventReplaceJMP2));
	
	*(unsigned int*)0x00670040 = (unsigned int)fHandlerIT;
	
	//WW Drop Replace Hook for GS
	BYTE WWEventReplaceJMP[] = {0xE9, 0xF1, 0x01, 0x00, 0x00};
	memcpy((int*)0x0048351F, WWEventReplaceJMP, sizeof(WWEventReplaceJMP));
	BYTE WWEventReplaceCall[] = {0xFF, 0x15, 0x60, 0x00, 0x67, 0x00};
	memcpy((int*)0x00483715, WWEventReplaceCall, sizeof(WWEventReplaceCall));
	BYTE WWEventReplaceJMP2[] = {0xE9, 0x04, 0xFE, 0xFF, 0xFF};
	memcpy((int*)0x0048371B, WWEventReplaceJMP2, sizeof(WWEventReplaceJMP2));
	
	*(unsigned int*)0x00670060 = (unsigned int)fHandlerWW;

	//CC Drop Replace Hook for GS
	BYTE CCEventReplaceJMP[] = {0xEB, 0x3F, 0x90, 0x90, 0x90};
	memcpy((int*)0x00597F97, CCEventReplaceJMP, sizeof(CCEventReplaceJMP));
	BYTE CCEventReplaceCall[] = {0xFF, 0x15, 0x70, 0x00, 0x67, 0x00};
	memcpy((int*)0x00597FD8, CCEventReplaceCall, sizeof(CCEventReplaceCall));
	BYTE CCEventReplaceJMP2[] = {0xEB, 0xBC};
	memcpy((int*)0x00597FDE, CCEventReplaceJMP2, sizeof(CCEventReplaceJMP2));

	*(unsigned int*)0x00670070 = (unsigned int)fHandlerCC;
}

void cDropEvent::Init()
{
	// Map code => BC code
	this->mapsBC[238] = 1; //map:11
	this->mapsBC[239] = 2; //map:12
	this->mapsBC[240] = 3; //map:13
	this->mapsBC[241] = 4; //map:14
	this->mapsBC[242] = 5; //map:15
	this->mapsBC[243] = 6; //map:16
	this->mapsBC[244] = 7; //map:17
	this->mapsBC[245] = 8; //map:52

	// Map code => IT code
	this->mapsIT[45] = 1; //map:45
	this->mapsIT[46] = 2; //map:46
	this->mapsIT[47] = 3; //map:47
	this->mapsIT[48] = 4; //map:48
	this->mapsIT[49] = 5; //map:49
	this->mapsIT[50] = 6; //map:50

	// Map code => CC code
	this->mapsCC[18] = 1; //map:18
	this->mapsCC[19] = 2; //map:19
	this->mapsCC[20] = 3; //map:20
	this->mapsCC[21] = 4; //map:21
	this->mapsCC[22] = 5; //map:22
	this->mapsCC[23] = 6; //map:23
	this->mapsCC[53] = 7; //map:53

	this->updateBC();
	this->updateIT();
	this->updateWW();
	this->updateCC();
	this->asmHook();
}

void cDropEvent::dropItem(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int LootIndex, s_DropEventItem&i)
{
	// User input fix's
	if (i.minLvl > i.maxLvl) i.minLvl = i.maxLvl;
	if (i.minOpt > i.maxOpt) i.minOpt = i.maxOpt;
	if (i.minExc > i.maxExc) i.minExc = i.maxExc;

	// Drop procedure
	int i_item = i.iGroup * 512 + i.iId;
	int i_lvl = (i.minLvl == i.maxLvl) ? i.minLvl : i.minLvl + rand() % (i.maxLvl - i.minLvl + 1);
	int i_skill = ((rand() % 100 + 1) <= i.skill) ? 1 : 0;
	int i_luck = ((rand() % 100 + 1) <= i.luck) ? 1 : 0;
	int i_opt = (i.minOpt == i.maxOpt) ? i.minOpt : i.minOpt + rand() % (i.maxOpt - i.minOpt + 1);

	// Exc randomizer
	int i_exc = 0;

	if ((rand() % 100 + 1) <= i.exc)
	{
		i_exc = Utilits.GenExcOpt((i.minExc == i.maxExc) ? i.minExc : i.minExc + rand() % (i.maxExc - i.minExc + 1));
	}

	// Anc randomizer
	int i_anc_type = (i.ancType == 5 || i.ancType == 10) ? i.ancType : 0;
	int i_anc = ((rand() % 100 + 1) <= i.anc) ? i_anc_type : 0;

	//Ïמ טהוט LootIndex = aIndex
	ItemSerialCreateSend(aIndex, MapNumber, x, y, i_item, i_lvl, i.dur, i_skill, i_luck, i_opt, LootIndex, i_exc, i_anc);
}

void cDropEvent::updateBC()
{
	// Drop Count
	dropCountBC.countRate = Config.GetInt(0, 100, 0, "DropCount", "DropCountRate",GreatDevelopEventDropBC);
	dropCountBC.countItems = Config.GetInt(1, 10, 1, "DropCount", "DropCountItems",GreatDevelopEventDropBC);

	// Drop List

	FILE* fp;
	fp = fopen(GreatDevelopEventDropBC, "r");
	rewind(fp);

	if(fp == NULL)
	{
		MessageBox(NULL, "EventDropBC.txt not found", "Error", MB_OK);
		::ExitProcess(0);
	}

	// Clear all BC drop confs
	for(int i = 1; i <= 8; i++)
		this->dropListBC[i].clear();

	char Line[255]; // line
	s_DropEventItem lineDropBC = {0}; // array for line parser
	int bcType = -1; // bc type (1-8)

	while(!feof(fp))
	{
		fgets(Line, 255, fp);

		// Skip comments & end's
		if(Line[0] == '/' || strcmp(Line, "end") == 0 || strcmp(Line, "end\n") == 0 || strcmp(Line, "\n") == 0) continue;

		if (strlen(Line) <= 3)
		{
			int q = 0;
			sscanf_s(Line, "%d", &q);
			bcType = q;
		}
		else
		{
			// %Rate, Type, Idx, minLvl, maxLvl, Dur, %Skill, %Luck, minOpt, maxOpt, %Exc, minExc, maxExc, %Anc, ancType
			sscanf_s(Line, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", &lineDropBC.dropRate, &lineDropBC.iGroup, &lineDropBC.iId, &lineDropBC.minLvl, &lineDropBC.maxLvl, &lineDropBC.dur, &lineDropBC.skill, &lineDropBC.luck, &lineDropBC.minOpt, &lineDropBC.maxOpt, &lineDropBC.exc, &lineDropBC.minExc, &lineDropBC.maxExc, &lineDropBC.anc, &lineDropBC.ancType);
			this->dropListBC[bcType].push_back(lineDropBC);
		}
	}

    fclose(fp);

	// Log
	Log.ConsoleOutPut(1, c_Cyan, t_Default, "[DropEvent System -> BC] Load sucsessfully.");
}

void cDropEvent::handlerBC(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level, BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption)
{
	//srand(time_seed());
	int rand10k = rand() % 10000 + 1;
	int d = 0; // random system

	int bcType = this->mapsBC[(int)MapNumber]; // 1-8 (id for drop map)

	std::vector<s_DropEventItem>::iterator i;

	for(i = this->dropListBC[bcType].begin(); i < this->dropListBC[bcType].end(); i++)
	{
		/*if ((d + i->dropRate) > 10000)
		{
			MessageBox(NULL, "EventDropBC.txt -> DropRate SUM for one BC must be <= 10000", "Error", MB_OK);
			::ExitProcess(0);
		}*/

		if (rand10k > d && rand10k < (d + i->dropRate))
		{
			// Item founded -> Drop procedure & break
			this->dropItem(aIndex, MapNumber, x, y, LootIndex, *i);
			break;
		}
		else {
			d += i->dropRate;
		}
	}
}

//##############################################################################################

void cDropEvent::updateIT()
{
	// Drop Count
	dropCountIT.countRate = Config.GetInt(0, 100, 0, "DropCount", "DropCountRate",GreatDevelopEventDropIT);
	dropCountIT.countItems = Config.GetInt(1, 10, 1, "DropCount", "DropCountItems",GreatDevelopEventDropIT);

	// Drop List

	FILE* fp;
	fp = fopen(GreatDevelopEventDropIT, "r");
	rewind(fp);

	if(fp == NULL)
	{
		MessageBox(NULL, "EventDropIT.txt not found", "Error", MB_OK);
		::ExitProcess(0);
	}

	// Clear all IT drop confs
	for(int i = 1; i <= 6; i++)
		this->dropListIT[i].clear();

	char Line[255]; // line
	s_DropEventItem lineDropIT = {0}; // array for line parser
	int itType = -1; // it type (1-6)

    while(!feof(fp))
    {
        fgets(Line, 255, fp);

		// Skip comments & end's
		if(Line[0] == '/' || strcmp(Line, "end") == 0 || strcmp(Line, "end\n") == 0 || strcmp(Line, "\n") == 0) continue;

		if (strlen(Line) <= 3)
		{
			int q = 0;
			sscanf_s(Line, "%d", &q);
			itType = q;
		}
		else
		{
			sscanf_s(Line, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d", &lineDropIT.dropRate, &lineDropIT.iGroup, &lineDropIT.iId, &lineDropIT.minLvl, &lineDropIT.maxLvl, &lineDropIT.skill, &lineDropIT.luck, &lineDropIT.minOpt, &lineDropIT.maxOpt, &lineDropIT.exc, &lineDropIT.minExc, &lineDropIT.maxExc, &lineDropIT.anc, &lineDropIT.ancType);
			this->dropListIT[itType].push_back(lineDropIT);
		}
	}

    fclose(fp);

	// Log
	Log.ConsoleOutPut(1, c_Cyan, t_Default, "[DropEvent System -> IT] Load sucsessfully.");
}

void cDropEvent::handlerIT(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level, BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption)
{
	//srand(time_seed());
	int rand10k = rand() % 10000 + 1;
	int d = 0; // random system

	int itType = this->mapsIT[(int)MapNumber]; // 1-6 (id for drop map)

	std::vector<s_DropEventItem>::iterator i;

	for(i = this->dropListIT[itType].begin(); i < this->dropListIT[itType].end(); i++)
	{
		/*if ((d + i->dropRate) > 10000)
		{
			MessageBox(NULL, "EventDropIT.txt -> DropRate SUM for one IT must be <= 10000", "Error", MB_OK);
			::ExitProcess(0);
		}*/

		if (rand10k > d && rand10k < (d + i->dropRate))
		{
			// Item founded -> Drop procedure & break
			this->dropItem(aIndex, MapNumber, x, y, LootIndex, *i);
			break;
		}
		else {
			d += i->dropRate;
		}
	}
}

//##############################################################################################

void cDropEvent::updateWW()
{
	// Drop Count
	dropCountWW.countRate = Config.GetInt(0, 100, 0, "DropCount", "DropCountRate",GreatDevelopEventDropWW);
	dropCountWW.countItems = Config.GetInt(1, 10, 1, "DropCount", "DropCountItems",GreatDevelopEventDropWW);

	// Drop List

	FILE* fp;
	fp = fopen(GreatDevelopEventDropWW, "r");
	rewind(fp);

	if(fp == NULL)
	{
		MessageBox(NULL, "EventDropWW.txt not found", "Error", MB_OK);
		::ExitProcess(0);
	}

	// Clear WW drop confs
	this->dropListWW.clear();

	char Line[255]; // line
	s_DropEventItem lineDropWW = {0}; // array for line parser

    while(!feof(fp))
    {
        fgets(Line, 255, fp);

		// Skip comments & end's
		if(Line[0] == '/' || strcmp(Line, "end") == 0 || strcmp(Line, "end\n") == 0 || strcmp(Line, "\n") == 0) continue;

		sscanf_s(Line, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d", &lineDropWW.dropRate, &lineDropWW.iGroup, &lineDropWW.iId, &lineDropWW.minLvl, &lineDropWW.maxLvl, &lineDropWW.skill, &lineDropWW.luck, &lineDropWW.minOpt, &lineDropWW.maxOpt, &lineDropWW.exc, &lineDropWW.minExc, &lineDropWW.maxExc, &lineDropWW.anc, &lineDropWW.ancType);
		this->dropListWW.push_back(lineDropWW);
	}

    fclose(fp);

	// Log
	Log.ConsoleOutPut(1, c_Cyan, t_Default, "[DropEvent System -> WW] Load sucsessfully.");
}

void cDropEvent::handlerWW(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level, BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption)
{
	//srand(time_seed());
	int rand10k = rand() % 10000 + 1;
	int d = 0; // random system

	std::vector<s_DropEventItem>::iterator i;

	for(i = this->dropListWW.begin(); i < this->dropListWW.end(); i++)
	{
		/*if ((d + i->dropRate) > 10000)
		{
			MessageBox(NULL, "EventDropWW.txt -> DropRate SUM for WW must be <= 10000", "Error", MB_OK);
			::ExitProcess(0);
		}*/

		if (rand10k > d && rand10k < (d + i->dropRate))
		{
			// Item founded -> Drop procedure & break
			this->dropItem(aIndex, MapNumber, x, y, LootIndex, *i);
			break;
		}
		else {
			d += i->dropRate;
		}
	}
}

//##############################################################################################

void cDropEvent::updateCC()
{
	// Drop Count
	dropCountCC.countRate = Config.GetInt(0, 100, 0, "DropCount", "DropCountRate",GreatDevelopEventDropCC);
	dropCountCC.countItems = Config.GetInt(1, 10, 1, "DropCount", "DropCountItems",GreatDevelopEventDropCC);

	// Drop List

	FILE* fp;
	fp = fopen(GreatDevelopEventDropCC, "r");
	rewind(fp);

	if(fp == NULL)
	{
		MessageBox(NULL, "EventDropCC.txt not found", "Error", MB_OK);
		::ExitProcess(0);
	}

	// Clear all CC drop confs
	for(int i = 1; i <= 7; i++)
		this->dropListCC[i].clear();

	char Line[255]; // line
	s_DropEventItem lineDropCC = {0}; // array for line parser
	int ccType = -1; // cc type (1-7)

    while(!feof(fp))
    {
        fgets(Line, 255, fp);

		// Skip comments & end's
		if(Line[0] == '/' || strcmp(Line, "end") == 0 || strcmp(Line, "end\n") == 0 || strcmp(Line, "\n") == 0) continue;

		if (strlen(Line) <= 3)
		{
			int q = 0;
			sscanf_s(Line, "%d", &q);
			ccType = q;
		}
		else
		{
			sscanf_s(Line, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d", &lineDropCC.dropRate, &lineDropCC.iGroup, &lineDropCC.iId, &lineDropCC.minLvl, &lineDropCC.maxLvl, &lineDropCC.skill, &lineDropCC.luck, &lineDropCC.minOpt, &lineDropCC.maxOpt, &lineDropCC.exc, &lineDropCC.minExc, &lineDropCC.maxExc, &lineDropCC.anc, &lineDropCC.ancType);
			this->dropListCC[ccType].push_back(lineDropCC);
		}
	}

    fclose(fp);

	// Log
	Log.ConsoleOutPut(1, c_Cyan, t_Default, "[DropEvent System -> CC] Load sucsessfully.");
}

void cDropEvent::handlerCC(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level, BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption)
{
	//srand(time_seed());
	int rand10k = rand() % 10000 + 1;
	int d = 0; // random system

	int ccType = this->mapsCC[(int)MapNumber]; // 1-7 (id for drop map)

	std::vector<s_DropEventItem>::iterator i;

	for(i = this->dropListCC[ccType].begin(); i < this->dropListCC[ccType].end(); i++)
	{
		/*if ((d + i->dropRate) > 10000)
		{
			MessageBox(NULL, "EventDropCC.txt -> DropRate SUM for one CC must be <= 10000", "Error", MB_OK);
			::ExitProcess(0);
		}*/

		if (rand10k > d && rand10k < (d + i->dropRate))
		{
			// Item founded -> Drop procedure & break
			this->dropItem(aIndex, MapNumber, x, y, LootIndex, *i);
			break;
		}
		else {
			d += i->dropRate;
		}
	}
}
#endif