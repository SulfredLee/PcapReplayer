#include "PcapReader.h"
#include "LogMgr.h"
#include "Common.h"

#include <boost/filesystem.hpp>

PcapReader::PcapReader(){
}

PcapReader::~PcapReader(){
    
}

void PcapReader::InitComponent(boost::function<void (pcap_pkthdr*, const unsigned char*, int)> f){
    m_fOutputCallback = f;
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
