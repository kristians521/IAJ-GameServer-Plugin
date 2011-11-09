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
#include "Logger.h"
#include "GMSystem.h"
#include "AntiAFK.h"
#include "News.h"
#include "DropSystem.h"
#include "Archer.h"
#include "PCPoint.h"
#include "MossGambler.h" 
#include "MapSystem.h"
#include "Query.h"
#include "WZEventDrop.h"
#include "MoveReq.h"
#include "DuelManager.h"
#include "Vip.h"
#include "Monster.h"
#include "HappyHour.h"

cConfigs Configs;	  


void cConfigs::LoadAll()
{
	LoadConfigsInGS();
	PCPoint.LoadIniConfigs();
	GmSystem.Load();
	News.Load();
	AntiAFK.Load();
	GoldenArcher.Load();
	Vip.Load();
	Monster.LoadPkClear();
	DropSystem.LoadDropItems();
	MapSystem.Load();
	MoveReq.MoveReqLoad();
	ZenFixes();
	LoadNotice();
	LoadCommands();
	Misc();
	HappyHour.LoadConfig();
#ifdef _GS
	DuelSystem.Load();
	Moss.Load();
	DropEvent.Init();
#endif 
}

void cConfigs::ZenFixes()
{
	Zen.NormalZen					= GetInt(0, 65535,				20,		"ZenSettings",	"ZenDrop",			IAJuliaCommon);
	Zen.ZenInParty					= GetInt(0, 65535,				20,		"ZenSettings",	"ZenPartyDrop",		IAJuliaCommon);
}

void cConfigs::LoadPets()
{		  																									   
	Panda.PetPandaDefense				= GetInt(0, 1000,			10,		"Panda",		"PetPandaDefense",				IAJuliaPets);
	Panda.PetPandaAttackDamageMinLeft	= GetInt(0, 1000,			10,		"Panda",		"PetPandaAttackDamageMinLeft",	IAJuliaPets);
	Panda.PetPandaAttackDamageMaxLeft	= GetInt(0, 1000,			10,		"Panda",		"PetPandaAttackDamageMaxLeft",	IAJuliaPets);
	Panda.PetPandaAttackDamageMinRight	= GetInt(0, 1000,			10,		"Panda",		"PetPandaAttackDamageMinRight",	IAJuliaPets);
	Panda.PetPandaAttackDamageMaxRight	= GetInt(0, 1000,			10,		"Panda",		"PetPandaAttackDamageMaxRight",	IAJuliaPets);
	Panda.PetPandaMagicDamageMin		= GetInt(0, 1000,			10,		"Panda",		"PetPandaMagicDamageMin",		IAJuliaPets);
	Panda.PetPandaMagicDamageMax		= GetInt(0, 1000,			10,		"Panda",		"PetPandaMagicDamageMax",		IAJuliaPets);
	Panda.PetPandaMagicSpeed			= GetInt(0, 1000,			10,		"Panda",		"PetPandaMagicSpeed",			IAJuliaPets);
	Panda.PetPandaAttackSpeed			= GetInt(0, 1000,			10,		"Panda",		"PetPandaAttackSpeed",			IAJuliaPets);

	Panda.PandaRingDefense				= GetInt(0, 1000,			10,		"PandaRing",	"PandaRingDefense",				IAJuliaPets);
	Panda.PandaRingAttackDamageMinLeft	= GetInt(0, 1000,			10,		"PandaRing",	"PandaRingAttackDamageMinLeft",	IAJuliaPets);
	Panda.PandaRingAttackDamageMaxLeft	= GetInt(0, 1000,			10,		"PandaRing",	"PandaRingAttackDamageMaxLeft",	IAJuliaPets);
	Panda.PandaRingAttackDamageMinRight	= GetInt(0, 1000,			10,		"PandaRing",	"PandaRingAttackDamageMinRight",IAJuliaPets);
	Panda.PandaRingAttackDamageMaxRight	= GetInt(0, 1000,			10,		"PandaRing",	"PandaRingAttackDamageMaxRight",IAJuliaPets);
	Panda.PandaRingMagicDamageMin		= GetInt(0, 1000,			10,		"PandaRing",	"PandaRingMagicDamageMin",		IAJuliaPets);
	Panda.PandaRingMagicDamageMax		= GetInt(0, 1000,			10,		"PandaRing",	"PandaRingMagicDamageMax",		IAJuliaPets);
	Panda.PandaRingMagicSpeed			= GetInt(0, 1000,			10,		"PandaRing",	"PandaRingMagicSpeed",			IAJuliaPets);
	Panda.PandaRingAttackSpeed			= GetInt(0, 1000,			10,		"PandaRing",	"PandaRingAttackSpeed",			IAJuliaPets);
																										  
	Panda.PetPandaExpirence				= GetInt(0, 10000,			100,	"Panda",		"PetPandaExpirence",			IAJuliaPets);
	Panda.PetPandaMLExpirence			= GetInt(0, 10000,			100,	"Panda",		"PetPandaMLExpirence",			IAJuliaPets);

	Panda.PandaRingExpirence			= GetInt(0, 10000,			100,	"PandaRing",	"PandaRingExpirence",			IAJuliaPets);
	Panda.PandaRingMLExpirence			= GetInt(0, 10000,			100,	"PandaRing",	"PandaRingMLExpirence",			IAJuliaPets);
}

void cConfigs::LoadNotice()
{	
	ConnectInfo					= GetInt(0, 1,					1,		"Connect",		"ConnectInfo",					IAJuliaGS);
	GetPrivateProfileString("Connect","ConnectNotice","Powered by IA Julia 1.1.x",ConnectNotice,sizeof(ConnectNotice),IAJuliaGS); 
	Log.CheckProcent(ConnectNotice);	 
} 

