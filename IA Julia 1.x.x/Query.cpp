// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#include "stdafx.h"
#include "Query.h"
#include "Logger.h"
#include "Configs.h"

CQuery MuOnlineQuery;
CQuery Me_MuOnlineQuery;

CQuery::CQuery()
{
	this->m_ColCount = -1;
	this->m_hConnection = NULL;
	this->m_hEnviroment = NULL;
	this->m_hStmt = NULL;
	memset(this->m_SQLColName, 0, sizeof(this->m_SQLColName));
	memset(this->m_SQLData, 0, sizeof(this->m_SQLData));
	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &this->m_hEnviroment);
	SQLSetEnvAttr(this->m_hEnviroment,SQL_ATTR_ODBC_VERSION,(SQLPOINTER)SQL_OV_ODBC3, SQL_NTS);

}

CQuery::~CQuery()
{
	if ( this->m_hStmt != SQL_NULL_HANDLE )
		SQLFreeHandle(SQL_HANDLE_STMT, this->m_hStmt);

	if ( this->m_hConnection != SQL_NULL_HANDLE )
		SQLFreeHandle(SQL_HANDLE_DBC, this->m_hConnection);

	if ( this->m_hEnviroment != SQL_NULL_HANDLE )
		SQLFreeHandle(SQL_HANDLE_ENV, this->m_hEnviroment);
}


void CQuery::CheckColumns(int MeMu)
{								   	
	struct Temp
	{  
		char AddQuery[255];
		char Column[35];	  
		char Table[35];
	};									  

	Temp QueryMuOnline[] = {	
		{"ALTER TABLE Character ADD [DuelWins] [int] DEFAULT (0) NOT NULL"
		,"DuelWins","Character"},	 

		{"ALTER TABLE Character ADD [DuelLoses] [int] DEFAULT (0) NOT NULL"
		,"DuelLoses","Character"},

		{"ALTER TABLE Character ADD [DuelsTotal] [int] DEFAULT (0) NOT NULL"
		,"DuelsTotal","Character"},

		{"ALTER TABLE Character ADD [LastDuel] [datetime] NULL"
		,"LastDuel","Character"},

		{"ALTER TABLE Character ADD [BanPost] [tinyint] DEFAULT (0) NOT NULL"
		,"BanPost","Character"}, 													   

		{"ALTER TABLE warehouse ADD [UsedSlot] [int] NULL"
		,"UsedSlot","warehouse"},		   

		{"ALTER TABLE warehouse ADD [Items01][varbinary](1920) NULL"
		,"Items01","warehouse"},		

		{"ALTER TABLE warehouse ADD [Items02][varbinary](1920) NULL"
		,"Items02","warehouse"},		

		{"ALTER TABLE warehouse ADD [Items03][varbinary](1920) NULL"
		,"Items03","warehouse"},		

		{"ALTER TABLE warehouse ADD [Items04][varbinary](1920) NULL"
		,"Items04","warehouse"},		

		{"ALTER TABLE warehouse ADD [Items05][varbinary](1920) NULL"
		,"Items05","warehouse"},															   																   
	};

	Temp QueryMeMuOnline[] = {												   																   

		{"ALTER TABLE MEMB_STAT ADD [OnlineHours][int] DEFAULT (0) NOT NULL"
			,"OnlineHours","MEMB_STAT"}, 

		{"ALTER TABLE MEMB_INFO ADD [cspoints][int] DEFAULT (0) NOT NULL"
			,"cspoints","MEMB_INFO"}, 
	};
	if(MeMu)
		for(int i = 0; i < (sizeof QueryMeMuOnline / sizeof QueryMeMuOnline[0]); i++)
			CheckColumn(QueryMeMuOnline[i].Column, QueryMeMuOnline[i].Table, QueryMeMuOnline[i].AddQuery);
	else
		for(int i = 0; i < (sizeof QueryMuOnline / sizeof QueryMuOnline[0]); i++)
			CheckColumn(QueryMuOnline[i].Column, QueryMuOnline[i].Table, QueryMuOnline[i].AddQuery);
}

