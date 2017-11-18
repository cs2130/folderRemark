#include "stdafx.h"
#include "resource.h"
#include "AddInfoWnd.h"
#include "XmlMessageBox.h"

CAddInfoWnd::CAddInfoWnd( LPCTSTR pszXMLPath )
	: CXmlWnd(pszXMLPath)

{
}

void CAddInfoWnd::InitWindow()
{
	SetIcon(IDI_ICON); // 设置任务栏图标
	CenterWindow();
	m_bCloseFlag = true;
	m_strLoad = "";
	string strLoad = GetCommandLine();

	CRichEditUI* pPathKey = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("path")));
	m_pInfoKey = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("info")));

	m_pInfoKey->SetLimitText(1024);  
	if( isDir(strLoad.c_str()) ){
		m_strLoad = strLoad;
	} else {
		int nPos = strLoad.rfind('\\');
		
		if(strLoad.substr(0,1)=="\""){
			m_strLoad = strLoad.substr(1,nPos-1);
		}else{
			m_strLoad = strLoad.substr(0,nPos);
		}
	}
	string strIni = m_strLoad+"\\desktop.ini";
	char cInfo[1024];
	memset(cInfo,0,1024);
	::GetPrivateProfileString(".ShellClassInfo","InfoTip","",cInfo,sizeof(cInfo),strIni.c_str());
	pPathKey->SetText(m_strLoad.c_str());
	m_pInfoKey->SetText(cInfo);

	TCHAR exeFullPath[MAX_PATH]; // MAX_PATH
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	m_strOwnDir = exeFullPath;
	int nPosDir = m_strOwnDir.rfind('\\');
	m_strOwnDir = m_strOwnDir.substr(0,nPosDir);
	m_sqlSexecute.SqlSetPath(m_strOwnDir.c_str());
	TCHAR sql[4096] = {""};
	m_sqlSexecute.SqlCreate(sql);
}

bool CAddInfoWnd::isDir(const char* path) {
	struct _stat buf = {0};
	_stat(path, &buf);
	return buf.st_mode & _S_IFDIR;
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
		case VK_ESCAPE:
			return FALSE;
		case VK_RETURN:
			return FALSE;
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
	case WM_TIMER: lRes = OnTimer(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:	lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case XML_WN_COLSE: lRes = XmlOnClose(uMsg, wParam, lParam, bHandled); break;
	//case WM_CHAR: lRes = OnChar(uMsg, wParam, lParam, bHandled); break;
	case ALL_WN_CLOSE: Close(6); break;
	default:
		bHandled = FALSE;
		break;
	}

	if(bHandled) 
		return lRes;
	// || m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes)
	return __super::HandleMessage(uMsg, wParam, lParam);
}

/*
LRESULT CAddInfoWnd::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){
	if (m_pInfoKey->IsFocused()) {
		CheckUploadInput();
	}
	bHandled = FALSE;
	return 0;
}

void CAddInfoWnd::CheckUploadInput()  
{  
	string strContent = "";  
	string strContentCheck = ""; 
	strContent = m_pInfoKey->GetText();
	::MessageBox(NULL, strContent.c_str(), _T("提示(by tojen)"), MB_OK);
	RemoveEnterChar(strContent, strContentCheck);
	m_pInfoKey->SetText(strContentCheck.c_str());
}
*/

void CAddInfoWnd::RemoveEnterChar(string& strIn, string& strOut)
{
	int iIndex = 0;
	int iLenSrc = _tcslen(strIn.c_str());
	WCHAR   cTemp = L'\0';
	strOut = "";

	for (iIndex = 0; iIndex < iLenSrc; iIndex++)
	{
		cTemp = strIn[iIndex];
		if ((cTemp != L'\r') && (cTemp != L'\n'))
			strOut += cTemp;
	}
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
	::PostQuitMessage(0L);
	bHandled = FALSE;
	return 0;
}

void CAddInfoWnd::Notify( TNotifyUI& msg )
{
	if(msg.sType==_T("click"))
	{
		CDuiString name = msg.pSender->GetName();
		if(name==_T("ok")){
			WriteInfo();
		} else if(name==_T("cancel")){
			if(m_bCloseFlag) {
				CXmlMessageBox *pMeb = new CXmlMessageBox(_T("MessageBox.xml"));
				pMeb->Init(*this, _T("提示"), _T("是否退出?"), true);
				pMeb->ShowWindow(TRUE);
				m_bCloseFlag = false;
			}
		}
	}

	__super::Notify(msg);
}

LRESULT CAddInfoWnd::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	/*
	HWND hwnd = m_PaintManager.GetPaintWindow();
	KillTimer(hwnd, 1);
	string strDat = m_strLoad;
	string strDatIni = m_strLoad+"\\desktop.ini";
	SetFileAttributes(strDat.c_str(), FILE_ATTRIBUTE_SYSTEM);
	SetFileAttributes(strDatIni.c_str(), FILE_ATTRIBUTE_SYSTEM|FILE_ATTRIBUTE_HIDDEN);
	::SendMessage(hwnd, ALL_WN_CLOSE, 1, (LPARAM)this->m_hWnd);
	*/
	return 0;
}