void cConfigs::LoadConfigsInGS()
{								  	 

	#ifdef _GS
	DWORD *LoreGuard = (DWORD*)GUARD_SAY;
	char Lore[25];
	GetPrivateProfileString("Connect","GuardSay","Don't waste my time!",Lore,25,IAJuliaGS);
	memset(&LoreGuard[0],0,25);
	memcpy(&LoreGuard[0],Lore,strlen(Lore));
	#endif

	DWORD dword;
	BYTE byte;

	dword = GetInt(300, 1000, 400,"LevelSettings", "MaxLevel", IAJuliaCommon);
	*(unsigned int*) GS_MAX_LEVEL1 = dword;
	*(unsigned int*) GS_MAX_LEVEL2 = dword;
	*(unsigned int*) GS_MAX_LEVEL3 = dword;
	*(unsigned int*) GS_MAX_LEVEL4 = dword;
	*(unsigned int*) GS_MAX_LEVEL5 = dword;
	Commands.MaxLvl	= dword;

	*(unsigned int*) GS_NOEXP_LEVEL = GetInt(401, 1001, 401,"LevelSettings", "MaxXPLevel", IAJuliaCommon);
	*(unsigned int*) GS_MAX_MASTERLEVEL = GetInt(1, 400, 200,"LevelSettings", "MaxMasterLevel", IAJuliaCommon);

	dword = GetInt(0, 360, 120,"ItemDropRates", "LootingTime", IAJuliaItems);
	*(unsigned int*) GS_ITEM_TIME1 =	1000 * dword;
	*(unsigned int*) GS_ITEM_TIME2 =	1000 * dword;

	*(unsigned int*) GS_TRANSFORMATIONRING1 = GetInt(0, 600, 2,"TransformationRings","TransformRing1",IAJuliaItems);
	*(unsigned int*) GS_TRANSFORMATIONRING2 = GetInt(0, 600, 7,"TransformationRings","TransformRing2",IAJuliaItems);
	*(unsigned int*) GS_TRANSFORMATIONRING3 = GetInt(0, 600, 14,"TransformationRings","TransformRing3",IAJuliaItems);
	*(unsigned int*) GS_TRANSFORMATIONRING4 = GetInt(0, 600, 8,"TransformationRings","TransformRing4",IAJuliaItems);
	*(unsigned int*) GS_TRANSFORMATIONRING5 = GetInt(0, 600, 9,"TransformationRings","TransformRing5",IAJuliaItems);
	*(unsigned int*) GS_TRANSFORMATIONRING6 = GetInt(0, 600, 41,"TransformationRings","TransformRing6",IAJuliaItems);

	*(unsigned char*) GS_SUMMONORB1 = GetChar(0, 600, 26,"SummonOrbs","OrbSummon1",IAJuliaItems);
	*(unsigned char*) GS_SUMMONORB2 = GetChar(0, 600, 32,"SummonOrbs","OrbSummon2",IAJuliaItems);
	*(unsigned char*) GS_SUMMONORB3 = GetChar(0, 600, 21,"SummonOrbs","OrbSummon3",IAJuliaItems);
	*(unsigned char*) GS_SUMMONORB4 = GetChar(0, 600, 20,"SummonOrbs","OrbSummon4",IAJuliaItems);
	*(unsigned char*) GS_SUMMONORB5 = GetChar(0, 600, 10,"SummonOrbs","OrbSummon5",IAJuliaItems);
	*(unsigned char*) GS_SUMMONORB6 = GetChar(0, 600, 150,"SummonOrbs","OrbSummon6",IAJuliaItems);
	*(unsigned char*) GS_SUMMONORB7 =  GetChar(0, 600, 151,"SummonOrbs","OrbSummon7",IAJuliaItems);

	
	*(unsigned int*) GS_BLESS_PRICE = GetInt(0, 2000000000, 6000000,"JewelPrices","JewelOfBlessPrice",IAJuliaItems);
 	*(unsigned int*) GS_SOUL_PRICE = GetInt(0, 2000000000, 9000000,"JewelPrices","JewelOfSoulPrice",IAJuliaItems);
	*(unsigned int*) GS_CHAOS_PRICE = GetInt(0, 2000000000, 810000,"JewelPrices","JewelOfChaosPrice",IAJuliaItems);
	*(unsigned int*) GS_LIFE_PRICE =  GetInt(0, 2000000000, 45000000,"JewelPrices","JewelOfLifePrice",IAJuliaItems);
	*(unsigned int*) GS_CREATION_PRICE = GetInt(0, 2000000000, 36000000,"JewelPrices","JewelOfCreationPrice",IAJuliaItems);
	*(unsigned int*) GS_GUARDIAN_PRICE = GetInt(0, 2000000000, 60000000,"JewelPrices","JewelOfGuardianPrice",IAJuliaItems);
	*(unsigned int*) GS_FRUITS_PRICE = GetInt(0, 2000000000, 33000000,"JewelPrices","FruitPrice",IAJuliaItems);
	*(unsigned int*) GS_MONARCH_PRICE = GetInt(0, 2000000000, 750000,"JewelPrices","CrestOfMonarchPrice",IAJuliaItems);
	*(unsigned int*) GS_FEATHER_PRICE = GetInt(0, 2000000000, 180000,"JewelPrices","LochsFeatherPrice",IAJuliaItems);
	*(unsigned int*) GS_BLESSPOT_PRICE = GetInt(0, 2000000000, 900000,"JewelPrices","PotionOfBlessPrice",IAJuliaItems);
	*(unsigned int*) GS_SOULPOT_PRICE = GetInt(0, 2000000000, 450000,"JewelPrices","PotionOfSoulPrice",IAJuliaItems);
#ifdef _GS
	*(unsigned int*) GS_KUNDUN_ANC_PERCT = GetInt(0,10000,25,"Kundun","KundunDropAncRate",IAJuliaItems);
	*(unsigned char*) GS_KUNDUN_ITEM_NUMB = GetChar(0,20,3,"Kundun","KundunDropItemCount",IAJuliaItems);

	*(unsigned char*) GS_CCPLAYER = GetChar(0,20,2,"ChaosCastle","ChaosCastleMinPlayers",IAJuliaEvents); 
	*(unsigned char*) GS_ITPLAYER = GetChar(0,20,4,"IllusionTemple","ItMinPlayers",IAJuliaEvents); 
	/*
	*(unsigned char*) GS_CCREWARD1 = GetChar(0,15,14,"ChaosCastle","ChaosCastleRewardType1GroupID",IAJuliaEvents) * 512 + GetChar(0,255,16,"ChaosCastle","ChaosCastleRewardType1IndexID",IAJuliaEvents);
	*(unsigned char*) GS_CCREWARD2 = GetChar(0,15,14,"ChaosCastle","ChaosCastleRewardType2GroupID",IAJuliaEvents) * 512 + GetChar(0,255,13,"ChaosCastle","ChaosCastleRewardType2IndexID",IAJuliaEvents);
	*(unsigned char*) GS_CCREWARD3 = GetChar(0,15,14,"ChaosCastle","ChaosCastleRewardType3GroupID",IAJuliaEvents) * 512 + GetChar(0,255,22,"ChaosCastle","ChaosCastleRewardType3IndexID",IAJuliaEvents);
	*(unsigned char*) GS_CCREWARD4 = GetChar(0,15,14,"ChaosCastle","ChaosCastleRewardType4GroupID",IAJuliaEvents) * 512 + GetChar(0,255,14,"ChaosCastle","ChaosCastleRewardType4IndexID",IAJuliaEvents); 
	
	*(unsigned char*) GS_IT_GROUP_ID = GetChar(0,15,15,"IllusionTemple","ItDropGroup",IAJuliaEvents);
	*(unsigned char*) GS_IT_DROP_ID = GetChar(0,255,12,"IllusionTemple","ItDropID",IAJuliaEvents); 
	*(unsigned char*) GS_IT_ITEM_LVL = GetChar(0,15,0,"IllusionTemple","ItDropLevel",IAJuliaEvents);
	*(unsigned char*) GS_IT_ITEM_SKL =  GetChar(0,1,0,"IllusionTemple","ItDropWithSkill",IAJuliaEvents);
	*(unsigned char*) GS_IT_ITEM_LCK = GetChar(0,1,0,"IllusionTemple","ItDropWithLuck",IAJuliaEvents);
	*(unsigned char*) GS_IT_ITEM_LIF = GetChar(0,7,0,"IllusionTemple","ItDropLifeAdd",IAJuliaEvents);
	*(unsigned char*) GS_IT_ITEM_EXC = GetChar(0,63,0,"IllusionTemple","ItDropExcOpt",IAJuliaEvents);
	*(unsigned char*) GS_IT_ITEM_ANC = GetChar(0,255,0,"IllusionTemple","ItDropAncOpt",IAJuliaEvents); 

	*(unsigned char *) GS_BC_DROP_GROUP = GetChar(0,15,15,"BloodCastle","BcDropGroup",IAJuliaEvents);
	*(unsigned char *) GS_BC_DROP_ID = GetChar(0,255,12,"BloodCastle","BcDropID",IAJuliaEvents);
	*(unsigned char *) GS_BC_ITEM_LVL = GetChar(0,15,0,"BloodCastle","BcDropLevel",IAJuliaEvents);
	*(unsigned char *) GS_BC_ITEM_SKL = GetChar(0,1,0,"BloodCastle","BcDropWithSkill",IAJuliaEvents);
	*(unsigned char *) GS_BC_ITEM_LCK = GetChar(0,1,0,"BloodCastle","BcDropWithLuck",IAJuliaEvents);
	*(unsigned char *) GS_BC_ITEM_LIF = GetChar(0,7,0,"BloodCastle","BcDropLifeAdd",IAJuliaEvents);
	*(unsigned char *) GS_BC_ITEM_EXC = GetChar(0,63,0,"BloodCastle","BcDropExcOpt",IAJuliaEvents);
	*(unsigned char *) GS_BC_ITEM_ANC = GetChar(0,255,0,"BloodCastle","BcDropAncOpt",IAJuliaEvents); 

	*(unsigned char *) GS_WW_GROUP_ID = GetChar(0,15,13,"WhiteWizard","WizardDropGroup",IAJuliaEvents);
	*(unsigned char *) GS_WW_DROP_ID = GetChar(0,255,20,"WhiteWizard","WizardDropItemID",IAJuliaEvents);
	*(unsigned char *) GS_WW_ITEM_LVL = GetChar(0,15,0,"WhiteWizard","WizardDropLevel",IAJuliaEvents);
	*(unsigned char *) GS_WW_ITEM_SKL = GetChar(0,1,0,"WhiteWizard","WizardDropWithSkill",IAJuliaEvents);
	*(unsigned char *) GS_WW_ITEM_LCK = GetChar(0,1,0,"WhiteWizard","WizardDropWithLuck",IAJuliaEvents);
	*(unsigned char *) GS_WW_ITEM_LIF = GetChar(0,7,0,"WhiteWizard","WizardDropLifeAdd",IAJuliaEvents);
	*(unsigned char *) GS_WW_ITEM_EXC = GetChar(0,63,0,"WhiteWizard","WizardDropExcOpt",IAJuliaEvents);
	*(unsigned char *) GS_WW_ITEM_ANC = GetChar(0,255,0,"WhiteWizard","WizardDropAncOpt",IAJuliaEvents);
	*/
#endif
	*(unsigned char*) GS_GUILDALLIANCE = GetChar(0,50,20,"Guild","GuildAllianceMinPlayers",IAJuliaCommon);

	*(unsigned char*) GS_PKBugLimitFix1 = GetChar(0, 1000, 20,"PKOptions", "PKKillLimit", IAJuliaCommon);
    *(unsigned char*) GS_PKBugLimitFix2 = GetChar(0, 1000, 20,"PKOptions", "PKKillLimit", IAJuliaCommon);
    *(unsigned char*) GS_PKBugLimitFix3 = GetChar(0, 1000, 20,"PKOptions", "PKKillLimit", IAJuliaCommon);

	*(unsigned int*) GS_UDP_PORT = GetInt(0, 99999, 60006,"Connect","UDP-Port",IAJuliaGS);

	*(unsigned char*) GS_SOUL_RATE_NORMAL = GetChar(0,100,60,"JewelRates","SoulSuccessRateNormal",IAJuliaItems);
	*(unsigned char*) GS_SOUL_RATE_SPECIAL = GetChar(0,100,50,"JewelRates","SoulSuccessRateSpecial",IAJuliaItems);
	*(unsigned char*) GS_SOUL_RATE_SOCKET = GetChar(0,100,40,"JewelRates","SoulSuccessRateSocket",IAJuliaItems);
	*(unsigned char*) GS_SOUL_LUCK_ADD = GetChar(0,100,20,"JewelRates","SoulSuccessRateLuck",IAJuliaItems);
	*(unsigned char*) GS_JOL_FAIL_RATE = GetChar(0,100,50,"JewelRates","LifeFailRate",IAJuliaItems);

	byte = GetChar(0, 100, 60,"ChaosMachineMixes", "ChaosMix10Rate", IAJuliaItems);
	*(unsigned char*)  GS_CM_MIX_10_NORMAL = byte;
	*(unsigned char*)  GS_CM_MIX_10_SPECIAL = byte;
	*(unsigned char*)  GS_CM_MIX_10_SOCKET = byte;
	byte = GetChar(0, 100, 50,"ChaosMachineMixes", "ChaosMix11_12Rate", IAJuliaItems);
	*(unsigned char*)  GS_CM_MIX_11_12_NORMAL = byte;
	*(unsigned char*)  GS_CM_MIX_11_12_SPECIAL = byte;
	*(unsigned char*)  GS_CM_MIX_11_12_SOCKET = byte;
	byte = GetChar(0, 100, 40,"ChaosMachineMixes", "ChaosMix13Rate", IAJuliaItems);
	*(unsigned char*)  GS_CM_MIX_13_NORMAL = byte;
	*(unsigned char*)  GS_CM_MIX_13_SPECIAL = byte;
	*(unsigned char*)  GS_CM_MIX_13_SOCKET = byte;
	byte = GetChar(0, 100, 75,"ChaosMachineMixes", "ChaosMixWithLuck", IAJuliaItems);
	*(unsigned char*) GS_CM_MIX_10_LUCK = byte;
	*(unsigned char*) GS_CM_MIX_11_13_LUCK = byte;
	*(unsigned char*) GS_CM_MIX_LUCK_ADD = GetChar(0, 100, 20,"ChaosMachineMixes","ChaosMixRateIncreaseWithLuck",IAJuliaItems);
	*(unsigned char*) DEFAULTMIXRATE = GetChar(0, 100, 45,"ChaosMachineMixes","ChaosMixDefaultRate",IAJuliaItems);
	*(unsigned char*) GS_WINGLVL1MIX = GetChar(0, 100, 90,"ChaosMachineMixes","WingMixLvl1SuccessRate",IAJuliaItems);
	*(unsigned char*) GS_WINGLVL2MIX = GetChar(0, 100, 90,"ChaosMachineMixes","WingMixLvl2SuccessRate",IAJuliaItems);
	byte = GetChar(0, 100, 60,"ChaosMachineMixes","CondorMixSuccessRate",IAJuliaItems);
	*(unsigned char *) GS_WINGLVL3MIX11 = byte;
	*(unsigned char *) GS_WINGLVL3MIX12 = byte;
	byte = GetChar(0, 100, 40,"ChaosMachineMixes","WingMixLvl3SuccessRate",IAJuliaItems);
	*(unsigned char *) GS_WINGLVL3MIX21 = byte;
	*(unsigned char *) GS_WINGLVL3MIX22 = byte;

	*(unsigned char*) GS_SETITEMMIX1 = GetChar(0, 100, 80,"ChaosMachineMixes","380LevelMixSuccessRate",IAJuliaItems);
	*(unsigned char*) GS_SETITEMMIX2 = GetChar(0, 100, 80,"ChaosMachineMixes","380LevelMixSuccessRate",IAJuliaItems);
	*(unsigned int*) GS_DINO_MIX = GetInt(0, 100, 70,"ChaosMachineMixes","DinorantMixSuccessRate",IAJuliaItems);
	
	*(unsigned int*) GS_DARK_HORSE = GetInt(0, 100, 60,"DLPets","DarkHorseMixSuccessRate",IAJuliaItems);
	*(unsigned int*) GS_DARK_HORSE_PRICE = GetInt(0, 2000000000, 5000000,"DLPets","DarkHorseMixPrice",IAJuliaItems);
   	*(unsigned int*) GS_DARK_SPIRIT = GetInt(0, 100, 60,"DLPets","DarkSpiritMixSuccessRate",IAJuliaItems);
	*(unsigned int*) GS_DARK_SPIRIT_PRICE = GetInt(0, 2000000000, 1000000,"DLPets","DarkSpiritMixPrice",IAJuliaItems);

	//Percentage for EXERate 0 - 100%
	int EXERate = GetInt(0, 100, 10,"ItemDropRates","ExcDropRate",IAJuliaItems);
	*(unsigned int*) GS_EXC_DROP_RATE = 2000 - (20 * EXERate);
	//
	*(unsigned int*) GS_EXC_SKILL_RATE = GetInt(0, 100, 100,"ItemDropRates","ExcSkillDropRate",IAJuliaItems);
	*(unsigned int*) GS_EXC_LUCK_RATE = GetInt(0, 100, 1,"ItemDropRates","ExcLuckDropRate",IAJuliaItems);
	*(unsigned int*) GS_SKILL_RATE = GetInt(0, 100, 6,"ItemDropRates","NormalSkillDropRate",IAJuliaItems);
	*(unsigned int*) GS_LUCK_RATE = GetInt(0, 100, 4,"ItemDropRates","NormalLuckDropRate",IAJuliaItems);
	*(unsigned char*) GS_ANCREWARD_SKILL_RATE = GetChar(0, 100, 6,"ItemDropRates","AncientRewardSkillDropRate",IAJuliaItems);
	*(unsigned char*) GS_ANCREWARD_LUCK_RATE = GetChar(0, 100, 4,"ItemDropRates","AncientRewardLuckDropRate",IAJuliaItems);
	*(unsigned char*) GS_ANCRANDOM_SKILL_RATE = GetChar(0, 100, 6,"ItemDropRates","AncientRandomSkillDropRate",IAJuliaItems);
	*(unsigned char*) GS_ANCRANDOM_LUCK_RATE = GetChar(0, 100, 4,"ItemDropRates","AncientRandomLuckDropRate",IAJuliaItems);


	byte = GetChar(0, 20, 7,"LevelUpPoints","LevelUpPointsForDl",IAJuliaCommon);
	*(unsigned char*) GS_LVL_UP_DL1 = byte;
	*(unsigned char*) GS_LVL_UP_DL2 = byte;
	*(unsigned char*) GS_LVL_UP_DL3 = byte;
	*(unsigned char*) GS_LVL_UP_DL4 = byte;
	*(unsigned char*) GS_LVL_UP_DL5 = byte;

	byte = GetChar(0, 20, 7,"LevelUpPoints","LevelUpPointsForMg",IAJuliaCommon);
	*(unsigned char*) GS_LVL_UP_MG1 = byte;
	*(unsigned char*) GS_LVL_UP_MG2 = byte;
	*(unsigned char*) GS_LVL_UP_MG3 = byte;
	*(unsigned char*) GS_LVL_UP_MG4 = byte;
	*(unsigned char*) GS_LVL_UP_MG5 = byte;

	byte = GetChar(0, 20, 5,"LevelUpPoints","LevelUpPointsForDkElfDwSum",IAJuliaCommon);
	*(unsigned char*) GS_LVL_UP_NORMAL1 = byte;
	*(unsigned char*) GS_LVL_UP_NORMAL2 = byte;
	*(unsigned char*) GS_LVL_UP_NORMAL3 = byte;
	*(unsigned char*) GS_LVL_UP_NORMAL4 = byte;
	*(unsigned char*) GS_LVL_UP_NORMAL5 = byte;

	byte = GetChar(0, 5, 1,"LevelUpPoints","ExtraPointsAfterMarlonQuest",IAJuliaCommon);
	*(unsigned char*) GS_LVL_UP_QUEST1 = byte;
	*(unsigned char*) GS_LVL_UP_QUEST2 = byte;
	*(unsigned char*) GS_LVL_UP_QUEST3 = byte;
	*(unsigned char*) GS_LVL_UP_QUEST4 = byte;
	*(unsigned char*) GS_LVL_UP_QUEST5 = byte;

	*(unsigned char*) GS_ML_POINTS = GetChar(0, 20, 1,"LevelUpPoints","MasterlvlPoints",IAJuliaCommon);

	*(unsigned int *) GS_TWISTINGSLASH_USELV = GetInt(0,5000,80,"SkillLevel", "TwistingSlashMinLevel", IAJuliaSkills);
	*(unsigned int *) GS_RAGEFULBLOW_USELV = GetInt(0,5000,170,"SkillLevel", "RagefulBlowMinLevel", IAJuliaSkills);
	*(unsigned int *) GS_DEATHSTAB_USELV = GetInt(0,5000,160,"SkillLevel", "DeathStabMinLevel", IAJuliaSkills);
	*(unsigned int *) GS_IMPALE_USELV = GetInt(0,5000,25,"SkillLevel", "ImapleMinLevel", IAJuliaSkills) ;
	*(unsigned int *) GS_INNERSTRENGTH_USELV = GetInt(0,5000,120,"SkillLevel", "GreaterFortitudeMinLevel", IAJuliaSkills) ;
	*(unsigned int *) GS_PENETRATION_USELV = GetInt(0,5000,130,"SkillLevel", "PenetrationMinLevel", IAJuliaSkills) ;

	*(unsigned int *) GS_MSNORMAL01 = GetInt(0,10000,250,"ManaShield", "ManaShieldAgiNormal", IAJuliaSkills) ;
	*(unsigned int *) GS_MSNORMAL02 = GetInt(0,10000,200,"ManaShield", "ManaShieldEneNormal", IAJuliaSkills) ;
	*(unsigned int *) GS_MSMASTER01 = GetInt(0,10000,50,"ManaShield", "ManaShieldAgiMaster", IAJuliaSkills) ;
	*(unsigned int *) GS_MSMASTER02 = GetInt(0,10000,50,"ManaShield", "ManaShieldEneMaster", IAJuliaSkills) ; 

	*(unsigned int *) NormalManaShieldTime1 = GetInt(0,10000,40,"ManaShield", "ManaShieldTime", IAJuliaSkills) ;
	*(unsigned int *) MasterManaShieldTime2 = GetInt(0,10000,40,"ManaShield", "ManaShieldTimeMaster", IAJuliaSkills) ;


	dword =  GetInt(0,5000,150,"GreatFortitude", "GreatFortitudeVitNormal", IAJuliaSkills);
	*(unsigned int *) NormalGreatForitiuteVit01 = dword;
	*(unsigned int *) NormalGreatForitiuteVit02 = dword;
	*(unsigned int *) NormalGreatForitiuteVit03 = dword;
	*(unsigned int *) GS_GFNORMAL02 = GetInt(0,5000,100,"GreatFortitude", "GreatFortitudeEneNormal", IAJuliaSkills) ;

	dword =  GetInt(0,5000,150,"GreatFortitude", "GreatFortitudeVitMaster", IAJuliaSkills);
	*(unsigned int *) MasterGreatForitiuteVit01 = dword;
	*(unsigned int *) MasterGreatForitiuteVit02 = dword;
	*(unsigned int *) MasterGreatForitiuteVit03 = dword;
	*(unsigned int *) GS_GFMASTER02 = GetInt(0,5000,100,"GreatFortitude", "GreatFortitudeEneMaster", IAJuliaSkills) ;

	*(unsigned int *) NormalGreatForitiuteTime = GetInt(0,10000,10,"GreatFortitude", "GreatFortitudeTime", IAJuliaSkills) ;
	*(unsigned int *) MasterGreatForitiuteTime = GetInt(0,10000,10,"GreatFortitude", "GreatFortitudeMaster", IAJuliaSkills) ;


	*(unsigned int*) GS_BERSERKER_DIV1 = GetInt(0,5000,20,"Berserker","BerserkerDiv1",IAJuliaSkills);
	*(unsigned int*) GS_BERSERKER_DIV2 = GetInt(0,5000,30,"Berserker","BerserkerDiv2",IAJuliaSkills);
	*(unsigned int*) GS_BERSERKER_DIV3 = GetInt(0,5000,60,"Berserker","BerserkerDiv3",IAJuliaSkills);
	*(unsigned int*) GS_DAMAGEREF_DIV1 = GetInt(0,5000,42,"DamageRef","DamageRefDiv1",IAJuliaSkills);
	*(unsigned int*) GS_DAMAGEREF_DIV2 = GetInt(0,5000,25,"DamageRef","DamageRefDiv2",IAJuliaSkills);

	*(unsigned int*) GS_CRITICALDMG_COMMAND = GetInt(0,5000,25,"CriticalDmg","CriticalDmgLeaderShip",IAJuliaSkills);
	*(unsigned int*) GS_CRITICALDMG_ENERGY = GetInt(0,5000,30,"CriticalDmg","CriticalDmgEnergy",IAJuliaSkills);
	*(unsigned int*) GS_CRITICALDMG_TIME = GetInt(0,5000,10,"CriticalDmg","CriticalDmgTime",IAJuliaSkills);

	*(unsigned int*) MaxZen  = GetInt(0, 2000000000, 2000000000,"MaximumZen", "MaxZenInBag", IAJuliaCommon);

	dword = GetInt(0, 2000000000, 1000000000,"MaximumZen", "MaxZenInVault", IAJuliaCommon);
	*(unsigned int*) MaxZen1 = dword;
	*(unsigned int*) MaxZen2 = dword;
	*(unsigned int*) MaxZen3 = dword;

	*(unsigned int *) GS_PARTYEXP_2_NORMAL = GetInt(0,5000,160,"PartyExp","NormalParty2Exp",IAJuliaCommon);
	*(unsigned int *) GS_PARTYEXP_3_NORMAL = GetInt(0,5000,180,"PartyExp","NormalParty3Exp",IAJuliaCommon);
	*(unsigned int *) GS_PARTYEXP_4_NORMAL = GetInt(0,5000,200,"PartyExp","NormalParty4Exp",IAJuliaCommon);
	*(unsigned int *) GS_PARTYEXP_5_NORMAL = GetInt(0,5000,220,"PartyExp","NormalParty5Exp",IAJuliaCommon);
	*(unsigned int *) GS_PARTYEXP_3_SET = GetInt(0,5000,230,"PartyExp","SetParty3Exp",IAJuliaCommon);
	*(unsigned int *) GS_PARTYEXP_4_SET = GetInt(0,5000,270,"PartyExp","SetParty4Exp",IAJuliaCommon);
	*(unsigned int *) GS_PARTYEXP_5_SET  = GetInt(0,5000,300,"PartyExp","SetParty5Exp",IAJuliaCommon);
#ifdef _GS
	*(unsigned int*) GS_DS_PARTYEXP_2 = GetInt(0,5000,160,"DevilSquare","DevilSquarePartyExp2Players",IAJuliaEvents);
	*(unsigned int*) GS_DS_PARTYEXP_3 = GetInt(0,5000,180,"DevilSquare","DevilSquarePartyExp3Players",IAJuliaEvents);
	*(unsigned int*) GS_DS_PARTYEXP_4 = GetInt(0,5000,200,"DevilSquare","DevilSquarePartyExp4Players",IAJuliaEvents);
	*(unsigned int*) GS_DS_PARTYEXP_5 = GetInt(0,5000,220,"DevilSquare","DevilSquarePartyExp5Players",IAJuliaEvents);
	*(unsigned int*) GS_DS_PARTYDIFFEXP_3 = GetInt(0,5000,230,"DevilSquare","DevilSquareSetPartyExp3Players",IAJuliaEvents);
	*(unsigned int*) GS_DS_PARTYDIFFEXP_4 = GetInt(0,5000,270,"DevilSquare","DevilSquareSetPartyExp4Players",IAJuliaEvents);
	*(unsigned int*) GS_DS_PARTYDIFFEXP_5 = GetInt(0,5000,300,"DevilSquare","DevilSquareSetPartyExp5Players",IAJuliaEvents);
#endif
#ifdef _GS_CS
	*(unsigned int*) GSCS_LOT_MAXPRICE = GetInt(0,1000000000,300000,"CastleSiege","CSLandOfTrialMaxPayment",IAJuliaEvents);
	*(unsigned char*) GSCS_LORDMIX_MAXUSE  = GetChar(0,5000,0,"CastleSiege","CSLordMixMaxUsePerDay",IAJuliaEvents);

	*(unsigned char*) GSCS_CRYWOLF_REWARD_ELF = GetChar(0,15,14,"CryWolf","CrywolfAltarElfRewardGroupID",IAJuliaEvents) * 512 + GetChar(0,255,13,"CryWolf","CrywolfAltarElfRewardIndexID",IAJuliaEvents);
	*(unsigned char*) GSCS_CRYWOLF_REWARD_TOP5 = GetChar(0,15,14,"CryWolf","CrywolfTop5RewardGroupID",IAJuliaEvents) * 512 + GetChar(0,255,13,"CryWolf","CrywolfTop5RewardIndexID",IAJuliaEvents);
	
	dword = GetInt(0,1000000000,200000000,"CastleSiege","CSSeniorMaxCash",IAJuliaEvents);
	*(unsigned int*) GSCS_SENIOR_MAXZEN1 = dword;
	*(unsigned int*) GSCS_SENIOR_MAXZEN2 = dword;
#endif 
}	
void cConfigs::LoadFixes()
{
	#ifdef _GS
	EldaradoEventType	= GetInt(0, 1, 0,"EldaradoEvent", "EldaradoEventType", IAJuliaEvents);
	CCAllowingPlayers	= GetInt(0, 2, 0,"ChaosCastle", "ChaosCastleAllowingPlayers", IAJuliaEvents);
	BCAllowingPlayers	= GetInt(0, 2, 0,"BloodCastle", "BloodCastleAllowingPlayers", IAJuliaEvents);
	BCEnterMasterType	= GetInt(0, 1, 0,"BloodCastle", "BloodCastleMasterEnterType", IAJuliaEvents);
	DSAllowingPlayers	= GetInt(0, 2, 0,"DevilSquare", "DevilSquareAllowingPlayers", IAJuliaEvents); 
	#endif

	UseChecksum		= GetInt(0, 1, 0, "CheckSum", "UseCheckSum", IAJuliaCommon);	  
	PersonalIDFix	= GetInt(0, 1, 1, "Bypasseres", "PersonalIDBypasser", IAJuliaCommon);
	GuildIDFix		= GetInt(0, 1, 1, "Bypasseres", "GuildIDBypasser", IAJuliaCommon);
	DisableLogs		= GetInt(0, 1, 0, "Logs", "DisableLogs", IAJuliaCommon);
	AllowExeAnc		= GetInt(0, 1, 1, "SpecialItem", "AllowExeAnc", IAJuliaItems);	
	AllowJohAnc		= GetInt(0, 1, 1, "SpecialItem", "AllowJohAnc", IAJuliaItems);
	AllowExeSock	= GetInt(0, 1, 1, "SpecialItem", "AllowExeSock", IAJuliaItems);
	MaxLifeOpt		= GetInt(0, 1, 1, "SpecialItem", "MaxLifeOpt", IAJuliaItems);
	CSSkillsOnNOrmal = GetInt(0, 1, 1, "Skills", "CSSkillsOnNOrmal", IAJuliaSkills);    
	Enable65kStats	= GetInt(0, 1, 0, "AddCommand",	"Enable65kStats", IAJuliaCmd);	
	DumpFile		= GetInt(0, 1, 0, "Dump", "EnableCrashDump",IAJuliaCommon);
}

