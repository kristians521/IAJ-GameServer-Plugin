// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # GreatDevelop 1.0.1								# //
// ================================================== //

#include "StdAfx.h"
#include "Configs.h"
#include "Maps.h"
#include "Fixes.h"
#include "Utilits.h"
#include "Protocol.h"
#include "Logger.h"
#include "IpBlock.h"	 
#include "AntiAFK.h"
#include "DuelManager.h" 
#include "MoveReq.h"
#include "PCPoint.h"
#include "ChatCommands.h"
#include "MossGambler.h"	 
#include "Monster.h"  
#include "Query.h"
struct sIps
{
	char Ip[16];
	ULONG Count;	   
	long Time;
};

UINT NumIps = 0;
sIps Ips[1000];

bool CheckVipTime(int TimeInMin);

DWORD MainTick()  	
{
	while(true)
	{
		for(int i=OBJECT_MIN; i<=OBJECT_MAX; i++)
		{
			OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(i);	

			if(gObj->Connected < PLAYER_PLAYING) continue;	

			int Index = gObj->m_Index;  

			if(AddTab[Index].POST_Delay > 0)
				AddTab[Index].POST_Delay--;

			if(Config.AntiAfkConfig.Enabled == 1)
				AntiAFK.SendInfo(gObj);

			AddTab[Index].ON_Sek++;
			if(AddTab[Index].ON_Sek >= 60)
			{
				AddTab[Index].ON_Sek = 0;
				AddTab[Index].ON_Min++;
				//PCPoint.UpdatePoints(gObj,0,PLUS,PCPOINT);
			}
			if(AddTab[Index].ON_Min >= 60)
			{
				AddTab[Index].ON_Min = 0;
				AddTab[Index].ON_Hour++;
				Me_MuOnlineQuery.ExecQuery("UPDATE MEMB_STAT SET OnlineHours = OnlineHours + 1 WHERE memb___id = '%s'", gObj->AccountID);
					Me_MuOnlineQuery.Fetch();
					Me_MuOnlineQuery.Close();
			}
			/*Add PCPoints for Online System*/
			if (PCPoint.sPoints.Enabled && PCPoint.sPoints.AddPCPointsSec > 0)
			{
				AddTab[Index].PC_OnlineTimer++;
				if (AddTab[Index].PC_OnlineTimer == PCPoint.sPoints.AddPCPointsSec)
				{
					AddTab[Index].PC_OnlineTimer = 0;
					PCPoint.UpdatePoints(gObj,PCPoint.sPoints.AddPCPointsCount,PLUS,PCPOINT);

					if (AddTab[Index].PC_PlayerPoints < PCPoint.sPoints.MaximumPCPoints)
					{
						Chat.Message( Index,"[PointShop] You earned %d Points for being online!", PCPoint.sPoints.AddPCPointsCount);
						Chat.Message( Index,"[PointShop] You have been online %d Hour %d Minutes!", AddTab[Index].ON_Hour, AddTab[Index].ON_Min);
					}
					else
					{
						Chat.Message( Index,"[PCPoint] You have maximum PCPoints");
					}
				}
			} 
			/*VIP System*/
			if(AddTab[gObj->m_Index].VIP_Type > 0 && Config.VIP.Enabled)
			{ 
				AddTab[gObj->m_Index].VIP_Sec++;
				if(AddTab[gObj->m_Index].VIP_Sec >= 60)
				{
					AddTab[gObj->m_Index].VIP_Min--;
					AddTab[gObj->m_Index].VIP_Sec = 0;

					if(CheckVipTime(AddTab[gObj->m_Index].VIP_Min))
					{			
						Chat.MessageLog(1, c_Red, t_VIP, gObj, "[VIP] You have %d more vip minutes", AddTab[gObj->m_Index].VIP_Min); 
					}
					if(AddTab[gObj->m_Index].VIP_Min <= 0)
					{
						Chat.MessageLog(1, c_Red, t_VIP, gObj, "[VIP] Your vip time is over! You are normal player again."); 
						AddTab[gObj->m_Index].VIP_Type = 0;
						AddTab[gObj->m_Index].VIP_Min = 0;
						MuOnlineQuery.ExecQuery("UPDATE Character SET %s = 0, %s = 0 WHERE Name = '%s'", Config.VIP.Column, Config.VIP.ColumnDate, gObj->Name);
							MuOnlineQuery.Fetch();
							MuOnlineQuery.Close();
					}
					else
					{
						MuOnlineQuery.ExecQuery("UPDATE Character SET %s = %d WHERE Name = '%s'", Config.VIP.ColumnDate, AddTab[gObj->m_Index].VIP_Min, gObj->Name);
							MuOnlineQuery.Fetch();
							MuOnlineQuery.Close();
					}
				}
			}
		}				
														
		for(UINT x = 0; x < NumIps; x++)
		{
			if(Ips[x].Time > 0)
			{
				Ips[x].Time --;	  
				continue;
			}

			if(Ips[x].Time == 0)
			{
				for(UINT j = x; j < NumIps-1; j++)
				{
					strcpy(Ips[x].Ip, Ips[x+1].Ip);    
					Ips[x].Count = Ips[x+1].Count;
					Ips[x].Time = Ips[x+1].Time;		  
				}
				Ips[NumIps-1].Count = 0;
				Ips[NumIps-1].Ip[0] = NULL;
				Ips[NumIps-1].Time = -1;	
				NumIps--;
			}			 
		} 
#ifdef _GS
		/* Moss The Gambler */
		moss.MossConfig.EnableTimer && moss.MossConfig.EnableMoss ? moss.CheckTime() : moss.OpenedMoss = TRUE;	 	
#endif	
		Sleep(1000);
	}
	return 1;	
} 

