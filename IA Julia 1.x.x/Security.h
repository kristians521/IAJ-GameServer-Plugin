// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#ifndef IpBlock_H
#define IpBlock_H 

#endif

// # Hook in GS # //
short GOBJGetIndex(SOCKET aSocket, char* ip);
// # ---------- # //

class cSecurity
{				  
public:
	cSecurity();
	~cSecurity();
	void Load();
	void ChangeStar(char*IP,int i);
	bool CheckIp(char GobjIP[16]); 
	bool CheckIp2(char ConfigIP[16], char GobjIP[16]);
	bool CheckIp3(char GobjIP[16]);
	void Tick();

private:
	struct sIps
	{
		char IP[16];
		ULONG Count;	   
		long Time;
	};

	sIps IPBlockInfo[255];
	UINT IPBlockCount;
	sIps Ips[1000];
	UINT NumIps;
};
extern cSecurity Security;