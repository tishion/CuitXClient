//ShellIcon.h 

#pragma once 
#define WM_ICON WM_USER + 180 

template <class T, class MenuT, int MenuID> 
class CTrayIcon : public MenuT 
{ 
private: 
	NOTIFYICONDATA m_data; 
	UINT m_msgTaskbarRestart; 
	CString m_appName; 
public: 
	CTrayIcon()
	{    
		m_appName.LoadString(IDR_MAINFRAME); 
		m_msgTaskbarRestart = RegisterWindowMessage(TEXT("TaskbarCreated")); 
	} 
	~CTrayIcon()
	{  
		::Shell_NotifyIcon(NIM_DELETE, &m_data); 
	} 
	BOOL CreateTrayIcon()
	{ 
		T* pT = static_cast<T*>(this); 
		SecureZeroMemory(&m_data, sizeof(m_data)); 
		m_data.cbSize = sizeof(m_data); 
		m_data.hIcon = LoadIcon(_Module.get_m_hInst(), MAKEINTRESOURCE(IDR_MAINFRAME)); 
		m_data.hWnd = pT->m_hWnd; 
		m_data.uID = IDR_MAINFRAME; 
		m_data.uFlags = NIF_ICON | NIF_MESSAGE | NIF_INFO | NIF_TIP; 
		m_data.uCallbackMessage = WM_ICON; 
		m_data.dwInfoFlags = NIIF_USER; 
		::_tcscpy_s(m_data.szInfoTitle, m_appName); 
		::_tcscpy_s(m_data.szTip, m_appName); 
		return Shell_NotifyIcon(NIM_ADD, &m_data); 
	} 

	BOOL ShowIconMenu(UINT ID_Menu) 
	{ 
		T* pT = static_cast<T*>(this); 
		CMenu menu; 
		menu.LoadMenu(ID_Menu); 

		CMenu SubMenu(menu.GetSubMenu(0)); 
		POINT pos; 
		::GetCursorPos(&pos); 
		::SetForegroundWindow(pT->m_hWnd);
		SubMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_HORPOSANIMATION, pos.x, pos.y, pT->m_hWnd); 
		return TRUE; 
	} 

	BOOL DeleteTrayIcon()
	{ 
		return ::Shell_NotifyIcon(NIM_DELETE, &m_data); 
	} 

	void ModifyToolTips(LPCTSTR info) 
	{ 
		::_tcscpy_s(m_data.szInfo, info); 
	} 

	BOOL DispalyToolTips() 
	{ 
		return ::Shell_NotifyIcon(NIM_MODIFY, &m_data); 
	} 

	BOOL BalloonToolTips(LPCTSTR info) 
	{ 
		ModifyToolTips(info); 
		return DispalyToolTips(); 
	} 

	BEGIN_MSG_MAP(CTrayIcon)
		MESSAGE_HANDLER(m_msgTaskbarRestart, OnRestart)  
		MESSAGE_HANDLER(WM_ICON, OnIcon)
		COMMAND_ID_HANDLER(ID_IDR_RESUME, OnResume) 
		COMMAND_ID_HANDLER(ID_IDR_QUIT, OnQuit) 
	END_MSG_MAP() 

	LRESULT OnIcon(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) 
	{ 
		T* pT = static_cast<T*>(this); 
		if (wParam != IDR_MAINFRAME) return 1; 
		switch(lParam) 
		{ 
		case WM_RBUTTONUP: 
			pT->ShowIconMenu(MenuID); 
			break; 
		case WM_LBUTTONUP: 
			pT->ShowWindow(SW_SHOWNORMAL);
			::SetForegroundWindow(pT->m_hWnd);
			break;
		case WM_MOUSEMOVE:									//鼠标悬停在图标上时出现气泡提示
			//DispalyToolTips(); 
			//break; 
		default: 
			break;
		} 
		return 0; 
	} 

	LRESULT OnRestart(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) 
	{ 
		T* pT = static_cast<T*>(this); 
		::SecureZeroMemory(&m_data, sizeof(m_data)); 
		m_data.cbSize = sizeof(m_data); 
		m_data.hWnd = pT->m_hWnd; 
		m_data.uID = IDR_MAINFRAME; 
		::Shell_NotifyIcon(NIM_DELETE, &m_data); 
		CreateTrayIcon(); 
		return 0; 
	} 

	LRESULT OnResume(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) 
	{ 
		T* pT = static_cast<T*>(this); 
		pT->ShowWindow(SW_SHOWNORMAL); 
		::SetForegroundWindow(pT->m_hWnd);
		return 0; 
	} 
	LRESULT OnQuit(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) 
	{ 
		T* pT = static_cast<T*>(this); 
		pT->CloseDialog(wID);
		return 0; 
	} 
}; 