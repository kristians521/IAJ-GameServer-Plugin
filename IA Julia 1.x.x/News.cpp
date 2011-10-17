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

cNews::cNews()
{ 
}

cNews::~cNews()
{

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
			News.NewsSys[i].Time = GetPrivateProfileIntA(PState,"ShowTime", 0, IAJuliaNews) * 60000;
			News.NewsSys[i].Type = GetPrivateProfileIntA(PState,"ShowType", 0, IAJuliaNews);
			GetPrivateProfileStringA(PState,"News1", NULL, News.NewsSys[i].MSG[0], 50, IAJuliaNews);
			GetPrivateProfileStringA(PState,"News2", NULL, News.NewsSys[i].MSG[1], 50, IAJuliaNews);
			GetPrivateProfileStringA(PState,"News3", NULL, News.NewsSys[i].MSG[2], 50, IAJuliaNews); 
			Sleep(News.NewsSys[i].Time); 
			for(int j=0; j<3; j++)
			{
				if(News.NewsSys[i].MSG[j] == NULL)
					continue;
				Log.CheckProcent(News.NewsSys[i].MSG[j]);	
				Chat.MessageAll(News.NewsSys[i].Type, 0, NULL, News.NewsSys[i].MSG[j]);
			}
		} 
	}
	_endthread();
}