bool CheckVipTime(int TimeInMin)
{
	switch(TimeInMin)
	{
	case 1:
	case 2:
	case 3:
	case 5:
	case 15:
	case 30:
	case 60:
	case 120:
	case 340:
	case 680:
		return true;
	}
	return false;
}

short GOBJGetIndex(SOCKET aSocket, char* ip)
{																		   
	if(NumIps == 0)
	{			  				  
		strcpy(Ips[NumIps].Ip, ip);
		Ips[NumIps].Count = 0;		 
		Ips[NumIps].Time = 5;
		NumIps++;					
	}

	else if(NumIps > 0)
	{
		for(UINT x = 0; x < NumIps; x++)
		{
			if(!strcmp(Ips[x].Ip,ip))
			{						
				Ips[x].Count++;
				if(Ips[x].Count > 400000000)
					Ips[x].Count = 400000000;
				Ips[x].Time = 5*Ips[x].Count; 
				return -1;
			}
		}	   
		strcpy(Ips[NumIps].Ip, ip);  
		Ips[NumIps].Count = 0;		  
		Ips[NumIps].Time = 5; 
		NumIps++;
	}

	return GSGOBJGetIndex(aSocket, ip);
}

extern "C" __declspec (dllexport) void __cdecl RMST()
{
	DWORD OldProtect;
	if(VirtualProtect(LPVOID(0x401000), GSSIZE, PAGE_EXECUTE_READWRITE, &OldProtect))
	{	
		//Create Log folder in GameServer
		CreateDirectory(GreatDevelopConsoleLogs,NULL);	
		CreateDirectory(GreatDevelopChatLogs,NULL);
		CreateDirectory(GreatDevelopLog,NULL);	
		//
		Sleep(500);
		Log.LoggerInit();
		Sleep(500);
		//
		LoadQuery();
		Maps.MapInit();
		Config.LoadFixes();
		Fixes.ASMFixes();
		Config.LoadConfigsInGS();
		Config.LoadAll();
		IpBlock.LoadIpBlock();
		MoveReq.MoveReqLoad();
		Config.LoadNews();
		#ifdef _GS
		ReadMonsterAdd();
			if(Config.Duel.Enabled)
			{
				g_DuelSystem.DuelCreateThread();
				Utilits.HookThis((DWORD)&HookSetDuelOption,0x004037B5);
				Utilits.HookThis((DWORD)&HookIsDuelEnable,0x00404BCE);
				Utilits.HookThis((DWORD)&HookIsOnDuel,0x00404B56);
				Utilits.HookThis((DWORD)&HookDuelCheck1,0x00403017);
				Utilits.HookThis((DWORD)&HookDuelCheck2,0x004012DA);
				Utilits.HookThis((DWORD)&GSgObjUserDie,0x0040367A);
			}
			Utilits.HookThis((DWORD)&ProtocolCore, 0x004038BE); 
			Utilits.HookThis((DWORD)&gObjGameClose_Func, 0x004034B8);  
			Utilits.HookThis((DWORD)&ProtocolCoreSend,0x004055BF);  
			Utilits.HookThis((DWORD)&MyObjCalCharacter,0x004031A7);				 
			Utilits.HookThis((DWORD)&MonsterDie, 0x004184C4);
			Utilits.HookThis((DWORD)&GCEquipmentSendHook, 0x00407509);
			Utilits.HookThis((DWORD)&GCKillPlayerExpSendHook, 0x00406D11);	
			Utilits.HookThis((DWORD)&MygEventMonsterItemDrop, 0x00402AD1);
			Utilits.HookThis((DWORD)&GOBJGetIndex,0x00404D18);	
			Utilits.HookThis((DWORD)&gObjLevelUpPointAddEx,0x00406D7F);
			Utilits.HookThis((DWORD)&gObjPlayerKiller, 0x0040655F); 
			Utilits.HookThis((DWORD)&gObjAttack, 0x00403CA6);
		#endif

		#ifdef _GS_CS
			Utilits.HookThis((DWORD)&ProtocolCore, 0x00403A17); 
			Utilits.HookThis((DWORD)&ProtocolCoreSend,0x00405AE7); 	
			Utilits.HookThis((DWORD)&gObjGameClose_Func, 0x00403599);   
			Utilits.HookThis((DWORD)&MyObjCalCharacter,0x004031F7);		   
			Utilits.HookThis((DWORD)&MonsterDie, 0x0041A1AE);
			Utilits.HookThis((DWORD)&GCEquipmentSendHook, 0x00407E37);
			Utilits.HookThis((DWORD)&GCKillPlayerExpSendHook, 0x0040751D); 		
			Utilits.HookThis((DWORD)&MygEventMonsterItemDrop, 0x004029F0);
			Utilits.HookThis((DWORD)&GOBJGetIndex,0x00405114);	
			Utilits.HookThis((DWORD)&gObjLevelUpPointAddEx,0x004075B3);
			Utilits.HookThis((DWORD)&gObjPlayerKiller, 0x00406C8A);
			Utilits.HookThis((DWORD)&gObjAttack, 0x00403E72);

		#endif

		DWORD ThreadID;
		HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainTick, NULL, 0, &ThreadID);

		if ( hThread == 0 )
		{
			Log.ConsoleOutPut(0, c_Red, t_NULL, "CreateThread() failed with error %d", GetLastError());
			return;
		}

		CloseHandle(hThread);
	}
} 