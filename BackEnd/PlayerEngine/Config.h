#ifndef CONFIG_H
#define DONFIG_H

#include <boost/thread/mutex.hpp>
#include <boost/date_time.hpp>

#include <vector>
#include <string>
#include <sstream>
#include <map>

#include "Common.h"
#include "AdapterInfo.h"

class Config{
public:
    Config();
    ~Config();

    Config(const Config& that) = delete;

    // handle PcapFiles
    void AddPcapFiles(const std::vector<std::string>& PcapFiles);
    void RemovePcapFile(const int& nIndex);
    void RemoveAllPcapFile();
    std::vector<std::string> GetPcapFiles();

    void SetLatestFilePath(const std::string& INStr);
    void SetPlayerStatus(const PlayerStatus& INStatus);
    std::string GetLatestFilePath();
    PlayerStatus GetPlayerStatus();

    // handle AdapterInfo
    std::vector<std::string> GetAdapterName();
    std::vector<std::string> GetInterfaceInfo();
    std::vector<std::string> GetIP();
    int GetAdapterIdx();
    void SetAdapterIdx(const int& nSelect);

    // handle Speed
    void SetSpeedFactor(const double& dSpeedFactor);
    double GetSpeedFactor();
    void SetSpeedLimit(const double& dSpeedLimit);
    double GetSpeedLimit();

    // handle Loop
    void SetLoopCount(const int& nLoopCount);
    int GetLoopCount();

    // handle SrcIP DstIP mapping
    void SetMapDstIP(const std::map<std::string, std::string>& inMap);
    void SetMapSrcIP(const std::map<std::string, std::string>& inMap);
    std::map<std::string, std::string> GetMapDstIP();
    std::map<std::string, std::string> GetMapSrcIP();

    // handle Scheduler
    void SetSchedulerEnable(const bool& bState);
    bool GetSchedulerEnable();
    void SetOneTimeOnly(const bool& bIN);
    bool GetOneTimeOnly();
    void SetSchedulerDay(const bool& bSun
                         , const bool& bMon
                         , const bool& bTue
                         , const bool& bWed
                         , const bool& bThru
                         , const bool& bFri
                         , const bool& bSat);
    std::vector<bool> GetSchedulerDay();
    void SetDateTime(const boost::posix_time::ptime& DateTime);
    boost::posix_time::ptime GetDateTime();

    // handle config path
    void SetConfigPath(const std::string& strPath);
    std::string GetConfigPath();
private:
    std::vector<std::string> m_PcapFiles;
    std::string m_strLatestFilePath;
    PlayerStatus m_PlayerStatus;
    AdapterInfo m_AdapterInfo;
    int m_nAdapterIdx; // indicating user selected adapter index
    int m_nLoopCount;
    double m_dSpeedFactor;
    double m_dSpeedLimit; // MBit/s
    std::map<std::string, std::string> m_mapDstIP;
    std::map<std::string, std::string> m_mapSrcIP;
    boost::mutex m_MuData;
    // handle Scheduler
    bool m_bSchedulerEnable;
    bool m_bOneTimeOnly;
    std::vector<bool> m_vecSchedulerDay;
    boost::posix_time::ptime m_DateTime;
    // handle config path
    std::string m_strDefaultConfigPath;
    std::string m_strLatestConfigPath;
};
#endif
