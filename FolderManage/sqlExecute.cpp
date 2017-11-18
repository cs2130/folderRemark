#include "sqlExecute.h"

CSqlExecute::CSqlExecute()
{
	//_stprintf(m_szDbPath, _T("%s"), "folder.db");
}

CSqlExecute::~CSqlExecute()
{

}

void CSqlExecute::SqlSetPath(const TCHAR *csql){
	_stprintf(m_szDbPath, _T("%s\\%s"), csql, "folder.db");
	int nlen=0, codepage;
	char cpath[130];
	wchar_t wpath[130];
	codepage = AreFileApisANSI() ? CP_ACP : CP_OEMCP;
	nlen = MultiByteToWideChar(codepage, 0, m_szDbPath, -1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, m_szDbPath, -1, wpath, nlen); 
	nlen=WideCharToMultiByte(CP_UTF8 , 0, wpath, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_UTF8 , 0, wpath, -1, cpath, nlen, 0, 0);
	//::MessageBox(NULL, cpath, _T("提示(by tojen)"), MB_OK);
	_stprintf(m_szDbPath, _T("%s"), cpath);
}

bool CSqlExecute::SqlCreate(const TCHAR *csql)
{
	TCHAR sql[1024] = {0};
	bool breturn = false;
	SQLiteHelper sqlite;
	// 打开或创建数据库
	//******************************************************
	if(sqlite.Open(m_szDbPath))
	{
		if (!sqlite.BeginTransaction())
			breturn = false;
		breturn = true;
		_stprintf(sql,_T("%s"),
			_T("CREATE TABLE [folder] (")
			_T("[fr_id] INTEGER NOT NULL PRIMARY KEY, ")
			_T("[fr_name] NVARCHAR(200), ")
			_T("[fr_folder] NVARCHAR(200), ")
			_T("[fr_info] NVARCHAR(1024), ")
			_T("[fr_time] NVARCHAR(200), ")
			_T("[fr_bz] NVARCHAR(1000)); ")
			);

		if (!sqlite.ExcuteNonQuery(sql))
			breturn = false;

		if (!sqlite.CommitTransaction())
			breturn = false;
		sqlite.Close();
	}
	
	return breturn;
}

bool CSqlExecute::SqlInsertAddPrject(const TCHAR *csql)
{
	bool breturn = false;
	SQLiteHelper sqlite;
	if(sqlite.Open(m_szDbPath))
	{
		breturn = true;
		if (!sqlite.BeginTransaction())
			breturn = false;
		if (!sqlite.ExcuteNonQuery(csql))
			breturn = false;
		if (!sqlite.CommitTransaction())
			breturn = false;
		sqlite.Close();
	}
	return breturn;
}

bool CSqlExecute::SqlInsertAddPrject(const TCHAR *csql, int &iRowId)
{
	bool breturn = false;
	SQLiteHelper sqlite;
	if(sqlite.Open(m_szDbPath))
	{
		breturn = true;
		if (!sqlite.BeginTransaction())
			breturn = false;
		if (!sqlite.ExcuteNonQuery(csql, iRowId))
			breturn = false;
		if (!sqlite.CommitTransaction())
			breturn = false;
		sqlite.Close();
	}
	return breturn;
}

bool CSqlExecute::SqlUpdateAddPrject(const TCHAR *csql)
{
	bool breturn = false;
	SQLiteHelper sqlite;
	if(sqlite.Open(m_szDbPath))
	{
		breturn = true;
		if (!sqlite.BeginTransaction())
			breturn = false;
		if (!sqlite.ExcuteNonQuery(csql))
			breturn = false;
		if (!sqlite.CommitTransaction())
			breturn = false;
		sqlite.Close();
	}
	return breturn;
}

bool CSqlExecute::SqlDeleteAddPrject(const TCHAR *csql)
{
	bool breturn = false;
	SQLiteHelper sqlite;
	if(sqlite.Open(m_szDbPath))
	{
		breturn = true;
		if (!sqlite.BeginTransaction())
			breturn = false;
		if (!sqlite.ExcuteNonQuery(csql))
			breturn = false;
		if (!sqlite.CommitTransaction())
			breturn = false;
		sqlite.Close();
	}
	return breturn;
}

std::list<T> CSqlExecute::SqlSelectAddPrject(const TCHAR *csql)
{
	std::list<T> list_m;
	bool breturn = false;
	SQLiteHelper sqlite;
	if(sqlite.Open(m_szDbPath))
	{
		SQLiteDataReader Reader = sqlite.ExcuteQuery(csql);

		while(Reader.Read())
		{
			T t;
			int icol = Reader.ColumnCount();
			for (int i=0; i<icol; i++)
			{
				int ilx = Reader.GetDataType(i);
				switch(ilx)
				{
				case 2: t.insertMap( ilx, Reader.GetName(i), 0, Reader.GetStringValue(i)); break;
				case 3: t.insertMap( ilx, Reader.GetName(i), 0, Reader.GetStringValue(i) ); break;
				case 4: t.insertMap( ilx, Reader.GetName(i), 0, Reader.GetStringValue(i) ); break;
				default: break;
				}
			}
			list_m.push_back(t);
		}
		Reader.Close();
		sqlite.Close();
	}
	return list_m;
}