#include "stdafx.h"
#include "resource.h"
#include "AddInfoWnd.h"
#include "XmlMessageBox.h"

typedef void (WINAPI *PSWITCHTOTHISWINDOW)(HWND,BOOL);

CAddInfoWnd::CAddInfoWnd( LPCTSTR pszXMLPath )
	: CXmlWnd(pszXMLPath)

{
}

void CAddInfoWnd::InitWindow()
{
	SetIcon(IDI_ICON); // 设置任务栏图标
	CenterWindow();
	m_pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("listview")));
	m_bCloseFlag = true;

	// 注册托盘图标
	m_trayIcon.CreateTrayIcon(m_hWnd, IDI_ICON, _T("文件夹备注"));

	// TODO: Add extra initialization here
	TCHAR exeFullPath[MAX_PATH]; // MAX_PATH
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	m_strOwnDir = exeFullPath;
	int nPosDir = m_strOwnDir.rfind('\\');
	m_strOwnDir = m_strOwnDir.substr(0,nPosDir);
	m_sqlSexecute.SqlSetPath(m_strOwnDir.c_str());
	TCHAR sql[4096] = {""};
	m_sqlSexecute.SqlCreate(sql);
	getInfo();
}

void CAddInfoWnd::getInfo(){
	m_pList->RemoveAll();
	std::list<T> list_m_tdj = m_sqlSexecute.SqlSelectAddPrject("select * from folder order by fr_id desc");
	std::list<T>::iterator iter;
	int i = 0;
	for(iter=list_m_tdj.begin();iter!=list_m_tdj.end();++iter)
	{
		CListTextElementUI* pListElement = new CListTextElementUI;
		pListElement->SetTag(i);
		m_pList->Add(pListElement);
		pListElement->SetText(0, iter->getStringValue("fr_name").c_str());
		pListElement->SetText(1, iter->getStringValue("fr_folder").c_str());
		pListElement->SetText(2, iter->getStringValue("fr_info").c_str());
		i++;
	}
}

CControlUI* CAddInfoWnd::CreateControl( LPCTSTR pstrClassName )
{
	return NULL;
}

LRESULT CAddInfoWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/)
{
	if (uMsg == WM_KEYDOWN)
	{
		switch (wParam)
		{
		case VK_RETURN: 
			break;
		case VK_ESCAPE:
			break;
		case VK_SPACE:
			break;
		default:
			break;
		}
	}

	return FALSE;
}

