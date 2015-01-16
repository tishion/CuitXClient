// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"
#include "xClient.h"
#include "IPConfig.h"

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam== VK_ESCAPE)
		{
			return TRUE;
		}
	}
	return CWindow::IsDialogMessage(pMsg);
}

LRESULT CMainDlg::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == XCLIENT_CMD)
	{
		switch(wParam)
		{
		case XC_MESSAGE:
			AppandMessage(lParam);
			break;
		case XC_IPCONFIG:
			m_ButtonStart.EnableWindow(lParam);
			break;
		case XC_DISCONNETC:
			xclient->DisConnect();
			setEnableControls(true);
		case XC_NOTIFY:
			ShowBalloonToolTips(lParam);
			break;
		case XC_CONSUCCESS:
			ToTrayIcon();
			ShowBalloonToolTips(TEXT("连接建立,程序已最小化到系统托盘。"));
		default:
			break;
		}
	}
	return 0;
}

BOOL CMainDlg::OnIdle()
{
	//UIUpdateChildWindows();
	return FALSE;
}

void CMainDlg::setEnableControls(bool en)
{
	m_bStarted = !en;
	m_EditAccount.EnableWindow(en);
	m_EditPasswd.EnableWindow(en);
	m_CombNicList.EnableWindow(en);
	m_CombClientVer.SetEditSel(-1, 0);
	m_CombClientVer.EnableWindow(en);
	m_ButtonBroadCast.EnableWindow(en);
	m_ButtonMultiCast.EnableWindow(en);
	m_ButtonPktSingleCast.EnableWindow(en);
	m_ButtonPktMultiCast.EnableWindow(en);
	if (en)
	{
		m_ButtonStart.SetWindowText(TEXT("开始认证"));
	}
	else
	{
		m_ButtonStart.SetWindowText(TEXT("断开连接"));
	}
}

