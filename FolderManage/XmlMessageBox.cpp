#include "XmlMessageBox.h"


CXmlMessageBox::CXmlMessageBox(LPCTSTR pszXMLPath)
	: CXmlWnd(pszXMLPath)
{
	
}

CXmlMessageBox::~CXmlMessageBox(void)
{
}

void CXmlMessageBox::Init( HWND hWndParent, LPCTSTR strTitleName, LPCTSTR strBodyContent, bool bClose)
{
	m_bClose = bClose;
	m_hWndParent = hWndParent;
	Create(hWndParent, strTitleName, UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	CenterWindow();
	/*POINT ptPos;
	::ClientToScreen(hWndParent, &ptPos);
	::SetWindowPos(*this, NULL, ptPos.x, ptPos.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);*/
	CLabelUI* pLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("title")));
	pLabel->SetText(strTitleName);
	CTextUI* pText = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("body")));
	pText->SetText(strBodyContent);
}

void CXmlMessageBox::OnFinalMessage( HWND /*hWnd*/ )
{
	delete this;
}

void CXmlMessageBox::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("click") )
	{
		if( msg.pSender->GetName() == _T("ok") ) 
		{	
			if(m_bClose)
				::SendMessage(m_hWndParent, XML_WN_COLSE, 1, (LPARAM)this->m_hWnd);
			else
				Close();
		}
		else if( msg.pSender->GetName() == _T("cancel")||msg.pSender->GetName() == _T("close") )
		{
			if(m_bClose)
				::SendMessage(m_hWndParent, XML_WN_COLSE, 2, (LPARAM)this->m_hWnd);
			Close();
		}
	}
	 __super::Notify(msg);
}

LRESULT CXmlMessageBox::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/)
{
	if (uMsg == WM_KEYDOWN)
	{
		switch (wParam)
		{
		case VK_ESCAPE:
			return 0;
		default:
			break;
		}
	}
	return 0;
}


//LRESULT CXmlMessageBox::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	LRESULT lRes = 0;
//	BOOL    bHandled = TRUE;
//
//	switch( uMsg )
//	{
//	case WM_KILLFOCUS:     lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); break; 
//	default:
//		bHandled = FALSE;
//		break;
//	}
//
//	if( bHandled ) return lRes;
//	return __super::HandleMessage(uMsg, wParam, lParam);
//}

//LRESULT CXmlMessageBox::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//	if( (HWND)wParam == m_hWnd ) bHandled = TRUE;
//	Close();
//	bHandled = FALSE;
//    return 0;
//}