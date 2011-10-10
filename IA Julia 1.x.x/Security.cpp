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
#include "User.h" 
#include "Security.h"
#include "Logger.h"
					  
cSecurity Security;

cSecurity::cSecurity()
{		  
	Load();
	NumIps = 0;
}

cSecurity::~cSecurity()
{		  
}

void cSecurity::Load()
{
	FILE *fp;
	BOOL bRead = FALSE;
	DWORD dwArgv = 0;
	char sLineTxt[255] = {0};
	IPBlockCount = 1;

	fp = fopen(IAJuliaIPBlock,"r");

	rewind(fp);

	while(fgets(sLineTxt, 255, fp) != NULL)
	{
		if(sLineTxt[0] == '/')continue;			
		if(sLineTxt[0] == ';')continue;
		if(sLineTxt[0] == 'e' && sLineTxt[1] == 'n' && sLineTxt[2] == 'd')continue;
		char IP[16];							
		if(strlen(sLineTxt) <= 1)continue;

		sscanf(sLineTxt, "%s", IP);
		sprintf(IPBlockInfo[IPBlockCount].IP,"%s",IP);
		IPBlockCount++;
	}

	rewind(fp);
	fclose(fp);
}

bool cSecurity::CheckIp(char GobjIP[16])
{
	for(int x=1;x < IPBlockCount;x++)
	{
		if(!CheckIp2(IPBlockInfo[x].IP, GobjIP))
		{					
			Log.ConsoleOutPut(1, c_Red, t_IPBLOCK,"[Anti-Hack] Blocked %s try to connect!!!",GobjIP);
			return false;
		}
	}
	return true;
} 
						  
void cSecurity::ChangeStar(char*IP,int i)
{
	for(int j=strlen(IP); j>=i; j--)
		IP[j+1] = IP[j];
	IP[i] = '-';
	IP[i+1] = '1';
}

bool cSecurity::CheckIp2(char ConfigIP[16], char GobjIP[16])
{							   
	for(int i=0; i<16;i++)
	{							  
		if(ConfigIP[i] == '.')
			ConfigIP[i] = ' ';
		if(GobjIP[i] == '.')
			GobjIP[i] = ' ';
									 
		if(ConfigIP[i] == '*')
			ChangeStar(ConfigIP, i); 
	}

	int CIP[4];
	int GIP[4];
																	  
	sscanf(ConfigIP,"%d %d %d %d",&CIP[0],&CIP[1],&CIP[2],&CIP[3]);
	sscanf(GobjIP,"%d %d %d %d",&GIP[0],&GIP[1],&GIP[2],&GIP[3]);

	if((CIP[0] == GIP[0] || CIP[0] == -1) && 
		(CIP[1] == GIP[1] || CIP[1] == -1) &&
		(CIP[2] == GIP[2] || CIP[2] == -1) &&
		(CIP[3] == GIP[3] || CIP[3] == -1))
		return false;	
	return true;
}

void cSecurity::Tick()
{
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
				strcpy(Ips[x].IP, Ips[x+1].IP);    
				Ips[x].Count = Ips[x+1].Count;
				Ips[x].Time = Ips[x+1].Time;		  
			}
			Ips[NumIps-1].Count = 0;
			Ips[NumIps-1].IP[0] = NULL;
			Ips[NumIps-1].Time = -1;	
			NumIps--;
		}			 
	} 
}

bool cSecurity::CheckIp3(char GobjIP[16])
{
	if(NumIps == 0)
	{			  				  
		strcpy(Ips[NumIps].IP, GobjIP);
		Ips[NumIps].Count = 0;		 
		Ips[NumIps].Time = 5;
		NumIps++;					
	}
	else if(NumIps > 0)
	{
		for(UINT x = 0; x < NumIps; x++)
		{
			if(!strcmp(Ips[x].IP, GobjIP))
			{						
				Ips[x].Count++;
				if(Ips[x].Count > 400000000)
					Ips[x].Count = 400000000;
				Ips[x].Time = 5*Ips[x].Count; 
				return false;
			}
		}	   
		strcpy(Ips[NumIps].IP, GobjIP);  
		Ips[NumIps].Count = 0;		  
		Ips[NumIps].Time = 5; 
		NumIps++;
	}
	return true;
}

short GOBJGetIndex(SOCKET aSocket, char* ip)
{						
	if(!Security.CheckIp(ip))
		return -1;

	if(!Security.CheckIp3(ip))
		return -1;

	return GSGOBJGetIndex(aSocket, ip);
}