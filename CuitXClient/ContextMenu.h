//ContextMenu.h 

 #pragma once 

 template <class T> 
 class CContextMenu 
 { 
 public: 
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
}; 