void cConfigs::LoadCommands()
{	
	//		drop		//
	Commands.IsDrop						= GetInt(0,	1,					1,		"Drop",			"DropEnabled",					IAJuliaCmd);

	//		Reload		//
	Commands.IsReload					= GetInt(0,	1,					1,		"Reload",			"ReloadEnabled",					IAJuliaCmd);

	//		gmove		//
	Commands.IsGmove					= GetInt(0,	1,					1,		"Gmove",		"GmoveEnabled",					IAJuliaCmd);	 

	//		setpk
	Commands.IsSetPK					= GetInt(0, 1,					1,		"SetPK",		"SetPKEnabled",					IAJuliaCmd);  

	//		gg			//																	 	
	Commands.IsGg						= GetInt(0, 1,					1,		"GPost",		"GgEnabled",					IAJuliaCmd);	

	//		time		//
	Commands.IsTime						= GetInt(0, 1,					1,		"Time",			"TimeEnabled",					IAJuliaCmd);  

	//		online		//
	Commands.IsOnline					= GetInt(0, 1,					1,		"OnlineCommand","OnlineEnabled",				IAJuliaCmd); 

	//		status		//
	Commands.IsStatus					= GetInt(0, 1,					1,		"Status",		"StatusEnabled",				IAJuliaCmd); 

	//		SetChar		//
	Commands.IsSetChar					= GetInt(0, 1,					1,		"SetChar",		"SetCharEnabled",				IAJuliaCmd);
	//		bans		//
	Commands.IsBanPost					= GetInt(0, 1,					1,		"Bans",			"BanPostEnabled",				IAJuliaCmd);	
	Commands.IsBanChar					= GetInt(0, 1,					1,		"Bans",			"BanCharEnabled",				IAJuliaCmd);	
	Commands.IsBanAcc					= GetInt(0, 1,					1,		"Bans",			"BanAccEnabled",				IAJuliaCmd);  	

	//		skin		//
	Commands.SkinEnabled				= GetInt(0, 1,									1,		"Skin",			"SkinEnabled",					IAJuliaCmd);
	Commands.SkinOnlyForGm				= GetInt(0, 1,									0,		"Skin",			"SkinOnlyForGm",				IAJuliaCmd);
	Commands.SkinLevelReq				= GetInt(0, Commands.MaxLvl,					250,	"Skin",			"SkinLevelReq",					IAJuliaCmd);
	Commands.SkinPriceZen				= GetInt(0, 2000000000,							100000,	"Skin",			"SkinPriceZen",					IAJuliaCmd);
	Commands.SkinPricePcPoint			= GetInt(0, PCPoint.Config.MaximumPCPoints,	1,		"Skin",			"SkinPricePcPoint",				IAJuliaCmd);
	Commands.SkinPriceWCoin				= GetInt(0, PCPoint.Config.MaximumWCPoints,	1,		"Skin",			"SkinPriceWCoin",				IAJuliaCmd);	

	//		post		//	 
	Commands.IsPost						= GetInt(0, 1,									1,		"Post",			"PostEnabled",					IAJuliaCmd); 
	Commands.PostLvl					= GetInt(0, Commands.MaxLvl,					1,		"Post",			"PostLevelReq",					IAJuliaCmd);				  
	Commands.PostPriceZen				= GetInt(0, 2000000000,							10000,	"Post",			"PostPriceZen",					IAJuliaCmd);		 
	Commands.PostPricePCPoint			= GetInt(0, PCPoint.Config.MaximumPCPoints,	0,		"Post",			"PostPricePcPoint",				IAJuliaCmd);	 
	Commands.PostPriceWCoin				= GetInt(0, PCPoint.Config.MaximumWCPoints,	0,		"Post",			"PostPriceWCoin",				IAJuliaCmd);												
	Commands.PostColor					= GetInt(1, 3,									1,		"Post",			"PostColor",					IAJuliaCmd);  
	Commands.PostDelay					= GetInt(0,	32767,								60,		"Post",			"PostDelay",					IAJuliaCmd);

	//		add			//
	Commands.AddPointEnabled			= GetInt(0, 1,									1,		"AddCommand",	"AddPointEnabled",				IAJuliaCmd);
	Commands.AddPointLevelReq			= GetInt(0, Commands.MaxLvl,					0,		"AddCommand",	"AddPointLevelReq",				IAJuliaCmd);			   	
	Commands.AddPriceZen				= GetInt(0, 2000000000,							10000,	"AddCommand",	"AddPriceZen",					IAJuliaCmd);		 
	Commands.AddPricePCPoint			= GetInt(0, PCPoint.Config.MaximumPCPoints,	0,		"AddCommand",	"AddPricePCPoint",				IAJuliaCmd);	 
	Commands.AddPriceWCoin				= GetInt(0, PCPoint.Config.MaximumWCPoints,	0,		"AddCommand",	"AddPriceWCoin",				IAJuliaCmd);	 
	Commands.MaxAddedStats				= GetInt(0, 65000,								0,		"AddCommand",	"MaxAddedStats",				IAJuliaCmd);

	//		ware		//
	Commands.IsMultyVault				= GetInt(0,	1,									1,		"MultyVault",	"IsMultyVault",					IAJuliaCmd);  
	Commands.NumberOfVaults				= GetInt(2,	99,									3,		"MultyVault",	"NumberOfVaults",				IAJuliaCmd); 
	Commands.ZenForChange				= GetInt(0,	2000000000,							10000,	"MultyVault",	"ZenForChange",					IAJuliaCmd);
	Commands.PcPointForChange			= GetInt(0, PCPoint.Config.MaximumPCPoints,	0,		"MultyVault",	"PcPointForChange",				IAJuliaCmd);
	Commands.WCoinForChange				= GetInt(0,	PCPoint.Config.MaximumWCPoints,	0,		"MultyVault",	"WCoinForChange",				IAJuliaCmd);
}

