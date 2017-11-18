#pragma once
#include "main.h"
class CXmlMessageBox :
	public CXmlWnd
{
public:
	CXmlMessageBox(LPCTSTR pszXMLPath);

protected:
	virtual ~CXmlMessageBox(void);

public:
	void Init( HWND hWndParent, LPCTSTR strTitleName, LPCTSTR strBodyContent, bool bClose);
	virtual void OnFinalMessage(HWND hWnd);
	virtual void Notify(TNotifyUI& msg);
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/);
	//virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	//LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	HWND m_hWndParent;

private:
	bool m_bClose;
};