void CMainDlg::ReadInitData()
{
	DWORD nConut;

	m_nBubbleNotify = 1;
	m_nAutoRun = 1;
	m_nCastSet = 1;
	m_nPktCastSet = 0;
	m_nKeepPasswd = 0;
	m_nAutoStart = 0;
	m_szAccount = TEXT("");
	m_szPasswd = TEXT("");
	m_szNicName = TEXT("");
	m_szClientVer.LoadString(IDS_DEFAULT_VER);

	keyname.LoadString(IDS_CONFIGKEY);
	LONG lResult = regKey.Open(HKEY_LOCAL_MACHINE, keyname);
	if (lResult == ERROR_SUCCESS)
	{
		nConut = INPUT_MAX;
		keyname.LoadString(IDS_KEY_ACCOUNT);
		if (ERROR_SUCCESS != regKey.QueryStringValue(keyname, m_szAccount.GetBuffer(INPUT_MAX), &nConut))
		{
			m_szAccount.ReleaseBuffer();
			keyname.LoadString(IDS_KEY_ACCOUNT);
			regKey.SetStringValue(keyname, m_szAccount);
			keyname.LoadString(IDS_KEY_PASSWD);
			regKey.SetBinaryValue(keyname, m_szPasswd.GetBuffer(INPUT_MAX), 0);
		}
		m_szAccount.ReleaseBuffer();

		nConut = INPUT_MAX * sizeof(TCHAR) ;
		keyname.LoadString(IDS_KEY_PASSWD);
		if (ERROR_SUCCESS != regKey.QueryBinaryValue(keyname, m_szPasswd.GetBuffer(INPUT_MAX), &nConut))
		{
			m_szPasswd.ReleaseBuffer();
			regKey.SetBinaryValue(keyname, m_szPasswd.GetBuffer(INPUT_MAX), 0);
		}
		m_szPasswd.ReleaseBuffer();

		nConut = MAX_ADAPTER_NAME_LENGTH;
		keyname.LoadString(IDS_KEY_NICNAME);
		if (ERROR_SUCCESS != regKey.QueryStringValue(keyname, m_szNicName.GetBuffer(MAX_ADPTER_NAME_BUFLEN), &nConut))
		{
			m_szNicName.ReleaseBuffer();
			keyname.LoadString(IDS_KEY_NICNAME);
			regKey.SetStringValue(keyname, m_szNicName);
		}
		m_szNicName.ReleaseBuffer();

		nConut = VER_MAX;
		keyname.LoadString(IDS_KEY_CLIENTVER);
		if (ERROR_SUCCESS != regKey.QueryStringValue(keyname, m_szClientVer.GetBuffer(VER_MAX), &nConut))
		{
			m_szClientVer.ReleaseBuffer();
			regKey.SetStringValue(keyname, m_szClientVer);
		}
		m_szClientVer.ReleaseBuffer();

		keyname.LoadString(IDS_KEY_AUTOSTART);
		if (ERROR_SUCCESS != regKey.QueryDWORDValue(keyname, m_nAutoStart))
		{
			regKey.SetDWORDValue(keyname, m_nAutoStart);
		}

		keyname.LoadString(IDS_KEY_KEEPPASSWD);
		if (ERROR_SUCCESS != regKey.QueryDWORDValue(keyname, m_nKeepPasswd))
		{
			regKey.SetDWORDValue(keyname, m_nKeepPasswd);
		}

		keyname.LoadString(IDS_KEY_CASTSET);
		if (ERROR_SUCCESS != regKey.QueryDWORDValue(keyname, m_nCastSet))
		{
			regKey.SetDWORDValue(keyname, m_nCastSet);
		}

		keyname.LoadString(IDS_KEY_PKTCASTSET);
		if (ERROR_SUCCESS != regKey.QueryDWORDValue(keyname, m_nPktCastSet))
		{
			regKey.SetDWORDValue(keyname, m_nPktCastSet);
		}

		keyname.LoadString(IDS_KEY_AUTORUN);
		if (ERROR_SUCCESS != regKey.QueryDWORDValue(keyname, m_nAutoRun))
		{
			regKey.SetDWORDValue(keyname, m_nAutoRun);
		}

		keyname.LoadString(IDS_KEY_BUBBLENOTIFY);
		if (ERROR_SUCCESS != regKey.QueryDWORDValue(keyname, m_nBubbleNotify))
		{
			regKey.SetDWORDValue(keyname, m_nBubbleNotify);
		}
	}
	else
	{
		keyname.LoadString(IDS_CONFIGKEY);
		regKey.Create(HKEY_LOCAL_MACHINE, keyname);
		keyname.LoadString(IDS_CONFIGKEY);
		regKey.Open(HKEY_LOCAL_MACHINE, keyname);

		keyname.LoadString(IDS_KEY_ACCOUNT);
		regKey.SetStringValue(keyname, m_szAccount);

		keyname.LoadString(IDS_KEY_PASSWD);
		regKey.SetBinaryValue(keyname, m_szPasswd.GetBuffer(INPUT_MAX), 0);

		keyname.LoadString(IDS_KEY_NICNAME);
		regKey.SetStringValue(keyname, m_szNicName);

		keyname.LoadString(IDS_KEY_CLIENTVER);
		regKey.SetStringValue(keyname, m_szClientVer);

		keyname.LoadString(IDS_KEY_AUTOSTART);
		regKey.SetDWORDValue(keyname, m_nAutoStart);

		keyname.LoadString(IDS_KEY_KEEPPASSWD);
		regKey.SetDWORDValue(keyname, m_nKeepPasswd);

		keyname.LoadString(IDS_KEY_CASTSET);
		regKey.SetDWORDValue(keyname, m_nCastSet);

		keyname.LoadString(IDS_KEY_PKTCASTSET);
		regKey.SetDWORDValue(keyname, m_nPktCastSet);

		keyname.LoadString(IDS_KEY_AUTORUN);
		regKey.SetDWORDValue(keyname, m_nAutoRun);

		keyname.LoadString(IDS_KEY_BUBBLENOTIFY);
		regKey.SetDWORDValue(keyname, m_nBubbleNotify);
	}
}