void cConfigs::Misc()
{
	GetPrivateProfileString("Reset","ResetColumn","Resets", Configs.ResetColumn, sizeof(Configs.ResetColumn), IAJuliaCommon); 
	MuOnlineQuery.CheckColumn(Configs.ResetColumn, "Character", "ALTER TABLE Character ADD [%s][int] DEFAULT (0) NOT NULL", Configs.ResetColumn);
	GetPrivateProfileString("GameServerInfo","ServerName","GS", Configs.ServerName, sizeof(Configs.ServerName), ".\\Data\\ServerInfo.dat"); 
	IsPartyGap				= GetInt(0, 1, 1,"PartyGap", "IsPartyGap", IAJuliaCommon);	
	PartyGapLvl				= GetInt(0, 400, 120,"PartyGap", "PartyGapLvl", IAJuliaCommon);
	GuildRes				= GetInt(0, 32767, 5,"GuildMaster", "GuildCreateReset", IAJuliaCommon);
	Unicode32				= GetInt(0, 1, 0,"CharacterGuild", "AllowUnicode32", IAJuliaCommon);
	LahapDupe				= GetInt(0, 1, 1,"Lahap", "DupeBugFix", IAJuliaCommon);
	GuildLevel				= GetInt(0, 400, 320,"GameServerInfo", "GuildCreateLevel", "..\\Data\\CommonServer.cfg"); 
}

