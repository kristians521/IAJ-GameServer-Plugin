// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#include "StdAfx.h" 
#include "Configs.h"
#include "ChatCommands.h"
#include "News.h"
#include "Logger.h"
#pragma warning(disable: 4244)

cNews News;

void cNews::Load()
{
	Config.IsNews						= Configs.GetInt(0, 1, 1, "News", "NewsSystemEnabled", IAJuliaNews);
	if(!Config.IsNews)return;
	_beginthread(NewsSystem, 0, NULL);
}

void NewsSystem(void * lpParam)
{
	while(true)
	{ 
		News.Sections = GetPrivateProfileIntA("News","NewsSections",5,IAJuliaNews); 

		char PState[10]; 
		for(int i = 1; i <= News.Sections; i++)
		{
			wsprintf(PState, "Section%d", i);		
			News.NewsSys[i].Time = GetPrivateProfileIntA(PState, "ShowTime", 0, IAJuliaNews) * 60000;
			News.NewsSys[i].Type = GetPrivateProfileIntA(PState, "ShowType", 0, IAJuliaNews);
			Sleep(News.NewsSys[i].Time); 
			char PNews[6];
			for(int j=0; j<3; j++)
			{
				wsprintf(PNews, "News%d", j);	
				GetPrivateProfileStringA(PState, PNews, NULL, News.NewsSys[i].MSG[0], 50, IAJuliaNews);
				if(News.NewsSys[i].MSG[j] == NULL)
					continue;
				Log.CheckProcent(News.NewsSys[i].MSG[j]);	
				Chat.MessageAll(News.NewsSys[i].Type, 0, NULL, News.NewsSys[i].MSG[j]);
			}
		} 
	}
	_endthread();
}
