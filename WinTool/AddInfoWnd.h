#pragma once
#include "main.h"
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
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//virtual LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM& wParam, LPARAM& lParam, BOOL& bHandled);
	LRESULT XmlOnClose(UINT uMsg, WPARAM& wParam, LPARAM& lParam, BOOL& bHandled);	
	LRESULT OnDestroy(UINT uMsg, WPARAM& wParam, LPARAM& lParam, BOOL& bHandled);
	//void CheckUploadInput();
	void RemoveEnterChar(string& strIn, string& strOut);
	bool m_bCloseFlag;
public:
	void WriteInfo();
	bool isDir(const char* path);
public:
	string m_strLoad;
	string m_strOwnDir;
	CSqlExecute m_sqlSexecute;
	CRichEditUI* m_pInfoKey;
};