bool CMainDlg::EnumNic()
{
	m_NicList.RemoveAll();
	m_CombNicList.ResetContent();

	PIP_ADAPTER_INFO pAdapter = NULL;
	PIP_ADAPTER_INFO pAdaptersInfo;
	DWORD dwRetVal = 0;
	NICDATA nicdata;

	{//START_ENUM_ADPATERS_INFO:
		ULONG ulOutBufLen = 0;
		DWORD dwRetVal = 0;

		pAdaptersInfo = (PIP_ADAPTER_INFO ) ::GlobalAlloc(GPTR, sizeof(IP_ADAPTER_INFO));
		ulOutBufLen = sizeof(IP_ADAPTER_INFO);

		// Make an initial call to GetAdaptersInfo to get
		// the necessary size into the ulOutBufLen variable
		if (::GetAdaptersInfo( pAdaptersInfo, &ulOutBufLen) != ERROR_SUCCESS)
		{
			::GlobalFree (pAdaptersInfo);
			pAdaptersInfo = (IP_ADAPTER_INFO *)::GlobalAlloc(GPTR, ulOutBufLen);
		}

		if ((dwRetVal = ::GetAdaptersInfo(pAdaptersInfo, &ulOutBufLen)) != NO_ERROR)
		{
			::GlobalFree(pAdaptersInfo);
			return  false;
		}
	}////END_ENUM_ADPATERS_INFO:
	USES_CONVERSION;

	pAdapter = pAdaptersInfo;
	while (pAdapter) {
		nicdata.name.Format(CA2T(pAdapter->AdapterName));
		nicdata.description.Format(CA2T(pAdapter->Description));
		::memcpy(nicdata.mac, pAdapter->Address, 6);
		nicdata.index = pAdapter->Index;
		m_NicList.Add(nicdata);
		m_CombNicList.InsertString(-1, nicdata.description);

		pAdapter = pAdapter->Next;
	}
	::GlobalFree(pAdaptersInfo);
	return true;
}

void CMainDlg::InitNicListCombox()
{
	m_CombNicList.SetCurSel(-1);
	if (!EnumNic())
	{
		return ;
	}
	for (int i=0; i<m_NicList.GetSize(); i++)
	{
		if (m_szNicName == m_NicList[i].name)
		{
			m_CombNicList.SetCurSel(i);
			m_useNic = m_NicList[i];
			return ;
		}
	}
}
void CMainDlg::InitVerListCombox()
{
	m_CombClientVer.LimitText(VER_MAX);
	CString clientver(MAKEINTRESOURCE(IDS_DEFAULT_VER));
	m_CombClientVer.InsertString(-1, clientver);
	clientver.LoadString(IDS_VER_1);
	m_CombClientVer.InsertString(-1, clientver);
	m_CombClientVer.SetWindowText(m_szClientVer);
	return ;
}

void CMainDlg::AppandMessage(ATL::_U_STRINGorID message)
{
	int index;
	CString temp;
	CTime nowtime = CTime::GetCurrentTime();
	LPTSTR lpstrMessage = NULL;
	if(IS_INTRESOURCE(message.m_lpstr))
	{
		for(int nLen = 256; ; nLen *= 2)
		{
			ATLTRY(lpstrMessage = new TCHAR[nLen]);
			if(lpstrMessage == NULL)
			{
				ATLASSERT(FALSE);
				return ;
			}
			int nRes = ::LoadString(ModuleHelper::GetResourceInstance(), LOWORD(message.m_lpstr), lpstrMessage, nLen);
			if(nRes < nLen - 1)
				break;
			delete [] lpstrMessage;
			lpstrMessage = NULL;
		}
		message.m_lpstr = lpstrMessage;
	}
	
	nowtime.GetCurrentTime();
	temp.Format(TEXT("[%02d:%02d:%02d]:%s"),
		nowtime.GetHour(), nowtime.GetMinute(), 
		nowtime.GetSecond(), message.m_lpstr);
	index = m_ListBoxLog.InsertString(-1, temp);
	m_ListBoxLog.SetTopIndex(index);

	delete [] lpstrMessage;
	return ;
}

