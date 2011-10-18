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
#include "Maps.h"
#include "Fixes.h"
#include "Utilits.h"
#include "Protocol.h"
#include "Logger.h"
#include "Security.h" 
#include "AntiAFK.h"
#include "DuelManager.h" 
#include "MoveReq.h"
#include "PCPoint.h"
#include "ChatCommands.h"
#include "MossGambler.h"	 
#include "Monster.h"  
#include "Query.h"
#include "Vip.h"

DWORD MainTick()  	
{
	while(true)
	{
		int Temp_All = 0;
		int Temp_Gms = 0;
		for(int i=OBJECT_MIN; i<=OBJECT_MAX; i++)
		{
			OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(i);	

			// # PLAYER  LOGGED # //

			if(gObj->Connected < PLAYER_LOGGED) continue;

			if(GmSystem.IsGMBD(gObj->Name))
				Temp_Gms++;
			Temp_All++;

			// # -------------- # //

			// # PLAYER PLAYING # //

			if(gObj->Connected < PLAYER_PLAYING) continue;				

			if(AddTab[gObj->m_Index].POST_Delay > 0)
				AddTab[gObj->m_Index].POST_Delay--;

			AntiAFK.Tick(gObj);
			User.OnlineTimeTick(gObj);
			PCPoint.Tick(gObj);
			Vip.Tick(gObj);

			// # -------------- # //
		}										

		Security.Tick();
#ifdef _GS
		Moss.CheckTime();	 	
#endif	
		Sleep(1000);
		Log.Online_All = Temp_All;
		Log.Online_Gms = Temp_Gms;
	}
	return 1;	
} 

extern "C" __declspec (dllexport) void __cdecl RMST()
{
	DWORD OldProtect;
	if(VirtualProtect(LPVOID(0x401000), GSSIZE, PAGE_EXECUTE_READWRITE, &OldProtect))
	{	
		//Create Log folder in GameServer
		CreateDirectory(IAJuliaConsoleLogs,NULL);	
		CreateDirectory(IAJuliaChatLogs,NULL);
		CreateDirectory(IAJuliaLog,NULL);	
		//
		Sleep(500);
		Log.LoggerInit();
		Sleep(500);
		//
		LoadQuery();
		Maps.MapInit();
		Fixes.ASMFixes();
		Configs.LoadAll();
		#ifdef _GS
			if(Configs.Duel.Enabled)
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

		#ifdef _GS
			hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MonsterAddTick, NULL, 0, &ThreadID);

			if ( hThread == 0 )
			{
				Log.ConsoleOutPut(0, c_Red, t_NULL, "CreateThread() failed with error %d", GetLastError());
				return;
			}

			CloseHandle(hThread);
		#endif
	}
} 