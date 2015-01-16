#pragma once
class CMainDlg;

class xClient
{
public: 	
	xClient(CMainDlg *hDlg);
	~xClient();
	
	CMainDlg *m_hDlg;
	HANDLE m_hThread;
	HANDLE m_hStopEvent;
	PXCONDATA pxcondata;
	void Connect(PXCONDATA pcd);
	void DisConnect();

public:
	pcap_t * hPcap;

	UCHAR ethhdr[14];

	bool OpenAdapter();
	void CloseAdapter();

	void SendStartPkt();
	void SendLogoffPkt();
	void SendResponseIdentity(const UCHAR request[]);
	void SendResponseMD5(const UCHAR request[]);
	void SendResponseNotification(const UCHAR request[]);
	
	void MakeMD5(UCHAR digest[], UCHAR id, const char passwd[], const UCHAR srcMD5[]);
	void MakeClientVersion(UCHAR area[]);
	void MakeBase64(char area[]);
	void XOR(UCHAR data[], unsigned dlen, const char key[], unsigned klen);

	friend DWORD WINAPI WorkProc(LPVOID lpParam);
};