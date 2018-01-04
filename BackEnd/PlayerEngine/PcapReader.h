#ifndef PCAPREADER_H
#define PCAPREADER_H

#include <string>
#include <sstream>
#include <map>

#include <boost/function.hpp>

#include "pcap.h"

class Config;

class PcapReader{
public:
    PcapReader();
    ~PcapReader();

    void InitComponent(boost::function<void (pcap_pkthdr*, const unsigned char*, int)> f
                       , Config* pConfig);
    void ReadFile(const std::string& strPcapFile);
    void Reset();
private:
    boost::function<void (pcap_pkthdr*, const unsigned char*, int)> m_fOutputCallback;
    std::string getDstIPFromPcapPkt(const u_char *& data);
    std::string getSrcIPFromPcapPkt(const u_char *& data);
    void UpdateDstIP(u_char *& data, std::string strDstIP);
    void UpdateScrIP(u_char *& data, std::string strScrIP);
    void ReCalculateCheckSum_IPHeader(u_char* pData);
    void ReCalculateCheckSum_UDP_Pkt(u_char* pData, unsigned int unDataLen);
    std::vector<std::string> string_split(const std::string& s, const char& delim);
#ifndef WINDOWS
    unsigned short _byteswap_ushort(const unsigned short& usIN);
#endif

private:
    Config* m_pConfig;
    std::string m_strPcapFile;
    std::map<std::string, std::string> m_mapDstIP;
    std::map<std::string, std::string> m_mapSrcIP;
};

#endif
