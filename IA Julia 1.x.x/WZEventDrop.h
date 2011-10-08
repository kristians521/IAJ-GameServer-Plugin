// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#ifdef _GS
void fHandlerBC(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level, BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption);
void fHandlerIT(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level, BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption);
void fHandlerWW(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level, BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption);
void fHandlerCC(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level, BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption);

struct s_DropEventItem
{
	int dropRate;
	int iGroup;
	int iId;
	int minLvl;
	int maxLvl;
	int dur;
	int skill;
	int luck;
	int minOpt;
	int maxOpt;
	int exc;
	int minExc;
	int maxExc;
	int anc;
	int ancType;
};

struct s_DropEventItemCount
{
	int countRate;
	int countItems;
};

class cDropEvent
{
private:
	std::map<int, int> mapsBC;
	std::map<int, std::vector<s_DropEventItem> > dropListBC;

	std::map<int, int> mapsIT;
	std::map<int, std::vector<s_DropEventItem> > dropListIT;

	std::vector<s_DropEventItem> dropListWW;

	std::map<int, int> mapsCC;
	std::map<int, std::vector<s_DropEventItem> > dropListCC;

public:
	s_DropEventItemCount dropCountBC;
	s_DropEventItemCount dropCountIT;
	s_DropEventItemCount dropCountWW;
	s_DropEventItemCount dropCountCC;

	void asmHook();
	void Init();
	void dropItem(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int LootIndex, s_DropEventItem&i);

	void updateBC();
	void updateIT();
	void updateWW();
	void updateCC();

	void handlerBC(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level, BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption);
	void handlerIT(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level, BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption);
	void handlerWW(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level, BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption);
	void handlerCC(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level, BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption);
};

extern cDropEvent DropEvent;
#endif