void CQuery::CheckColumn(char* Column, char* Table, char* AddQuery, ...)
{		
	char Temp[255];	 
	Temp[0] = 0;
	va_list pArguments;
	va_start(pArguments, AddQuery);
	vsprintf_s(Temp, AddQuery, pArguments);
	va_end(pArguments);

	char Result[255];
	Result[0] = 0;
	ExecQuery("SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.Columns where COLUMN_NAME = '%s' and TABLE_NAME = '%s'", Column, Table);
		Fetch();
		lstrcpy(Result, this->m_SQLData[0]);
		Close();

	if(strcmp(Result, Column))
	{ 
		ExecQuery(Temp);
			Fetch();
			Close();
		Log.ConsoleOutPut(0, c_Red, t_Error, "IAJulia SQL Error!! \n Can't find %s column in %s table.\nSucsessfully added %s column in %s table \n \n", Column, Table, Column, Table);
	}
}

void LoadQuery()
{
	char szUser[50];
	char szPassword[50];
	char szDatabase[50];
	char szDatabase2[50];

	GetPrivateProfileStringA("SQL","SQLLogin","sa",szUser,sizeof(szUser),IAJuliaGS);
	GetPrivateProfileStringA("SQL","SQLPass","PASS",szPassword,sizeof(szPassword),IAJuliaGS);
	GetPrivateProfileStringA("SQL","SQLDB","MuOnline",szDatabase,sizeof(szDatabase),IAJuliaGS);
	GetPrivateProfileStringA("SQL","SQLDB2","Me_MuOnline",szDatabase2,sizeof(szDatabase2),IAJuliaGS);

	if ( MuOnlineQuery.Connect(szDatabase, szUser, szPassword) == TRUE )
	{
		Log.ConsoleOutPut(0, c_Yellow, t_SQL, "[MuOnlineQuery] Connection Successfull!");
	}
	else
	{
		Log.ConsoleOutPut(0, c_Yellow, t_SQL, "[MuOnlineQuery] Error On Connection!");
	}

	if ( Me_MuOnlineQuery.Connect(szDatabase2, szUser, szPassword) == TRUE )
	{
		Log.ConsoleOutPut(0, c_Yellow, t_SQL, "[Me_MuOnlineQuery] Connection Successfull!");
	}
	else
	{
		Log.ConsoleOutPut(0, c_Yellow, t_SQL, "[Me_MuOnlineQuery] Error On Connection!");
	}
	MuOnlineQuery.CheckColumns(0);
	Me_MuOnlineQuery.CheckColumns(1);
}

BOOL CQuery::Connect(TCHAR * lpszDNS, TCHAR * lpszUser, TCHAR * lpszPassword)
{
	lstrcpy(this->m_szDNS, lpszDNS);
	lstrcpy(this->m_szUser, lpszUser);
	lstrcpy(this->m_szPassword, lpszPassword);
	SQLHANDLE rgbValue;

	SQLAllocHandle(SQL_HANDLE_DBC, this->m_hEnviroment, &this->m_hConnection);
	SQLSetConnectAttr(this->m_hConnection, SQL_LOGIN_TIMEOUT, &rgbValue, 0);

	SQLRETURN Result = SQLConnect(this->m_hConnection, (SQLTCHAR *)this->m_szDNS, SQL_NTS,
		(SQLTCHAR *)this->m_szUser, SQL_NTS, (SQLTCHAR *)this->m_szPassword, SQL_NTS);


	if ( Result != SQL_SUCCESS && Result != SQL_SUCCESS_WITH_INFO )
	{
		SQLSMALLINT sRecord = 1, MsgLen;
		SQLTCHAR SqlState[6], SQLMsgError[SQL_MAX_MESSAGE_LENGTH];
		SQLINTEGER NativeError;

		while ( SQLGetDiagRec(SQL_HANDLE_DBC, this->m_hConnection, sRecord,
			SqlState, &NativeError, SQLMsgError, sizeof(SQLMsgError), &MsgLen) != SQL_NO_DATA )
		{
			Log.ConsoleOutPut(1, c_Red, t_SQL, "SQLSTATE:%s, Diagnosis:%s", SqlState, SQLMsgError);
		}

		return FALSE;
	}

	Result = SQLAllocHandle(SQL_HANDLE_STMT, this->m_hConnection, &this->m_hStmt);

	if ( Result != SQL_SUCCESS && Result != SQL_SUCCESS_WITH_INFO )
		return FALSE;

	return TRUE;
}


BOOL CQuery::ReConnect()
{
	return this->Connect(this->m_szDNS, this->m_szUser, this->m_szPassword);
}



