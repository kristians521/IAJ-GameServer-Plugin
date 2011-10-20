// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#ifndef CONFIGS_H
#define CONFIGS_H

class cConfigs
{
private:				
	void LoadNotice();
	void LoadPets();
	void LoadCommands();
	void ZenFixes();
	void Misc(); 
	void MapSys();
public:			 
	cConfigs();
	~cConfigs();
	void LoadFixes();	
	void LoadConfigsInGS();
	void LoadAll();
	long GetInt(long Min, long Max, long Default, LPCSTR BlockName, LPCSTR ConfigName, LPCSTR FolderName);
	char GetChar(long Min, long Max, unsigned char Default, LPCSTR BlockName, LPCSTR ConfigName, LPCSTR FolderName);

	#ifdef _GS
	int EldaradoEventType;
	int CCAllowingPlayers;
	int BCAllowingPlayers;
	int BCEnterMasterType;
	int DSAllowingPlayers; 
	#endif

	int UseChecksum;	  
	int PersonalIDFix;	 
	int GuildIDFix;
	int DisableLogs;
	int AllowExeAnc;
	int AllowJohAnc;
	int AllowExeSock;
	int MaxLifeOpt;
	int Use65kStats;
	int CSSkillsOnNOrmal;
	int Enable65kStats;
	int DumpFile;

	char ResetColumn[25];	
	char ServerName[25];
	char ConnectNotice[100];	
	int ConnectInfo; 
	struct sCommands
	{
		int MaxLvl; 
		int IsGg;
		int IsPost;

		int PostLvl;
		DWORD MaxAddedStats;
		DWORD PostPriceZen;	
		int PostPricePCPoint;
		int PostPriceWCoin;
		int PostColor;  
		int PostDelay;

		int IsBanPost;
		int IsBanAcc;
		int IsBanChar;
		int IsDrop;
		int IsReload;
		int IsGmove; 
		int IsSetPK;
		int IsTime;
		int IsOnline; 
		int IsZenDrop;
		int IsStatus;
		int IsSetChar; 

		int SkinEnabled;
		int SkinOnlyForGm;
		int SkinLevelReq;
		DWORD SkinPriceZen;
		int SkinPricePcPoint;
		int SkinPriceWCoin;

		int IsMultyVault;
		int NumberOfVaults;	   
		DWORD ZenForChange;
		int PcPointForChange;
		int WCoinForChange;

		int AddPointEnabled;
		int AddPointLevelReq;  
		DWORD AddPriceZen; 
		int AddPricePCPoint;
		int AddPriceWCoin;
	}Commands;
	
	struct cZen
	{ 
		int	NormalZen;
		int ZenInParty;

	}Zen;
	
	int GuildRes;
	int GuildLevel;
	int IsPartyGap;
	int PartyGapLvl;
	int LahapDupe;
	int Unicode32;

	struct sPanda
	{
		int PetPandaDefense;
		int PetPandaAttackDamageMinLeft;
		int PetPandaAttackDamageMaxLeft;
		int PetPandaAttackDamageMinRight;
		int PetPandaAttackDamageMaxRight;
		int PetPandaMagicDamageMin;
		int PetPandaMagicDamageMax;
		int PetPandaMagicSpeed;
		int PetPandaAttackSpeed;
		int PetPandaExpirence;
		int PetPandaMLExpirence;

		int PandaRingDefense;
		int PandaRingAttackDamageMinLeft;
		int PandaRingAttackDamageMaxLeft;
		int PandaRingAttackDamageMinRight;
		int PandaRingAttackDamageMaxRight;
		int PandaRingMagicDamageMin;
		int PandaRingMagicDamageMax;
		int PandaRingMagicSpeed;
		int PandaRingAttackSpeed;
		int PandaRingExpirence;
		int PandaRingMLExpirence;
	}Panda;
};
extern cConfigs Configs;

#endif