void CMainDlg::ShowBalloonToolTips(ATL::_U_STRINGorID message)
{
	LPTSTR lpstrMessage = NULL;
	if(IS_INTRESOURCE(message.m_lpstr))
	{
		for(int nLen = 256; ; nLen *= 2)
		{
			ATLTRY(lpstrMessage = new TCHAR[nLen]);
			if(lpstrMessage == NULL)
			{
				ATLASSERT(FALSE);
				return ;
			}
			int nRes = ::LoadString(ModuleHelper::GetResourceInstance(), LOWORD(message.m_lpstr), lpstrMessage, nLen);
			if(nRes < nLen - 1)
				break;
			delete [] lpstrMessage;
			lpstrMessage = NULL;
		}
		message.m_lpstr = lpstrMessage;
	}
	BalloonToolTips(message.m_lpstr);
	delete [] lpstrMessage;
	return ;
}

void CMainDlg::ToTrayIcon()
{
	ShowWindow(SW_HIDE);
	return ;
}

bool CMainDlg::Start()
{
	if (m_bStarted == true)
	{
		xclient->DisConnect();
		setEnableControls(true);
		return true;
	}

	m_EditAccount.GetWindowText(m_szAccount);
	m_EditPasswd.GetWindowText(m_szPasswd);
	m_CombClientVer.GetWindowText(m_szClientVer);

	if (m_szAccount.GetLength()==0 || m_szPasswd.GetLength() ==0)
	{
		AtlMessageBox(m_hWnd, IDS_STRING144);
		return false;
	}

	if (m_CombNicList.GetCurSel() == CB_ERR)
	{
		AtlMessageBox(m_hWnd, IDS_STRING145);
		return false;
	}

	if (m_nCastSet >1 || m_nCastSet < 0)
	{
		AtlMessageBox(m_hWnd, IDS_STRING146);
		return false;
	}

	if (m_nPktCastSet > 1 || m_nPktCastSet <0)
	{
		AtlMessageBox(m_hWnd, IDS_STRING147);
		return false;
	}

	if (m_szClientVer.GetLength() == 0)
	{
		AtlMessageBox(m_hWnd, IDS_STRING148);
		return false;
	}
	setEnableControls(false);
	m_ListBoxLog.ResetContent();

	keyname.LoadString(IDS_KEY_ACCOUNT);
	regKey.SetStringValue(keyname, m_szAccount);


	keyname.LoadString(IDS_KEY_PASSWD);
	if (m_nKeepPasswd == 0)
	{
		regKey.SetBinaryValue(keyname, m_szPasswd.GetBuffer(INPUT_MAX), 0);	
	}
	else
	{
		regKey.SetBinaryValue(keyname, m_szPasswd.GetBuffer(INPUT_MAX), INPUT_MAX*sizeof(TCHAR));	
	}
	


	keyname.LoadString(IDS_KEY_CLIENTVER);
	regKey.SetStringValue(keyname, m_szClientVer);

	keyname.LoadString(IDS_KEY_NICNAME);
	regKey.SetStringValue(keyname, m_useNic.name);

	sprintf_s(m_xcontdata.account, INPUT_MAX, "%s", CT2A(m_szAccount));
	sprintf_s(m_xcontdata.passwd, INPUT_MAX , "%s", CT2A(m_szPasswd));
	sprintf_s(m_xcontdata.clientver, VER_MAX, "%s", CT2A(m_szClientVer));
	sprintf_s(m_xcontdata.nicname, MAX_ADPTER_NAME_BUFLEN, "%s", CT2A(m_useNic.name));
	::memcpy(m_xcontdata.nicmac, m_useNic.mac, 6);
	m_xcontdata.nicindex = m_useNic.index;
	m_xcontdata.castset = m_nCastSet;
	m_xcontdata.pktcastset = m_nPktCastSet;

	xclient->Connect(&m_xcontdata);
	return true;
}