LRESULT CAddInfoWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL    bHandled = TRUE;

	switch( uMsg )
	{
	case WM_CLOSE:	lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case XML_WN_COLSE: lRes = XmlOnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY: lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
	case UIMSG_TRAYICON: lRes = OnTrayIcon(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
		break;
	}

	if( bHandled ) return lRes;
	//if( m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;

	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CAddInfoWnd::OnTrayIcon(UINT uMsg, WPARAM& wParam, LPARAM& lParam, BOOL& bHandled){

	UINT uIconMsg = (UINT)lParam;
	if(uIconMsg == WM_LBUTTONUP) {
		//BOOL bVisible = IsWindowVisible(m_hWnd);
		//::ShowWindow(m_hWnd, !bVisible ?  SW_SHOW : SW_HIDE);
		PSWITCHTOTHISWINDOW SwitchToThisWindow;
		HMODULE hUser32=GetModuleHandle(_T("user32"));
		SwitchToThisWindow=(PSWITCHTOTHISWINDOW)GetProcAddress(hUser32,"SwitchToThisWindow");
		//ShowWindow(SW_SHOWNORMAL);
		SwitchToThisWindow(this->m_hWnd,TRUE);

	} else if(uIconMsg == WM_RBUTTONUP) {

		/*
		CDuiPoint point;
		::GetCursorPos(&point);			//获取鼠标指针位置
		CMenu menu;
		menu.LoadMenu(IDR_TRAYMENU);			//加载菜单资源
		CMenu *pSubMenu = menu.GetSubMenu(0);	//获取第一个子菜单

		SetForegroundWindow();	//设置为前景窗口
		//弹出菜单
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, Curpt.x, Curpt.y, this);
		menu.DestroyMenu();		//释放菜单
		PostMessage(WM_NULL, 0, 0);	
		*/
		
		CDuiPoint point;
		::GetCursorPos(&point);
		CMenuWnd* m_pMenu = new CMenuWnd();
		if( m_pMenu == NULL ) { return 0; }
		POINT pt = {point.x, point.y};
		//::ClientToScreen(*this, &pt);
		m_pMenu->Init(m_PaintManager.GetFocus(), pt, &m_PaintManager);
	}

	bHandled = false;
	return 0;
}

LRESULT CAddInfoWnd::OnClose(UINT uMsg, WPARAM& wParam, LPARAM& lParam, BOOL& bHandled)
{
	if(wParam==1||wParam==0)
	{
		if(m_bCloseFlag)
		{
			CXmlMessageBox *pMeb = new CXmlMessageBox(_T("MessageBox.xml"));
			pMeb->Init(*this, _T("提示"), _T("是否退出?"), true);
			pMeb->ShowWindow(TRUE);
			m_bCloseFlag = false;
		}
	}
	else if ( wParam==6 )
	{
		bHandled = false;
		wParam = 1;
		lParam = 0;
	}
	return 0;
}

LRESULT CAddInfoWnd::XmlOnClose(UINT uMsg, WPARAM& wParam, LPARAM& lParam, BOOL& bHandled)
{
	if(wParam==1)
	{
		//::SendMessage((HWND)lParam, WM_CLOSE, 1, 0); 
		Close(6);
	}
	else if (wParam==2)
	{
		m_bCloseFlag = true;
	}
	return 0;
}

LRESULT CAddInfoWnd::OnDestroy(UINT uMsg, WPARAM& wParam, LPARAM& lParam, BOOL& bHandled)
{
	m_trayIcon.DeleteTrayIcon();
	bHandled = FALSE;
	::PostQuitMessage(0L);
	return 0;
}

void CAddInfoWnd::Notify( TNotifyUI& msg )
{
	if(msg.sType==_T("click"))
	{
		CDuiString name = msg.pSender->GetName();
		if(name==_T("ok")){
		} else if(name==_T("cancel")){
			
		}
	} else if (_tcsicmp(msg.sType, _T("itemactivate")) == 0){
		int iIndex = msg.pSender->GetTag();
		CDuiString sMessage = _T("Click: ");
		CListTextElementUI * pListElement = new CListTextElementUI;
		pListElement = (CListTextElementUI*)m_pList->GetItemAt(iIndex);
		string strPath = pListElement->GetText(1);
		//::MessageBox(NULL, strPath.c_str(), _T("提示(by tojen)"), MB_OK);
		ShellExecute(m_hWnd,"open", strPath.c_str(), NULL, NULL, SW_SHOW);
	} else if(msg.sType==_T("menu_open")){
		PSWITCHTOTHISWINDOW SwitchToThisWindow;
		HMODULE hUser32=GetModuleHandle(_T("user32"));
		SwitchToThisWindow=(PSWITCHTOTHISWINDOW)GetProcAddress(hUser32,"SwitchToThisWindow");
		SwitchToThisWindow(this->m_hWnd,TRUE);
	} else if(msg.sType==_T("menu_exit")){
		PSWITCHTOTHISWINDOW SwitchToThisWindow;
		HMODULE hUser32=GetModuleHandle(_T("user32"));
		SwitchToThisWindow=(PSWITCHTOTHISWINDOW)GetProcAddress(hUser32,"SwitchToThisWindow");
		SwitchToThisWindow(this->m_hWnd,TRUE);
		if(m_bCloseFlag)
		{
			CXmlMessageBox *pMeb = new CXmlMessageBox(_T("MessageBox.xml"));
			pMeb->Init(*this, _T("提示"), _T("是否退出?"), true);
			pMeb->ShowWindow(TRUE);
			m_bCloseFlag = false;
		}
	}

	__super::Notify(msg);
}