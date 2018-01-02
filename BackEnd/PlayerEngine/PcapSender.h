#ifndef PCAPSENDER_H
#define PCAPSENDER_H

#include <boost/function.hpp>

#include "pcap.h"

class Config;

class PcapSender{
public:
    PcapSender();
    ~PcapSender();

    void InitComponent(boost::function<void (pcap_pkthdr*, const unsigned char*)> f,
                       Config* pConfig);
    void SendPacket(pcap_pkthdr* pHeader, const unsigned char* pData);
    void SetAdapter();
private:
private:
    Config* m_pConfig;
    pcap_t* m_pAdapter;
    boost::function<void (pcap_pkthdr*, const unsigned char*)> m_fOutputCallback;
};

#endif
