// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#include "StdAfx.h"
#include "HappyHour.h"
#include "ChatCommands.h"
#include "Configs.h"
#include "Logger.h"
#include "Prodef.h"
#include "Utilits.h"

cHappyHour HappyHour;

void cHappyHour::Load()
{
	HappyHourEnabled = Configs.GetInt(0,1,1,"HappyHour","EnableHappyHour",IAJuliaHappyHour);
	if (HappyHourEnabled == 0) return;

	char Buff[256];
	int Flag = 0;
	HappyHoursNumber = 0;
	MessageTimeBeforeCount = 0;
	MessageTimeAfterCount = 0;
	
	CTime t = CTime::GetCurrentTime();
	int Hour = t.GetHour();
	int Min = t.GetMinute();
	int WeekDay = t.GetDayOfWeek();

	FILE *conf = fopen(IAJuliaHappyHour,"r");

	if ( conf != NULL)
	{
		while ( !feof(conf))
		{
			fgets(Buff,256,conf);

			if(Utilits.IsBadFileLine(Buff, &Flag))
				continue;

			if (Flag == 1)
			{
				int n[7];
				sscanf(Buff, "%d %d %d %d %d %d %d %d", &n[0], &n[1], &n[2], &n[3], &n[4], &n[5], &n[6], &n[7]);
				HappyStruct[HappyHoursNumber].MapNum	= n[0];
				HappyStruct[HappyHoursNumber].P_Zen		= n[1];
				HappyStruct[HappyHoursNumber].P_Drop	= n[2];
				HappyStruct[HappyHoursNumber].P_Exp		= n[3];
				HappyStruct[HappyHoursNumber].S_WeekDay	= n[4];
				HappyStruct[HappyHoursNumber].S_Hour	= n[5];
				HappyStruct[HappyHoursNumber].S_Min		= n[6];
				HappyStruct[HappyHoursNumber].T_Delay	= n[7];

				HappyStruct[HappyHoursNumber].Started	= false;

				HappyStruct[HappyHoursNumber].C_Min		= HappyStruct[HappyHoursNumber].S_Min + HappyStruct[HappyHoursNumber].T_Delay;
				HappyStruct[HappyHoursNumber].C_Hour	= HappyStruct[HappyHoursNumber].S_Hour;
				if(HappyStruct[HappyHoursNumber].S_WeekDay == -1)
					HappyStruct[HappyHoursNumber].C_WeekDay = WeekDay;
				else
					HappyStruct[HappyHoursNumber].C_WeekDay = HappyStruct[HappyHoursNumber].S_WeekDay;

				while (HappyStruct[HappyHoursNumber].C_Min >=60)
				{
					HappyStruct[HappyHoursNumber].C_Hour++;
					HappyStruct[HappyHoursNumber].C_Min -= 60;
				}
				while (HappyStruct[HappyHoursNumber].C_Hour >= 24)
				{
					HappyStruct[HappyHoursNumber].C_WeekDay = HappyStruct[HappyHoursNumber].C_WeekDay % 7 + 1;
					HappyStruct[HappyHoursNumber].C_Hour -= 24;
				}
				HappyHoursNumber++;
			}

			if (Flag == 2)
			{
				sscanf(Buff, "%d", &MessageTimeBefore[MessageTimeBeforeCount++]);
			}

			if (Flag == 3)
			{
				sscanf(Buff, "%d", &MessageTimeAfter[MessageTimeAfterCount++]);
			}
		}

	}			  
	fclose(conf);

	Log.ConsoleOutPut(1, c_Magenta, t_Default, "[Happy Hour] Loaded %d locations." ,HappyHoursNumber);
}

void cHappyHour::TickTimerHappyHour()
{
	if (HappyHourEnabled == 0) return;
	CTime t = CTime::GetCurrentTime();
	int Hour = t.GetHour();
	int Min = t.GetMinute();
	int WeekDay = t.GetDayOfWeek();
	
	if (!t.GetSecond())
	{
		for (int i=0; i<HappyHoursNumber; i++)
		{
			if (Hour == HappyStruct[i].S_Hour && Min == HappyStruct[i].S_Min && (WeekDay == HappyStruct[i].S_WeekDay || HappyStruct[i].S_WeekDay == -1))
			{
				Chat.MessageAll(0,0,NULL,"[Happy Hour] %s Event Started. Event Close in %d:%02d.",
					Utilits.GetMapName(HappyStruct[i].MapNum),HappyStruct[i].C_Hour,HappyStruct[i].C_Min);
				HappyStruct[i].Started = true;
			}

			if  (Hour == HappyStruct[i].C_Hour && Min == HappyStruct[i].C_Min && (WeekDay == HappyStruct[i].C_WeekDay || HappyStruct[i].C_WeekDay == -1))
			{
				Chat.MessageAll(0,0,NULL,"[Happy Hour] On %s Event Close.",Utilits.GetMapName(HappyStruct[i].MapNum));	
				HappyStruct[i].Started = false;
			}
			CheckNeedMessage(i);
		}
	}
}

void cHappyHour::CheckNeedMessage(int i)
{
	CTime t = CTime::GetCurrentTime();
	int Hour = t.GetHour();
	int Min = t.GetMinute();
	int WeekDay = t.GetDayOfWeek();

	for (int j = 0; j < MessageTimeBeforeCount; j++)
	{
		int TempMin = HappyStruct[i].S_Min - MessageTimeBefore[j];
		int TempHour = HappyStruct[i].S_Hour;
		int TempWeekday = HappyStruct[i].S_WeekDay;

		while(TempMin < 0)
		{
			TempMin += 60;
			TempHour --;
		}

		while(TempHour < 0)
		{
			TempHour += 24;
			TempWeekday --;
			if(TempWeekday == 0)
				TempWeekday = 7;
		}

		if(Hour == TempHour && Min == TempMin && WeekDay == TempWeekday)
			Chat.MessageAll(0,0,NULL,"[Happy Hour] %s will start in %d min.", Utilits.GetMapName(HappyStruct[i].MapNum), MessageTimeBefore[j]);	
	}

	for (int l = 0; l < MessageTimeAfterCount; l++)
	{
		if(HappyStruct[i].Started == false)
			continue;

		int TempMin = HappyStruct[i].C_Min - MessageTimeBefore[l];
		int TempHour = HappyStruct[i].C_Hour;
		int TempWeekday = HappyStruct[i].C_WeekDay;

		while(TempMin < 0)
		{
			TempMin += 60;
			TempHour --;
		}

		while(TempHour < 0)
		{
			TempHour += 24;
			TempWeekday --;
			if(TempWeekday == 0)
				TempWeekday = 7;
		}

		if(Hour == TempHour && Min == TempMin && WeekDay == TempWeekday)
			Chat.MessageAll(0,0,NULL,"[Happy Hour] %s will end in %d min.", Utilits.GetMapName(HappyStruct[i].MapNum), MessageTimeBefore[l]);	
	}
}

int cHappyHour::IsHappyHour(int Map)
{
	if (HappyHourEnabled == 0) return -1;

	for (int i = 0; i < HappyHoursNumber; i++)
	{
		if((HappyStruct[i].MapNum == Map ||	HappyStruct[i].MapNum == -1) &&
			HappyStruct[i].Started == true)
			return i;
	}
	return -1;
}