#pragma once
#include <UIlib.h>
using namespace DuiLib;

#ifdef _DEBUG
#       pragma comment(lib, "DuiLib_d.lib")
#else 
#       pragma comment(lib, "DuiLib.lib")
#endif

#define XML_WN_COLSE WM_USER+1006

typedef struct DbData
{
	int	m_iType;
	string	m_strName;
	int		m_intValue;
	string	m_strValue;
	DbData(int iType, string strName, int intValue, string strValue):m_iType(iType), m_strName(strName), m_intValue(intValue), m_strValue(strValue){
	}
}sData, *pDbData;

// 以XML生成界面的窗口基类
class CXmlWnd : public WindowImplBase
{
public:
	explicit CXmlWnd(LPCTSTR pszXMLPath) 
		: m_strXMLPath(pszXMLPath){}

public:
	virtual LPCTSTR GetWindowClassName() const
	{
		return _T("CXmlWnd");
	}

	virtual CDuiString GetSkinFile()
	{
		return m_strXMLPath;
	}

	virtual CDuiString GetSkinFolder()
	{
		return _T("");
	}

protected:
	CDuiString m_strXMLPath;
};