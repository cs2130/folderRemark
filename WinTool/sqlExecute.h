#ifndef _SQLEXECUTE_H
#define _SQLEXECUTE_H

#pragma once
#include "stdafx.h"
#include "main.h"
#include "SQLiteHelper.h"
//typedef std::map <std::string, std::string> map_t;

class T
{
public:
	T(void)
	{

	}

	~T(void)
	{

	}

private:
	std::map < std::string, DbData > map_d;
	typedef std::map <std::string, DbData> map_t;

public:
	void insertMap(int iType, string strName, int intValue, string strValue)
	{
		map_d.insert( std::pair< std::string, DbData >(strName, DbData(iType, strName, intValue, strValue)) );
	}

	void* getValue(string strName)
	{
		std::map < std::string, DbData >::iterator iter;  
		void* rvoid = NULL;
		iter = map_d.find(strName);
		if(iter!=map_d.end())
		{
			switch(iter->second.m_iType)
			{
			case 1: rvoid = &iter->second.m_intValue; break;
			case 3: rvoid = &iter->second.m_strValue; break;
			default: break;
			}
		}
		return rvoid;
	}

	int getIntValue(string strName)
	{
		std::map < std::string, DbData >::iterator iter;  
		int ir = 0;
		iter = map_d.find(strName);
		if(iter!=map_d.end())
			ir = iter->second.m_intValue;

		return ir;
	}

	string getStringValue(string strName)
	{
		std::map < std::string, DbData >::iterator iter;  
		string strr = "";
		iter = map_d.find(strName);
		if(iter!=map_d.end())
			strr = iter->second.m_strValue;

		return strr;
	}

	map_t getMapValue()
	{
		return map_d;
	}
};

class CSqlExecute
{
public:
	CSqlExecute();
	~CSqlExecute();

public:
	TCHAR m_szDbPath[250];
	

public:
	bool SqlCreate(const TCHAR *csql);
	bool SqlInsertAddPrject(const TCHAR *csql);
	bool SqlUpdateAddPrject(const TCHAR *csql);
	std::list<T> SqlSelectAddPrject(const TCHAR *csql);
	bool SqlInsertAddPrject(const TCHAR *csql, int &iRowId);
	bool SqlDeleteAddPrject(const TCHAR *csql);
	void SqlSetPath(const TCHAR *csql);
};

#endif /* des.h */