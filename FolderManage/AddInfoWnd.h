#pragma once
#include "main.h"
#include "MenuWnd.h"
#include "SQLiteHelper.h"
#include "sqlExecute.h"

class CAddInfoWnd :
	public CXmlWnd
{
public:
	explicit CAddInfoWnd(LPCTSTR pszXMLPath);
	virtual void InitWindow();
	virtual void Notify(TNotifyUI& msg);
	virtual CControlUI* CreateControl(LPCTSTR pstrClassName);
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnClose(UINT uMsg, WPARAM& wParam, LPARAM& lParam, BOOL& bHandled);
	LRESULT XmlOnClose(UINT uMsg, WPARAM& wParam, LPARAM& lParam, BOOL& bHandled);	
	LRESULT OnDestroy(UINT uMsg, WPARAM& wParam, LPARAM& lParam, BOOL& bHandled);
	bool m_bCloseFlag;
	LRESULT OnTrayIcon(UINT uMsg, WPARAM& wParam, LPARAM& lParam, BOOL& bHandled);

public:
	void getInfo();

public:
	string m_strLoad;
	CSqlExecute m_sqlSexecute;
	string m_strOwnDir;
	CListUI* m_pList;
	CTrayIcon m_trayIcon;
};

