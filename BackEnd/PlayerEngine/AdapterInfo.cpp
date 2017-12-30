#include "AdapterInfo.h"

#include "LogMgr.h"

AdapterInfo::AdapterInfo(){
    char errbuff[PCAP_ERRBUF_SIZE];
    m_vecAdapterName.clear();
    m_vecInterfaceInfo.clear();
    m_vecIP.clear();

    pcap_if_t *pAlldevs;
    /* Retrieve the device list on the local machine */
    if (pcap_findalldevs(&pAlldevs, errbuff) == -1){
        std::string strTemp = "Error in pcap_findalldevs: " + std::string(errbuff);
        LOGMSG_ERROR(strTemp);
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
                m_vecMacAddress.push_back(GetMacAddress(m_vecAdapterName.back()));
            }
        }
    }
}

AdapterInfo::~AdapterInfo(){
    
}