long cConfigs::GetInt(long Min, long Max, long Default, LPCSTR BlockName, LPCSTR ConfigName, LPCSTR FolderName)
{
	long lResult;
	char Message[250];
	char Message2[50];

	WIN32_FIND_DATAA wfd;
	if (FindFirstFileA(FolderName, &wfd) == INVALID_HANDLE_VALUE)
	{				
		wsprintf(Message,"Can't find file!\nTaked default value for %s(%ld)", ConfigName, Default);
		wsprintf(Message2,"Error configs in %s!", FolderName); 
			Log.ConsoleOutPut(0, c_Red, t_Error, "%s, \n %s \n \n", Message, Message2);
		lResult = Default;
	}
	else
		lResult = GetPrivateProfileInt(BlockName, ConfigName, -100500, FolderName);

	if(lResult == -100500)
	{	   
		wsprintf(Message,"Can't find '%s' in [%s] section!\nTaked default value for %s(%ld)", ConfigName, BlockName, ConfigName, Default);
		wsprintf(Message2,"Error configs in %s!", FolderName); 
			Log.ConsoleOutPut(0, c_Red, t_Error, "%s, \n %s", Message, Message2);
		lResult = Default;
	}

	if(lResult < Min || lResult > Max)
	{
		wsprintf(Message,"%s(%d) in [%s] is out of range!\nMin = %ld\t\tMax = %ld\nTaked default value(%ld)", ConfigName, lResult, BlockName, Min, Max, Default);
		wsprintf(Message2,"Error configs in %s!", FolderName); 
			Log.ConsoleOutPut(0, c_Red, t_Error, "%s, \n %s \n \n", Message, Message2);
		lResult = Default;
	} 
	return lResult;
}

