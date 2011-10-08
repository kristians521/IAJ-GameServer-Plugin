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

class cIpBlock
{				  
public:
	cIpBlock();
	~cIpBlock();
	void LoadIpBlock();
	bool CheckIp(DWORD aIndex); 
	bool CheckIp2(char ConfigIP[16], char GobjIP[16]);

	struct sIPBlock
	{
		char IP[16];
	};

private:
	sIPBlock IPBlockInfo[255];
	int IPBlockCount;
};
extern cIpBlock IpBlock;