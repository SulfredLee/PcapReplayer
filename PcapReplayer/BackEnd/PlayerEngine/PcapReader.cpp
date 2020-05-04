#include "PcapReader.h"
#include "LogMgr.h"
#include "Common.h"
#include "Config.h"

#include <boost/filesystem.hpp>

PcapReader::PcapReader(){
}

PcapReader::~PcapReader(){
    
}

void PcapReader::InitComponent(boost::function<void (pcap_pkthdr*, const unsigned char*, int)> f
                               , Config* pConfig){
    m_fOutputCallback = f;
    m_pConfig = pConfig;

    Reset();
}

void PcapReader::ReadFile(const std::string& strPcapFile){
    char errbuff[PCAP_ERRBUF_SIZE];
    pcap_t * pcap = pcap_open_offline(strPcapFile.c_str(), errbuff);
    std::stringstream ssTempLine;
    ssTempLine << "Reading: " << strPcapFile;
    LOGMSG_INFO(ssTempLine.str());
    if (pcap == NULL){
        ssTempLine.str(std::string());
        ssTempLine << "Error: " << errbuff;
        LOGMSG_ERROR(ssTempLine.str());
        return;
    }

    struct pcap_pkthdr *header;
    const u_char *data;
    uint64_t un64PacketCount = 1;
    uint64_t un64SentPacketSize = 0;
    boost::uintmax_t un64CurFileSize = boost::filesystem::file_size(strPcapFile.c_str());
    int nProgress = 0;

    while (int returnValue = pcap_next_ex(pcap, &header, &data) >= 0){
        long m_packetArrivalSec = header->ts.tv_sec;
        long m_packetArrivalmicroseconds = header->ts.tv_usec;
        double arrivalTime = m_packetArrivalSec + m_packetArrivalmicroseconds*0.000001;

        // handle IP mapping
        u_char* pMyData = new u_char[header->len];
        memcpy(pMyData, data, header->len);
        std::string strDstIP = getDstIPFromPcapPkt(data);
        std::string strSrcIP = getSrcIPFromPcapPkt(data);
        auto DstIT = m_mapDstIP.find(strDstIP);
        auto SrcIT = m_mapSrcIP.find(strSrcIP);
        if (DstIT != m_mapDstIP.end()) {
            UpdateDstIP(pMyData, DstIT->second);
        }
        if (SrcIT != m_mapSrcIP.end()) {
            UpdateScrIP(pMyData, SrcIT->second);
        }
        // handle checksum
        if (DstIT != m_mapDstIP.end()
            || SrcIT != m_mapSrcIP.end()){
            ReCalculateCheckSum_IPHeader(pMyData);
            ReCalculateCheckSum_UDP_Pkt(pMyData, header->len);
        }
		if (pMyData != nullptr){
			delete pMyData;
		}
        // handle progressBar
        un64SentPacketSize += header->len + PCAPLOCALHEADERSIZE;
        float fProgress = (float)un64SentPacketSize / (float)un64CurFileSize;
        if (nProgress != (int)(fProgress * 100)){
            ++nProgress;
        }

        // handle output data
        m_fOutputCallback(header, data, nProgress);

        un64PacketCount++;
    }
    ssTempLine.str(std::string());
    ssTempLine << "Finished reading: " << strPcapFile;
    LOGMSG_INFO(ssTempLine.str());
}

void PcapReader::Reset(){
    m_mapDstIP = m_pConfig->GetMapDstIP();
    m_mapSrcIP = m_pConfig->GetMapSrcIP();
}

std::string PcapReader::getDstIPFromPcapPkt(const u_char *& data){
    return std::to_string((unsigned long long)data[30]) + "."
        + std::to_string((unsigned long long)data[31]) + "."
        + std::to_string((unsigned long long)data[32]) + "."
        + std::to_string((unsigned long long)data[33]);
}

std::string PcapReader::getSrcIPFromPcapPkt(const u_char *& data){
    return std::to_string((unsigned long long)data[26]) + "."
        + std::to_string((unsigned long long)data[27]) + "."
        + std::to_string((unsigned long long)data[28]) + "."
        + std::to_string((unsigned long long)data[29]);
}

void PcapReader::UpdateDstIP(u_char *& data, std::string strDstIP){
    std::vector<std::string> parts = string_split(strDstIP, '.');
    data[30] = (u_char)atoi(parts[0].c_str());
    data[31] = (u_char)atoi(parts[1].c_str());
    data[32] = (u_char)atoi(parts[2].c_str());
    data[33] = (u_char)atoi(parts[3].c_str());
}

