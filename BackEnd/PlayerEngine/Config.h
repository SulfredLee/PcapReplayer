#ifndef CONFIG_H
#define DONFIG_H

#include <vector>
#include <string>
#include <sstream>

#include "Common.h"

class Config{
public:
    Config();
    ~Config();

    void AddPcapFiles(const std::vector<std::string>& PcapFiles);
    void RemovePcapFile(const int& nIndex);
    void RemoveAllPcapFile();

    void SetLatestFilePath(const std::string& INStr);
    void SetPlayerStatus(const PlayerStatus& INStatus);
    std::string GetLatestFilePath();
    PlayerStatus GetPlayerStatus();
private:
    std::vector<std::string> m_PcapFiles;
    std::string m_strLatestFilePath;
    PlayerStatus m_PlayerStatus;
};
#endif
