// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # GreatDevelop 1.0.1								# //
// ================================================== //

#include "User.h"
#include "Prodef.h"
#ifndef PROTOC_H
#define PROTOC_H	   								

int gObjAttack(LPOBJ lpObj, LPOBJ lpTargetObj, class CMagicInf* lpMagic, int magicsend, unsigned char MSBFlag, int AttackDamage, BOOL bCombo);	 
void gObjPlayerKiller(LPOBJ lpObj, LPOBJ lpTargetObj); 
bool ProtocolCore (BYTE protoNum, LPBYTE aRecv, DWORD aLen, int aIndex, DWORD Encrypt, int Serial);
void ProtocolCoreSend(int aIndex,unsigned char* aSend, int aLen);

class cProtoFunc
{
public:
	bool NPCTalkEx(LPOBJ gObj, int NpcId);
	void PlayerConnect(LPOBJ gObj); 
	void CheckRingSend(LPOBJ gObj, LPBYTE aRecv);	
	void CheckRing(LPOBJ gObj, LPBYTE aRecv);
	bool CharacterCreate(PMSG_CHARCREATE* lpMsg, int aIndex);
	bool GuildMasterInfoSave(int aIndex,PMSG_GUILDINFOSAVE* lpMsg);
	bool CGPartyRequestRecv(PMSG_PARTYREQUEST * lpMsg, int aIndex);
private:			
	void LoginMsg(LPOBJ gObj);
	void RingSkin(LPOBJ gObj);
	void PkClear(LPOBJ gObj, LPOBJ NpcObj);
};
extern cProtoFunc Protocol;
#endif