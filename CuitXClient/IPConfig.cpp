#include "stdafx.h"
#include "IPConfig.h"


int IPConfig(int flag, int nicindex, char *nicname)
{
	DWORD dwRetVal = 0;
	IP_ADAPTER_INDEX_MAP Adapter;

	Adapter.Index = nicindex;
	swprintf_s(Adapter.Name, MAX_ADAPTER_NAME, TEXT("\\DEVICE\\TCPIP_%s"), CA2T(nicname));
	if (flag == IPCON_RENEW)
	{	
		::IpReleaseAddress(&Adapter);
		if ((dwRetVal =  ::IpRenewAddress(&Adapter)) != NO_ERROR)
		{
			AtlTrace(TEXT("Failed at IpRenewAddress():%d"), dwRetVal);
			return -1;
		}	
	}

	if (flag == IPCON_RELEASE)
	{
		if ((dwRetVal = ::IpReleaseAddress(&Adapter)) != NO_ERROR)
		{
			return -1;
		}
	}

	return 1;
}