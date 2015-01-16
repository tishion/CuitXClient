// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once
#pragma comment(lib, "wpcap.lib")
#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "WS2_32.lib")

// Change these values to use different versions
#define WINVER		0x0500
#define _WIN32_WINNT	0x0501
#define _WIN32_IE	0x0501
#define _RICHEDIT_VER	0x0200

#define INPUT_MAX	64
#define VER_MAX 16
#define MAX_ADPTER_NAME_BUFLEN (MAX_ADAPTER_NAME_LENGTH+4)
#define  XCLIENT_CMD (WM_USER+1000)
#define  XC_MESSAGE 0
#define  XC_IPCONFIG 1
#define  XC_DISCONNETC 2
#define XC_NOTIFY 3
#define XC_CONSUCCESS 4

#include <atlstr.h>
#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atltime.h>
#include <atlwin.h>
#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlddx.h>
#include <atlcoll.h>

#define  HAVE_REMOTE
#include <pcap.h>
#include <IPHlpApi.h>

#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

typedef struct __NICDATA
{
	CString name;
	CString description;
	UCHAR mac[6];
	DWORD index;
}NICDATA, * PNICDATA;

typedef struct __XCONDATA  
{
	char account[INPUT_MAX];
	char passwd[INPUT_MAX ];
	char clientver[VER_MAX];
	char nicname[MAX_ADPTER_NAME_BUFLEN];
	char nicmac[6];
	DWORD nicindex;
	DWORD castset;
	DWORD pktcastset;
}XCONDATA, * PXCONDATA;

typedef enum {REQUEST=1, RESPONSE=2, SUCCESS=3, FAILURE=4, H3CDATA=10} EAP_Code;
typedef enum {IDENTITY=1, NOTIFICATION=2, MD5=4, AVAILABLE=20} EAP_Type;
typedef UCHAR EAP_ID;
const UCHAR MulticastAddr[6]  = {0x01,0x80,0xc2,0x00,0x00,0x03}; // 多播MAC地址
const UCHAR BroadcastAddr[6]  = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
const char H3C_KEY[]      ="HuaWei3COM1X";  // H3C的固定密钥