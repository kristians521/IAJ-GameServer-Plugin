// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # GreatDevelop 1.0.x								# //
// ================================================== //

#ifndef Logger_H
#define Logger_H

extern char Message[1024];

enum sColor
{
	c_Red,		// ��������
	c_Green,	// ������
	c_Blue,		// �������
	c_Cyan,		// ���������
	c_Yellow,	// Ƹ����
	c_Magenta,	// ����������
	c_Grey		// ����� 
};

enum sLogType
{
	t_NULL,
	t_Error,
	t_Default,
	t_GM,
	t_POST,
	t_DROP,
	t_BAN,
	t_COMMANDS,
	t_GPOST,
	t_PCPOINT,
	t_IPBLOCK,
	t_Duel,
	t_SQL,
	t_VIP
};

class Logger
{
public:	   
									
	Logger();
	~Logger();		 
	void LoggerInit();	
	void CheckProcent(char* message); 	
	void SaveFile(char *logString,char *Message);
	void ConsoleOutPut(int WOL, sColor Color, sLogType Type, const char* Format, ...);
	HANDLE Handle(BOOL Input);
	void CreateLog(sLogType Type,const char* Format, ...);	
	char *LoggerTittle();
	
	int Online_Max;
	int Online_All;
	int Online_Gms;
};
extern Logger Log;		

#endif Logger_H