void CAddInfoWnd::WriteInfo(){
	CRichEditUI* pPathKey = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("path")));
	string strPath = pPathKey->GetText();
	CRichEditUI* pInfoKey = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("info")));
	string strInfo = pInfoKey->GetText();
	string strInfoOut = "";
	RemoveEnterChar(strInfo, strInfoOut);
	if (strPath.size()==0){
		CXmlMessageBox *pMeb = new CXmlMessageBox(_T("MessageBox.xml"));
		pMeb->Init(*this, _T("提示"), _T("路径为空,不能添加备注!"), false);
		pMeb->ShowWindow(TRUE);
		pPathKey->SetFocus();
		return;
	}

	if (strInfoOut.size()==0){
		CXmlMessageBox *pMeb = new CXmlMessageBox(_T("MessageBox.xml"));
		pMeb->Init(*this, _T("提示"), _T("请添加备注内容!"), false);
		pMeb->ShowWindow(TRUE);
		pInfoKey->SetFocus();
		return;
	}

	/*
	int begin = 0;
	begin = strInfo.find(" ",begin); //查找空格在str中第一次出现的位置
	while(begin!=-1) //表示字符串中存在空格
	{
		strInfo.replace(begin, 1, "");  // 用空串替换str中从begin开始的1个字符
		begin = strInfo.find(" ",begin);  //查找空格在替换后的str中第一次出现的位置
	}

	int beginr = 0;
	beginr = strInfo.find("\n",beginr); //查找空格在str中第一次出现的位置
	while(beginr!=-1) //表示字符串中存在空格
	{
		strInfo.replace(beginr, 2, "");  // 用空串替换str中从begin开始的1个字符
		beginr = strInfo.find(" ",beginr);  //查找空格在替换后的str中第一次出现的位置
	}
	*/

	HWND hwnd = m_PaintManager.GetPaintWindow();
	string strIni = m_strLoad+"\\desktop.ini";
	::WritePrivateProfileString(_T(".ShellClassInfo"),_T("InfoTip"),strInfoOut.c_str(), strIni.c_str());
	string strDat = m_strLoad;
	string strDatIni = m_strLoad+"\\desktop.ini";
	SetFileAttributes(strDat.c_str(), FILE_ATTRIBUTE_SYSTEM);
	SetFileAttributes(strDatIni.c_str(), FILE_ATTRIBUTE_SYSTEM|FILE_ATTRIBUTE_HIDDEN);

	int nPos = m_strLoad.rfind('\\');
	string strName = m_strLoad.substr(nPos+1,m_strLoad.size());

	TCHAR sql[1024] = {0};
	_stprintf(sql,_T("delete from folder where fr_folder = '%s'"), m_strLoad.c_str());
	m_sqlSexecute.SqlDeleteAddPrject(sql);
	memset(sql,0,sizeof(sql));
	_stprintf(sql,_T("insert into folder (fr_name, fr_folder, fr_info) values ('%s','%s','%s')"), strName.c_str(), m_strLoad.c_str(), strInfoOut.c_str());
	m_sqlSexecute.SqlInsertAddPrject(sql);
	Close(6);

	//SetTimer(hwnd, 1, 1000, NULL);
	//string strDatIni = "attrib +s +h " + m_strLoad+"\\desktop.ini";
	//WinExec(strDatIni.c_str(), SW_HIDE);
	//Close(6);
	//ShellExecute(this->m_hWnd, "open","cmd.exe", strDat.c_str(),"", SW_SHOW);
	//ShellExecute(this->m_hWnd, "open","cmd.exe", strDatIni.c_str(),"", SW_SHOW);
	//ShellExecute(NULL,"open",strDat.c_str(),NULL,NULL,SW_SHOWNORMAL); 
	//ShellExecute(NULL,"open",strDatIni.c_str(),NULL,NULL,SW_SHOWNORMAL); 
	//ShellExecute(NULL, NULL, "attrib",strDat.c_str(),NULL,SW_HIDE);
	//ShellExecute(NULL, NULL, "attrib",strDatIni.c_str(),NULL,SW_HIDE);

	//WinExec(strDat.c_str(), SW_HIDE);
	//WinExec(strDatIni.c_str(), SW_HIDE);
	//SetTimer(hwnd, 1, 2000, NULL);

	//attrib +s "E:/JAVA/Container"（文件夹路径，自行更改）
	//attrib +s +h "E:/JAVA/Container/desktop.ini"（文件路径，自行更改）
	/*system("odbcconf.exe /S /Lv odbcconf.log /F %systemroot%\system32\mdaccore.rsp");
	system("odbcconf.exe /S /Lv odbcconf.log /F %systemroot%\system32\sqlclnt.rsp");
	system("odbcconf.exe /S /Lv odbcconf.log /F %systemroot%\system32\odbcconf.rsp");
	system("odbcconf.exe /S /Lv odbcconf.log /F %systemroot%\system32\redist.rsp");*/

	//WinExec("odbcconf.exe /S /Lv odbcconf.log /F %systemroot%\system32\sqlclnt.rsp", SW_HIDE);
	//WinExec("odbcconf.exe /S /Lv odbcconf.log /F %systemroot%\system32\odbcconf.rsp", SW_HIDE);
	//WinExec("odbcconf.exe /S /Lv odbcconf.log /F %systemroot%\system32\redist.rsp", SW_HIDE);
}