LRESULT CMainDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	{//Controls initialization；
	ReadInitData();
	m_ButtonStart.Attach(GetDlgItem(IDC_BUTTON_START));
	m_ButtonOption.Attach(GetDlgItem(IDC_BUTTON_OPTION));
	m_ListBoxLog.Attach(GetDlgItem(IDC_LIST_LOG));
	m_EditAccount.Attach(GetDlgItem(IDC_EDIT_ACCOUNT));
	m_EditPasswd.Attach(GetDlgItem(IDC_EDIT_PASSWD));
	m_EditAccount.SetLimitText(INPUT_MAX);
	m_EditPasswd.SetLimitText(INPUT_MAX);	

	m_ButtonBroadCast.Attach(GetDlgItem(IDC_RADIO_CAST));
	m_ButtonMultiCast.Attach(GetDlgItem(IDC_RADIO_CAST1));
	m_ButtonPktSingleCast.Attach(GetDlgItem(IDC_RADIO_PKTCAST));
	m_ButtonPktMultiCast.Attach(GetDlgItem(IDC_RADIO_PKTCAST1));

	m_CombClientVer.Attach(GetDlgItem(IDC_COMBO_CLIENTVER));
	InitVerListCombox();

	m_CombNicList.Attach(GetDlgItem(IDC_COMBO_NIC));
	InitNicListCombox();
	
	DoDataExchange(FALSE);
	xclient = new xClient(this);
	m_bStarted = false;
	}
	{
	//Expand window initialization
	m_bExpand = false;
	RECT rcExpwnd,rcDlg;
	GetWindowRect(&rcDlg);
	m_nFullWidth =rcDlg.right - rcDlg.left;
	GetDlgItem(IDC_STATIC_OPTIONFRAME).GetWindowRect(&rcExpwnd);
	m_nNormalWidth = rcDlg.right - (rcExpwnd.right - rcExpwnd.left);
	rcDlg.right = rcDlg.left + m_nNormalWidth;
	MoveWindow(&rcDlg,TRUE);
	}
	// center the dialog on the screen
	CenterWindow();


	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);
	CreateTrayIcon();

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	//UIAddChildWindowContainer(m_hWnd);
	if (m_nAutoStart == 1)
	{
		Start();
	}
	return TRUE;
}

LRESULT CMainDlg::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (IsIconic() == TRUE && m_bStarted == true)
	{
		ToTrayIcon();
		ShowBalloonToolTips(TEXT("程序已最小化到系统托盘。"));
	}
	return 0;
}

LRESULT CMainDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DeleteTrayIcon();
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

LRESULT CMainDlg::OnQueryEndSession(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//AtlMessageBox(this->m_hWnd, _T("关机?"));
	
	if (m_bStarted == true)
	{
		xclient->SendLogoffPkt();
		::IPConfig(IPCON_RELEASE, xclient->pxcondata->nicindex, xclient->pxcondata->nicname);
	}

	AtlTrace(TEXT("Received WM_QUERYENDSESSION message\n"));
	return TRUE;
}

LRESULT CMainDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (m_bStarted == false)
	{
		CloseDialog(wID);
	}
	else
	{
		ToTrayIcon();
		ShowBalloonToolTips(TEXT("程序已最小化到系统托盘。"));
	}
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	if (m_bStarted == true)
	{
		AtlMessageBox(m_hWnd, IDS_STRING142,
			IDR_MAINFRAME, MB_OK | MB_ICONINFORMATION);
		return ;
	}
	regKey.Flush();
	regKey.Close();
	DestroyWindow();
	::PostQuitMessage(nVal);
}

LRESULT CMainDlg::OnAppAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonOption(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// TODO: 在此添加控件通知处理程序代码
	m_bExpand = !m_bExpand;
	RECT rcDlg;
	GetWindowRect(&rcDlg);
	if (m_bExpand)//展开模式
	{
		rcDlg.right = rcDlg.left + m_nFullWidth;
		m_ButtonOption.SetWindowText(TEXT("选项(&O)<"));
	}
	else//折叠模式
	{
		rcDlg.right = rcDlg.left + m_nNormalWidth;
		m_ButtonOption.SetWindowText(TEXT("选项(&O)>"));
	}
	MoveWindow(&rcDlg, TRUE);
	return 0;
}


