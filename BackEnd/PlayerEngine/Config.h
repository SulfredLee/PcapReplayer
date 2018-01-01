#ifndef CONFIG_H
#define DONFIG_H

#include <vector>
#include <string>
#include <sstream>

#include "Common.h"
#include "AdapterInfo.h"

class Config{
public:
    Config();
    ~Config();

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
private:
    std::vector<std::string> m_PcapFiles;
    std::string m_strLatestFilePath;
    PlayerStatus m_PlayerStatus;
    AdapterInfo m_AdapterInfo;
    int m_nAdapterIdx; // indicating user selected adapter index
    double m_dSpeedFactor;
};
#endif
