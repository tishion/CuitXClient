// aboutdlg.cpp : implementation of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"

LRESULT CAboutDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());
	CString declare("责任声明：\r\n"
		"1.本软件作品仅为个人学习ATL&WTL编程技术之产物，如有侵犯任何个人或团体利益，请与本人联系。\r\n"
		"2.本人对由于使用本软件可能造成的任何损坏和损失不负任何责任。\r\n"
		"3.自您使用本软件之时起，表示您自愿接受此声明之所有条目。");
	SetDlgItemText(IDC_ABOUT_EDIT, declare);
	return TRUE;
}

LRESULT CAboutDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}


LRESULT CAboutDlg::OnNMClickSyslinkMail(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	::ShellExecute(NULL, NULL, ((PNMLINK)pNMHDR)->item.szUrl, NULL, NULL, SW_SHOWNORMAL);
	return 0;
}