LRESULT CMainDlg::OnBnClickedCheckKeeppasswd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// TODO: 在此添加控件通知处理程序代码
	DoDataExchange(TRUE, IDC_CHECK_KEEPPASSWD);
	keyname.LoadString(IDS_KEY_KEEPPASSWD);
	regKey.SetDWORDValue(keyname, m_nKeepPasswd);
	//清除已存储的密码
	keyname.LoadString(IDS_KEY_PASSWD);
	if (m_nKeepPasswd == 0)
	{
		regKey.SetBinaryValue(keyname, m_szPasswd.GetBuffer(INPUT_MAX), 0);	
	}
	else
	{
		regKey.SetBinaryValue(keyname, m_szPasswd.GetBuffer(INPUT_MAX), INPUT_MAX*sizeof(TCHAR));	
	}
	return 0;
}

LRESULT CMainDlg::OnBnClickedCheckAutostart(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// TODO: 在此添加控件通知处理程序代码
	DoDataExchange(TRUE, IDC_CHECK_AUTOSTART);
	keyname.LoadString(IDS_KEY_AUTOSTART);
	regKey.SetDWORDValue(keyname, m_nAutoStart);
	return 0;
}

LRESULT CMainDlg::OnBnClickedCheckAutorun(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// TODO: 在此添加控件通知处理程序代码
	DoDataExchange(TRUE, IDC_CHECK_AUTORUN);
	keyname.LoadString(IDS_KEY_AUTORUN);
	regKey.SetDWORDValue(keyname, m_nAutoRun);
	return 0;
}

LRESULT CMainDlg::OnBnClickedCheckBubblenotify(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// TODO: 在此添加控件通知处理程序代码
	DoDataExchange(TRUE, IDC_CHECK_BUBBLENOTIFY);
	keyname.LoadString(IDS_KEY_BUBBLENOTIFY);
	regKey.SetDWORDValue(keyname, m_nBubbleNotify);
	return 0;
}

LRESULT CMainDlg::OnBnClickedRadioCast(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// TODO: 在此添加控件通知处理程序代码
	DoDataExchange(TRUE, IDC_RADIO_CAST);
	keyname.LoadString(IDS_KEY_CASTSET);
	regKey.SetDWORDValue(keyname, m_nCastSet);
	return 0;
}


LRESULT CMainDlg::OnBnClickedRadioPktcast(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	DoDataExchange(TRUE, IDC_RADIO_PKTCAST);
	keyname.LoadString(IDS_KEY_PKTCASTSET);
	regKey.SetDWORDValue(keyname, m_nPktCastSet);
	return 0;
}


LRESULT CMainDlg::OnCbnDropdownComboNic(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	AtlTrace(TEXT("Down\n"));
	if (!EnumNic())
	{
		AtlMessageBox(m_hWnd, IDS_STRING143);
		return -1;
	}
	return 0;
}

LRESULT CMainDlg::OnCbnSelchangeComboNic(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	AtlTrace(TEXT("Change\n"));
	int cursel = m_CombNicList.GetCurSel();
	m_useNic = m_NicList[cursel];
	AtlTrace(TEXT("Nic Name:") + m_useNic.name + TEXT("\n"));
	AtlTrace(TEXT("Nic description:")  + m_useNic.description + TEXT("\n"));
	return 0;
}

LRESULT CMainDlg::OnCbnCloseUpComboNic(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	AtlTrace(TEXT("Close\n"));
	int cursel = m_CombNicList.GetCurSel();
	if (cursel == CB_ERR)
	{
		m_useNic.name = TEXT("");
		m_useNic.description = TEXT("");
		::memset(m_useNic.mac, 0 , 6);
	}
	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonStart(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	Start();
	return 0;
}