BOOL CQuery::ExecQuery(TCHAR * lpszStatement, ...)
{
	TCHAR szStatement[1024];

	va_list pArguments;
	va_start(pArguments, lpszStatement);
	//wvsprintf(szStatement, lpszStatement, pArguments);
	vsprintf(szStatement, lpszStatement, pArguments);
	va_end(pArguments);
	
	SQLRETURN Result = SQLExecDirect(this->m_hStmt, (SQLTCHAR *)szStatement, SQL_NTS);

	if ( Result != SQL_SUCCESS && Result != SQL_SUCCESS_WITH_INFO && Result != SQL_NO_DATA )
	{
		this->Diagnosis();
		return FALSE;
	}

	SQLINTEGER RowCount;
	SQLRowCount(this->m_hStmt, &RowCount);
	SQLNumResultCols(this->m_hStmt, &this->m_ColCount);

	if ( this->m_ColCount >= MAX_COLUMNS-1 )
	{
		Log.ConsoleOutPut(1, c_Red, t_SQL, "[CQuery] ColCount >= MAX_COLUMNS-1");
		return FALSE;
	}

	// Case just EXEC
	if ( this->m_ColCount == 0 )
	{
		Log.ConsoleOutPut(1, c_Green, t_SQL, "[Free Query] %s", szStatement);
		this->Close();
		return TRUE;
	}

	for(int iColPos=0; iColPos < this->m_ColCount ; iColPos++  )
	{
		SQLBindCol(this->m_hStmt, iColPos+1, SQL_CHAR, this->m_SQLData[iColPos],
			sizeof(this->m_SQLData[0])-1, &this->m_SQLDataLen[iColPos]);
		SQLDescribeCol( this->m_hStmt, iColPos+1, this->m_SQLColName[iColPos],
			sizeof(this->m_SQLColName[iColPos]), NULL, NULL, NULL, NULL, NULL);
	}
	Log.ConsoleOutPut(1, c_Blue, t_SQL, "[SUCCESS] %s", szStatement);

	return TRUE;
}


void CQuery::Close()
{
	SQLCloseCursor(this->m_hStmt);
	SQLFreeStmt(this->m_hStmt, 2);
}



SQLRETURN CQuery::Fetch()
{
	return SQLFetch(this->m_hStmt);
}


int CQuery::FindIndex(LPTSTR ColName)
{
	for(short i=0;i<this->m_ColCount;i++)
	{
		if ( this->m_SQLColName[i][0] == ColName[0] )
		{
			if ( lstrcmp((TCHAR *)this->m_SQLColName[i], (TCHAR *)ColName) == 0 )
			{
				return i;
			}
		}
	}

	return -1;
}


void CQuery::GetAsString(LPTSTR ColName, LPTSTR pOutBuffer)
{
	int iIndex = this->FindIndex(ColName);

	if ( iIndex != -1 )
	{
		lstrcpy(pOutBuffer, this->m_SQLData[iIndex]);
		Log.ConsoleOutPut(1, c_Red, t_SQL, "[Query][String] %s = %s", ColName, pOutBuffer);
	}
	else
	{
		pOutBuffer[0] = 0;
		Log.ConsoleOutPut(1, c_Red, t_SQL, "[Query][String] %s = ERROR", ColName);
	}
}


DWORD CQuery::GetAsInteger(LPTSTR ColName)
{
	int iIndex =this->FindIndex(ColName);

	if ( iIndex != -1 )
	{
		Log.ConsoleOutPut(1, c_Red, t_SQL, "[Query][Int] %s = %d", ColName, atoi(this->m_SQLData[iIndex]));
		return atoi(this->m_SQLData[iIndex]);
	}

	Log.ConsoleOutPut(1, c_Red, t_SQL, "[Query][Int] %s = ERROR", ColName);
	return -1;
}



float CQuery::GetAsFloat(LPTSTR ColName)
{
	int iIndex =this->FindIndex(ColName);

	if ( iIndex != -1 )
	{
		Log.ConsoleOutPut(1, c_Red, t_SQL, "[Query][Float] %s = %f", ColName, atof(this->m_SQLData[iIndex]));
		return (float)atof(this->m_SQLData[iIndex]);
	}

	Log.ConsoleOutPut(1, c_Red, t_SQL, "[Query][Float] %s = ERROR", ColName);
	return -1;
}