void PcapReader::UpdateScrIP(u_char *& data, std::string strScrIP){
    std::vector<std::string> parts = string_split(strScrIP, '.');
    data[26] = (u_char)atoi(parts[0].c_str());
    data[27] = (u_char)atoi(parts[1].c_str());
    data[28] = (u_char)atoi(parts[2].c_str());
    data[29] = (u_char)atoi(parts[3].c_str());
}

void PcapReader::ReCalculateCheckSum_IPHeader(u_char* pData){
    // reset checksum
    pData[24] = 0;
    pData[25] = 0;

    uint32_t unChecksum = 0;
    const uint16_t* pD16;
    pD16 = reinterpret_cast<const uint16_t*>(pData + 14);

    // adding checksum for scr IP and dst IP
    for (int i = 0; i < 10; i++){
        unChecksum += _byteswap_ushort(*pD16++);
    }
    while (unChecksum >> 16){
        unChecksum = (unChecksum & 0xffff) + (unChecksum >> 16);
    }

    // set checksum back to data
    uint16_t un16TempChecksum = static_cast<uint16_t>(~unChecksum);
    un16TempChecksum = _byteswap_ushort(un16TempChecksum);
    memcpy((char*)(pData + 24), &un16TempChecksum, sizeof(uint16_t));
}

void PcapReader::ReCalculateCheckSum_UDP_Pkt(u_char* pData, unsigned int unDataLen){
    // reset checksum
    pData[40] = 0;
    pData[41] = 0;

    uint32_t unChecksum = 0;
    const uint16_t* pD16;
    const uint8_t* pD8;
    // handle IP layer
    //		handle src IP
    pD16 = reinterpret_cast<const uint16_t*>(pData + 26);
    unChecksum += _byteswap_ushort(*pD16++); // since wireshark file is big-endian, but c++ in PC is little-endian
    unChecksum += _byteswap_ushort(*pD16);
    //		handle dst IP
    pD16 = reinterpret_cast<const uint16_t*>(pData + 30);
    unChecksum += _byteswap_ushort(*pD16++); // since wireshark file is big-endian, but c++ in PC is little-endian
    unChecksum += _byteswap_ushort(*pD16);
    //		handle portocol
    pD8 = reinterpret_cast<const uint8_t*>(pData + 23);
    unChecksum += *pD8;
    //		handle data lenght, from IP layer to udp data layer
    pD16 = reinterpret_cast<const uint16_t*>(pData + 38);
    unChecksum += _byteswap_ushort(*pD16);


    // handle UDP layer
    //		handle src Port
    pD16 = reinterpret_cast<const uint16_t*>(pData + 34);
    unChecksum += _byteswap_ushort(*pD16);
    //		handle dst Port
    pD16 = reinterpret_cast<const uint16_t*>(pData + 36);
    unChecksum += _byteswap_ushort(*pD16);	
    //		handle data lenght, from IP layer to udp data layer
    pD16 = reinterpret_cast<const uint16_t*>(pData + 38);
    unChecksum += _byteswap_ushort(*pD16);

    // handle udp data
    size_t len = unDataLen - DATAGRAME_DATA_OFFSET; // 42 is the Ethernet header
    pD16 = reinterpret_cast<const uint16_t*>(pData + DATAGRAME_DATA_OFFSET);
    while (len > 1){
        unChecksum += _byteswap_ushort(*pD16++); // since wireshark file is big-endian, but c++ in PC is little-endian
        len -= sizeof(uint16_t);
    }
    if (len){ // if total lenght the data is odd
        unChecksum += *reinterpret_cast<const uint8_t*>(pD16);
    }


    while (unChecksum >> 16){
        unChecksum = (unChecksum & 0xffff) + (unChecksum >> 16);
    }

    // set checksum back to data
    uint16_t un16TempChecksum = static_cast<uint16_t>(~unChecksum);
    un16TempChecksum = _byteswap_ushort(un16TempChecksum);
    memcpy((char*)(pData + 40), &un16TempChecksum, sizeof(uint16_t));
}

std::vector<std::string> PcapReader::string_split(const std::string& s, const char& delim){
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;

}

#ifndef WINDOWS
unsigned short PcapReader::_byteswap_ushort(const unsigned short& usIN){
    uint16_t hi = (usIN & 0xff00);
    uint16_t lo = (usIN & 0xff);
    return (lo << 8) | (hi >> 8);
}
#endif