char cConfigs::GetChar(long Min, long Max, unsigned char Default, LPCSTR BlockName, LPCSTR ConfigName, LPCSTR FolderName)
{
	unsigned char cResult;
	char Message[250];
	char Message2[50];

	WIN32_FIND_DATAA wfd;
	if (FindFirstFileA(FolderName, &wfd) == INVALID_HANDLE_VALUE)
	{				
		wsprintf(Message,"Can't find file!\nTaked default value for %s(%ld)", ConfigName, Default);
		wsprintf(Message2,"Error configs in %s!", FolderName); 
			Log.ConsoleOutPut(0, c_Red, t_Error, "%s, \n %s \n \n", Message, Message2);
		cResult = Default;
	}
	else
		cResult = GetPrivateProfileInt(BlockName, ConfigName, -100500, FolderName);

	if(cResult == -100500)
	{	   
		wsprintf(Message,"Can't find '%s' in [%s] section!\nTaked default value for %s(%ld)", ConfigName, BlockName, ConfigName, Default);
		wsprintf(Message2,"Error configs in %s!", FolderName); 
		Log.ConsoleOutPut(0, c_Red, t_Error, "%s, \n %s \n \n", Message, Message2);
		cResult = Default;
	}

	if(cResult < Min || cResult > Max)
	{
		wsprintf(Message,"%s(%d) in [%s] is out of range!\nMin = %ld\t\tMax = %ld\nTaked default value(%ld)", ConfigName, cResult, BlockName, Min, Max, Default);
		wsprintf(Message2,"Error configs in %s!", FolderName); 
		Log.ConsoleOutPut(0, c_Red, t_Error, "%s, \n %s \n \n", Message, Message2);
		cResult = Default;
	}
	return cResult;
}