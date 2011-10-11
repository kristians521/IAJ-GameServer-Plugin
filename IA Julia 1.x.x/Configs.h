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
	void LoadGmSystem();  									 
	void LoadAntiAfk();	
	void LoadDuel();
	void LoadNotice();
	void LoadPets();
	void LoadArcher();
	void VIPSystem();
	void LoadCommands();
	void LoadPkClearGuard();
	void ZenFixes();
	void Misc(); 
	void MapSys();
public:			 
	cConfigs();
	~cConfigs();
	void LoadFixes();	
	void LoadConfigsInGS();
	void LoadAll();
	void LoadNews();
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
	int IsNews;
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

	struct sPkClear
	{
		int Enabled;
		int NpcId;	  
		int OnlyForGm;
		int Type;	
		DWORD PriceZen;
		DWORD PriceZenForAll;	  
		int PricePcPoints;
		int PricePcPointsForAll;	   
		int PriceWCoins;
		int PriceWCoinsForAll;
		int LevelReq;
	};	

	sPkClear ClearNpc;			 
	sPkClear ClearCommand;


	struct sGmConfig
	{	  
		int IsGMSystem;
		int IsGMInDB;
	}GmSystemConfig;	 

	struct sDuel
	{
		int Enabled;
		int Ranking;
		int Logging;
	}Duel;

	struct sAntiAfkConfig
	{
		int Enabled;
		int Time;
		int Warnings;
	}AntiAfkConfig;

	struct sArcher
	{
		int Enabled;
		int ZenReward;
		int WCoinsReward;
		int PCPointsReward;
		int NeedRenaAmount; 
	}Archer;

	struct cZen
	{ 
		int	NormalZen;
		int ZenInParty;

	}Zen;

	struct VIP
	{
		int NumStates;
		int Enabled;
		char Column[25];
		char ColumnDate[25];

		struct VIPConfig
		{ 
			char VIPName[8];
			int EnabledCmd;
			DWORD CostZen;
			int CostPCPoints;

			int CostWCoins;
			int BonusExp;
			int BonusZen;
			int BonusDrop; 

			int MinHours;
			int MaxHours;
		};
		VIPConfig VIPState[20]; 

	}VIP;

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
extern cConfigs Config;

#endif