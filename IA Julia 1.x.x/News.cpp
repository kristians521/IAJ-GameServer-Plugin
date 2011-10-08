// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # GreatDevelop 1.1.0 beta						# //
// ================================================== //

#include "StdAfx.h" 
#include "Configs.h"
#include "ChatCommands.h"
#include "News.h"
#include "Logger.h"
#pragma warning(disable: 4244)

News g_News;

News::News()
{ 
}

News::~News()
{

}
void NewsSystem(void * lpParam)
{
	while(true)
	{ 
		g_News.Sections = GetPrivateProfileIntA("News","NewsSections",5,GreatDevelopNews); 

		char PState[10]; 
		for(int i = 1; i <= g_News.Sections; i++)
		{
			wsprintf(PState, "Section%d", i);		
			g_News.NewsSys[i].Time = GetPrivateProfileIntA(PState,"ShowTime",0,GreatDevelopNews) * 60000;
			g_News.NewsSys[i].Type = GetPrivateProfileIntA(PState,"ShowType",0,GreatDevelopNews);
			GetPrivateProfileStringA(PState,"News1","NULL",g_News.NewsSys[i].MSG01,50,GreatDevelopNews);
			GetPrivateProfileStringA(PState,"News2", "NULL", g_News.NewsSys[i].MSG02,50,GreatDevelopNews);
			GetPrivateProfileStringA(PState,"News3", "NULL", g_News.NewsSys[i].MSG03,50,GreatDevelopNews); 	   
			Log.CheckProcent(g_News.NewsSys[i].MSG01);	 
			Log.CheckProcent(g_News.NewsSys[i].MSG02);
			Log.CheckProcent(g_News.NewsSys[i].MSG03);
			Sleep(g_News.NewsSys[i].Time); 
			Chat.MessageAll(g_News.NewsSys[i].Type, 0, NULL, g_News.NewsSys[i].MSG01);
			Chat.MessageAll(g_News.NewsSys[i].Type, 0, NULL, g_News.NewsSys[i].MSG02);
			Chat.MessageAll(g_News.NewsSys[i].Type, 0, NULL, g_News.NewsSys[i].MSG03); 
		} 
	}
	_endthread();
}
