#ifndef PCAPREADER_H
#define PCAPREADER_H

#include <string>
#include <sstream>

#include <boost/function.hpp>

#include "pcap.h"

class PcapReader{
public:
    PcapReader();
    ~PcapReader();

    void InitComponent(boost::function<void (pcap_pkthdr*, const unsigned char*, int)> f);
    void ReadFile(const std::string& strPcapFile);
private:
    std::string m_strPcapFile;
    boost::function<void (pcap_pkthdr*, const unsigned char*, int)> m_fOutputCallback;
};

#endif
