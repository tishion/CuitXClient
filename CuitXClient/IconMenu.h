#pragma once 
#include "stdafx.h" 
#include "resource.h"
//#include "ContextMenu.h"

template <class T> 
class CIconMenu //: public CContextMenu<T> 
{ 
public: 
	BEGIN_MSG_MAP(CIconMenu) 
		COMMAND_ID_HANDLER(ID_IDR_RESUME, OnResume) 
		COMMAND_ID_HANDLER(ID_IDR_QUIT, OnQuit) 
	END_MSG_MAP() 

	BOOL CreateContextMenu(UINT ID_Menu) 
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

	LRESULT OnResume(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) 
	{ 
		T* pT = static_cast<T*>(this); 
		pT->ShowWindow(SW_SHOW); 
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