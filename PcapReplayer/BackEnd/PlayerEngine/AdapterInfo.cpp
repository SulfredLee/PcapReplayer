#include "AdapterInfo.h"

#include "Logger.h"
#include "pcap.h"

#ifdef WINDOWS
#include "Packet32.h"
//#include <ntddndis.h>
#else
#include "sys/socket.h"
#include "sys/types.h"
#include <netinet/in.h> // for sockaddr_in
#include <arpa/inet.h> // for inet_ntop()
#endif

AdapterInfo::AdapterInfo(){
    LOGMSG_CLASS_NAME("AdapterInfo");
    LOGMSG_MSG_S() << "IN" << std::endl;
    char errbuff[PCAP_ERRBUF_SIZE];
    m_vecAdapterName.clear();
    m_vecInterfaceInfo.clear();
    m_vecIP.clear();

    pcap_if_t *pAlldevs;
    /* Retrieve the device list on the local machine */
    if (pcap_findalldevs(&pAlldevs, errbuff) == -1){
        LOGMSG_ERR_S() << "Error in pcap_findalldevs: " << std::string(errbuff) << std::endl;
    }

    m_bHasAdapter = false;
    for (pcap_if_t* pD = pAlldevs; pD; pD = pD->next){
        m_bHasAdapter = true;
        for (pcap_addr_t *pA = pD->addresses; pA != NULL; pA = pA->next){
            if (pA->addr->sa_family == AF_INET){
                std::string line;
                std::string interfaceIP;
                //interfaceIP = inet_ntoa(((struct sockaddr_in*)pA->addr)->sin_addr);
                char tempInterfaceIP[1024];
                inet_ntop(pA->addr->sa_family,
                          &((struct sockaddr_in*)pA->addr)->sin_addr,
                          tempInterfaceIP,
                          sizeof(tempInterfaceIP));
                interfaceIP = tempInterfaceIP;
                line += interfaceIP;
                line += " ";

                line += pD->name;
                line += " ";
                if (pD->description)
                    line += pD->description;
                else
                    line += "(No description available)";

                m_vecAdapterName.push_back(pD->name);
                m_vecInterfaceInfo.push_back(line);
                m_vecIP.push_back(interfaceIP);
#ifdef WINDOWS
                //m_vecMacAddress.push_back(GetMacAddress(m_vecAdapterName.back()));
#endif
            }
        }
    }
    LOGMSG_MSG_S() << "OUT" << std::endl;
}

AdapterInfo::~AdapterInfo(){

}

std::vector<std::string> AdapterInfo::GetAdapterName(){
    return m_vecAdapterName;
}

std::vector<std::string> AdapterInfo::GetInterfaceInfo(){
    return m_vecInterfaceInfo;
}

std::vector<std::string> AdapterInfo::GetIP(){
    return m_vecIP;
}

#ifdef WINDOWS
//std::string AdapterInfo::GetMacAddress(const std::string& strAdapterName){
//    LPADAPTER	lpAdapter = 0;
//	DWORD		dwErrorCode;
//	PPACKET_OID_DATA  OidData;
//	BOOLEAN		Status;
//	std::string strMacAddress;
//
//	//
//	// Open the selected adapter
//	//
//
//	lpAdapter = PacketOpenAdapter((char*)(strAdapterName.c_str()));
//
//	if (!lpAdapter || (lpAdapter->hFile == INVALID_HANDLE_VALUE))
//	{
//		dwErrorCode = GetLastError();
//		LOGMSG_ERR_S() << "Unable to open the adapter, Error Code : " << std::to_string(dwErrorCode) << std::endl;
//
//		return std::string();
//	}
//
//	//
//	// Allocate a buffer to get the MAC adress
//	//
//
//	OidData = (PPACKET_OID_DATA)malloc(6 + sizeof(PACKET_OID_DATA));
//	if (OidData == NULL)
//	{
//		LOGMSG_ERR_S() << "error allocating memory!\n";
//		PacketCloseAdapter(lpAdapter);
//		return std::string();
//	}
//
//	//
//	// Retrieve the adapter MAC querying the NIC driver
//	//
//
//	OidData->Oid = OID_802_3_CURRENT_ADDRESS;
//
//	OidData->Length = 6;
//	ZeroMemory(OidData->Data, 6);
//
//	Status = PacketRequest(lpAdapter, FALSE, OidData);
//	if (Status)
//	{
//		// The MAC address of the adapter is
//		std::stringstream ss;
//		ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)((OidData->Data)[0]);
//		ss << ":";
//		ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)((OidData->Data)[1]);
//		ss << ":";
//		ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)((OidData->Data)[2]);
//		ss << ":";
//		ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)((OidData->Data)[3]);
//		ss << ":";
//		ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)((OidData->Data)[4]);
//		ss << ":";
//		ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)((OidData->Data)[5]);
//		strMacAddress = ss.str();
//	}
//	else
//	{
//		LOGMSG_ERR_S() << "error retrieving the MAC address of the adapter!\n";
//	}
//
//	free(OidData);
//	PacketCloseAdapter(lpAdapter);
//
//	return strMacAddress;
//}
#endif
