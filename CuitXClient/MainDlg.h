// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "IconMenu.h"
#include "TrayIcon.h"
class xClient;
class CMainDlg : public CDialogImpl<CMainDlg>, 
	//public CUpdateUI<CMainDlg>,
	public CMessageFilter, 
	public CIdleHandler,
	public CWinDataExchange<CMainDlg>,
	public CTrayIcon<CMainDlg, CIconMenu<CMainDlg>, IDR_ICON_MENU>
{
protected:
	CRegKey regKey;
	CString keyname;
	CEdit m_EditAccount;
	CEdit m_EditPasswd;
	CComboBox m_CombNicList;
	CComboBox m_CombClientVer;

	CButton m_ButtonBroadCast;
	CButton m_ButtonMultiCast;

	CButton m_ButtonPktMultiCast;
	CButton m_ButtonPktSingleCast;
	CListBox m_ListBoxLog;

	CString m_szAccount;
	CString m_szPasswd;	
	DWORD m_nCastSet;
	DWORD m_nPktCastSet;
	CString m_szClientVer;
	CString m_szNicName;
	NICDATA m_useNic;

	DWORD m_nKeepPasswd;
	DWORD m_nAutoStart;
	DWORD m_nAutoRun;
	DWORD m_nBubbleNotify;
	
	
	bool m_bExpand;
	UINT m_nNormalWidth;
	UINT m_nFullWidth;

	//control var
	CButton m_ButtonStart;
	CButton m_ButtonOption;

	CSimpleValArray<NICDATA> m_NicList;
	XCONDATA m_xcontdata;
	xClient *xclient;
	bool m_bStarted;

	void setEnableControls(bool en);
	void ReadInitData();
	bool EnumNic();
	void InitNicListCombox();
	void InitVerListCombox();
	void AppandMessage(ATL::_U_STRINGorID message);
	void ShowBalloonToolTips(ATL::_U_STRINGorID message);
	void ToTrayIcon();
	bool Start();

public:
	enum { IDD = IDD_MAINDLG };
	typedef CTrayIcon<CMainDlg, CIconMenu<CMainDlg>, IDR_ICON_MENU> TrayIcon; 

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();
	void CloseDialog(int nVal);

	//BEGIN_UPDATE_UI_MAP(CMainDlg)
	//END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_QUERYENDSESSION, OnQueryEndSession)
		MESSAGE_HANDLER(XCLIENT_CMD, MessageHandler)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_HANDLER(IDC_BUTTON_OPTION, BN_CLICKED, OnBnClickedButtonOption)
		COMMAND_HANDLER(IDC_CHECK_KEEPPASSWD, BN_CLICKED, OnBnClickedCheckKeeppasswd)
		COMMAND_HANDLER(IDC_CHECK_AUTOSTART, BN_CLICKED, OnBnClickedCheckAutostart)
		COMMAND_HANDLER(IDC_CHECK_AUTORUN, BN_CLICKED, OnBnClickedCheckAutorun)
		COMMAND_HANDLER(IDC_CHECK_BUBBLENOTIFY, BN_CLICKED, OnBnClickedCheckBubblenotify)
		COMMAND_HANDLER(IDC_RADIO_CAST, BN_CLICKED, OnBnClickedRadioCast)
		COMMAND_HANDLER(IDC_RADIO_CAST1, BN_CLICKED, OnBnClickedRadioCast)
		COMMAND_HANDLER(IDC_RADIO_PKTCAST, BN_CLICKED, OnBnClickedRadioPktcast)
		COMMAND_HANDLER(IDC_RADIO_PKTCAST1, BN_CLICKED, OnBnClickedRadioPktcast)
		COMMAND_HANDLER(IDC_COMBO_NIC, CBN_DROPDOWN, OnCbnDropdownComboNic)
		COMMAND_HANDLER(IDC_COMBO_NIC, CBN_SELCHANGE, OnCbnSelchangeComboNic)
		COMMAND_HANDLER(IDC_COMBO_NIC, CBN_CLOSEUP, OnCbnCloseUpComboNic)
		COMMAND_HANDLER(IDC_BUTTON_START, BN_CLICKED, OnBnClickedButtonStart)
		CHAIN_MSG_MAP(TrayIcon);
	END_MSG_MAP()

	BEGIN_DDX_MAP(CMainDlg)		
		DDX_TEXT(IDC_EDIT_ACCOUNT, m_szAccount)
		DDX_TEXT(IDC_EDIT_PASSWD, m_szPasswd)
		DDX_CHECK(IDC_CHECK_KEEPPASSWD, (int &)m_nKeepPasswd)
		DDX_CHECK(IDC_CHECK_AUTOSTART, (int &)m_nAutoStart)
		DDX_RADIO(IDC_RADIO_CAST, (int &)m_nCastSet)
		DDX_RADIO(IDC_RADIO_PKTCAST, (int &)m_nPktCastSet)
		DDX_CHECK(IDC_CHECK_AUTORUN, (int &)m_nAutoRun)
		DDX_CHECK(IDC_CHECK_BUBBLENOTIFY, (int &)m_nBubbleNotify)			
	END_DDX_MAP()

// Handler prototypes (uncomment arguments if needed):
	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnQueryEndSession(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	
	LRESULT OnBnClickedButtonOption(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedCheckKeeppasswd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedCheckAutostart(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedCheckAutorun(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedCheckBubblenotify(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedRadioCast(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedRadioPktcast(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCbnDropdownComboNic(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCbnCloseUpComboNic(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCbnSelchangeComboNic(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonStart(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
