#include "PcapSender.h"
#include "Config.h"
#include "Logger.h"

#include <string>
#include <sstream>

PcapSender::PcapSender(){
    LOGMSG_CLASS_NAME("PcapSender");
    m_pAdapter = nullptr;
}

PcapSender::~PcapSender(){
	if (m_pAdapter != nullptr){
		pcap_close(m_pAdapter);
		m_pAdapter = nullptr;
	}
}

void PcapSender::InitComponent(boost::function<void (pcap_pkthdr*, const unsigned char*)> f,
                               Config* pConfig){
    m_fOutputCallback = f;
    m_pConfig = pConfig;

    SetAdapter();
}

void PcapSender::SendPacket(pcap_pkthdr* pHeader, const unsigned char* pData){
    if (m_pAdapter == nullptr){
        LOGMSG_ERR_S() << "Error Adapter is empty" << std::endl;
        return;
    }
    if (pcap_sendpacket(m_pAdapter,
                        pData,
                        pHeader->len) != 0){
        LOGMSG_ERR_S() << "Error sending the packet: " << pcap_geterr(m_pAdapter) << std::endl;
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
        LOGMSG_ERR_S() << "Unable to open the adapter. " << strAdapterName << " is not supported by WinPcap" << std::endl;
        LOGMSG_ERR_S() << errbuff << std::endl;
    }
}