void CQuery::Diagnosis()
{
	SQLSMALLINT sRecord = 1, MsgLen;
	SQLTCHAR SqlState[6], SQLMsgError[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER NativeError;

	while ( SQLGetDiagRec(SQL_HANDLE_STMT, this->m_hStmt, sRecord,
		SqlState, &NativeError, SQLMsgError, sizeof(SQLMsgError), &MsgLen) != SQL_NO_DATA )
	{
		Log.ConsoleOutPut(1, c_Red, t_SQL, "SQLSTATE:%s, Diagnosis:%s", SqlState, SQLMsgError);
		sRecord++;
	}

	if ( !lstrcmp((TCHAR *)SqlState, "08S01"))	// Communication Link Failure
		this->ReConnect();
}


int CQuery::GetResult(int iIndex)
{
	return atoi(this->m_SQLData[iIndex]);
}

int CQuery::GetAsBinary(LPTSTR lpszStatement, LPBYTE OUT lpszReturnBuffer)
{
	SQLCHAR * pSQLBuf;
	SQLINTEGER BufLen;
	SQLCHAR SQLBinary[10000];
	SQLINTEGER lOfs = 0;
	SQLINTEGER SQLLen;
	SQLRETURN SQLResult;

	//SQLAllocHandle(SQL_HANDLE_STMT, this->m_hConnection, &this->m_hStmt);

	SQLResult = SQLExecDirect(this->m_hStmt, (SQLTCHAR *)lpszStatement, SQL_NTS);

	if ( SQLResult != SQL_SUCCESS )
	{
		this->Diagnosis();
		return -1;
	}

	while ( true )
	{
		SQLResult = SQLFetch(this->m_hStmt);

		if ( SQLResult == SQL_NO_DATA )
			break;

		pSQLBuf = lpszReturnBuffer;

		while ( true )
		{

			SQLResult = SQLGetData(this->m_hStmt,
				1, SQL_C_BINARY , SQLBinary, sizeof(SQLBinary), &SQLLen);

			if ( SQLResult == SQL_NO_DATA )
				break;

			if ( SQLLen == -1 )
			{
				this->Close();
				return 0;
			}

			if ( SQLResult == SQL_SUCCESS )
			{
				BufLen = SQLLen;
			}
			else
			{
				BufLen = sizeof(SQLBinary);
			}

			lOfs += BufLen;
			memcpy(pSQLBuf, SQLBinary, BufLen);
			pSQLBuf = &pSQLBuf[lOfs];
		}
	}

	//SQLFreeHandle(SQL_HANDLE_STMT, this->m_hStmt);
	return BufLen;
}


void CQuery::SetAsBinary(LPTSTR lpszStatement, LPBYTE lpBinaryBuffer, SQLUINTEGER BinaryBufferSize)
{
	SQLINTEGER cbValueSize = -0x64 - BinaryBufferSize;
	SQLPOINTER pToken;
	BYTE cBUFFER[10000];
	SQLRETURN Result;

	SQLBindParameter(this->m_hStmt, 1, SQL_PARAM_INPUT,
		SQL_C_BINARY , SQL_LONGVARBINARY, BinaryBufferSize, 0, (SQLPOINTER)1, 0, &cbValueSize);

	SQLExecDirect(this->m_hStmt, (SQLTCHAR *)lpszStatement, SQL_NTS);
	Result = SQLParamData(this->m_hStmt, &pToken);

	int lOfs=0;

	while ( Result == SQL_NEED_DATA )
	{
		memcpy(cBUFFER, lpBinaryBuffer, sizeof(cBUFFER));
		Result = SQLPutData(this->m_hStmt, cBUFFER, BinaryBufferSize);
		Result = SQLParamData(this->m_hStmt, &pToken);
		lOfs += sizeof(cBUFFER);
	}

	SQLParamData(this->m_hStmt, &pToken);
	this->Close();
}

void CQuery::Disconnect()
{
	if ( this->m_hStmt )
		SQLFreeHandle(SQL_HANDLE_STMT, this->m_hStmt);

	if ( this->m_hConnection )
		SQLDisconnect(this->m_hConnection);

	if ( this->m_hConnection )
		SQLFreeHandle(SQL_HANDLE_DBC, this->m_hConnection);

	this->m_hStmt = NULL;
	this->m_hConnection = NULL;
}