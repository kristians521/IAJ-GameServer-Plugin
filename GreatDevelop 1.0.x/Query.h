// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # GreatDevelop 1.1.0 beta						# //
// ================================================== //

#ifndef Query_H
#define Query_H

#include "StdAfx.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define MAX_COLUMNS	100

void LoadQuery();

class CQuery  
{
public:

	CQuery();
	virtual ~CQuery();

	BOOL Connect(LPTSTR lpszDNS, LPTSTR lpszUser, LPTSTR lpszPassword);
	BOOL ExecQuery(LPTSTR lpszStatement, ...);
	void Close();
	void GetAsString(LPTSTR ColName, LPTSTR pOutBuffer);
	DWORD GetAsInteger(LPTSTR ColName);
	float GetAsFloat(LPTSTR ColName);
	void Disconnect();
	int GetAsBinary(LPSTR lpszStatement, LPBYTE OUT lpszReturnBuffer);
	SQLRETURN Fetch();
	int GetResult(int iIndex);
	void SetAsBinary(LPTSTR lpszStatement, LPBYTE lpBinaryBuffer, SQLUINTEGER BinaryBufferSize);
	void CheckColumns(int MeMu);
	void CheckColumn(char* Column, char* Table, char* AddQuery, ...);

private:

	BOOL ReConnect();

	int FindIndex(LPTSTR ColName);
	void Diagnosis();

protected:

	SQLHANDLE m_hEnviroment;
	SQLHANDLE m_hConnection;
	SQLHANDLE m_hStmt;
	TCHAR m_szDNS[64];
	TCHAR m_szUser[64];
	TCHAR m_szPassword[64];
	SQLSMALLINT m_ColCount;
	SQLTCHAR m_SQLColName[MAX_COLUMNS][30];
	TCHAR m_SQLData[MAX_COLUMNS][256];
	SQLINTEGER m_SQLDataLen[MAX_COLUMNS];

};

extern CQuery MuOnlineQuery;
extern CQuery Me_MuOnlineQuery;

#endif
