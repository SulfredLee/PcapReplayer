#include "PcapSender.h"
#include "Config.h"
#include "LogMgr.h"

#include <string>
#include <sstream>

PcapSender::PcapSender(){
    m_pAdapter = nullptr;
}

PcapSender::~PcapSender(){
    
}

void PcapSender::InitComponent(boost::function<void (pcap_pkthdr*, const unsigned char*)> f,
                               Config* pConfig){
    m_fOutputCallback = f;
    m_pConfig = pConfig;

    SetAdapter();
}

void PcapSender::SendPacket(pcap_pkthdr* pHeader, const unsigned char* pData){
    if (m_pAdapter == nullptr){
        LOGMSG_ERROR("Error Adapter is empty");
        return;
    }
    if (pcap_sendpacket(m_pAdapter,
                        pData,
                        pHeader->len) != 0){
        std::ostringstream ssTempLine;
        ssTempLine << "Error sending the packet: " << pcap_geterr(m_pAdapter);
        LOGMSG_ERROR(ssTempLine.str());
    }
    m_fOutputCallback(pHeader, pData);
}

void PcapSender::SetAdapter(){
    if (m_pAdapter != nullptr){
        pcap_close(m_pAdapter);
        m_pAdapter = nullptr;
    }
    char errbuff[PCAP_ERRBUF_SIZE];
    /* Open the adapter */
    std::string strAdapterName = m_pConfig->GetAdapterName()[m_pConfig->GetAdapterIdx()];
    if ((m_pAdapter = pcap_open_live(strAdapterName.c_str(),		// name of the device
                                     65536,			// portion of the packet to capture. It doesn't matter in this case 
                                     1,				// promiscuous mode (nonzero means promiscuous)
                                     1000,			// read timeout
                                     errbuff			// error buffer
                                     )) == NULL){
        std::ostringstream ssTempLine;
        ssTempLine << "Unable to open the adapter. " << strAdapterName << " is not supported by WinPcap";
        LOGMSG_ERROR(ssTempLine.str());

        ssTempLine.str(std::string());
        ssTempLine << errbuff;
        LOGMSG_ERROR(ssTempLine.str());
    }
}
