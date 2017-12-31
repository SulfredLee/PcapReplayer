#include "Config.h"
#include "LogMgr.h"

Config::Config(){
    LOGMSG_INFO("IN");
    m_PlayerStatus = PlayerStatus::Stop;
    m_nAdapterIdx = 0;
    LOGMSG_INFO("OUT");
}

Config::~Config(){
    
}

void Config::AddPcapFiles(const std::vector<std::string>& PcapFiles){
    LOGMSG_INFO("IN");
    m_PcapFiles.insert(m_PcapFiles.end(), PcapFiles.begin(), PcapFiles.end());
    LOGMSG_INFO("OUT");
}

void Config::RemovePcapFile(const int& nIndex){
    LOGMSG_INFO("IN");
    m_PcapFiles.erase(m_PcapFiles.begin() + nIndex);
    LOGMSG_INFO("OUT");
}

void Config::RemoveAllPcapFile(){
    LOGMSG_INFO("IN");
    m_PcapFiles.clear();
    LOGMSG_INFO("OUT");
}

void Config::SetLatestFilePath(const std::string& INStr){
    LOGMSG_INFO("IN");
    m_strLatestFilePath = INStr;
    LOGMSG_INFO("OUT");
}

void Config::SetPlayerStatus(const PlayerStatus& INStatus){
    LOGMSG_INFO("IN");
    m_PlayerStatus = INStatus;

    std::stringstream ssLog;
    ssLog << (int)m_PlayerStatus;
    LOGMSG_INFO(ssLog.str());

    LOGMSG_INFO("OUT");
}

std::string Config::GetLatestFilePath(){
    LOGMSG_INFO("IN");
    LOGMSG_INFO("OUT");
    return m_strLatestFilePath;
}

PlayerStatus Config::GetPlayerStatus(){
    LOGMSG_INFO("IN");

    std::stringstream ssLog;
    ssLog << (int)m_PlayerStatus;
    LOGMSG_INFO(ssLog.str());

    LOGMSG_INFO("OUT");
    return m_PlayerStatus;
}

std::vector<std::string> Config::GetAdapterName(){
    return m_AdapterInfo.GetAdapterName();
}

std::vector<std::string> Config::GetInterfaceInfo(){
    return m_AdapterInfo.GetInterfaceInfo();
}

std::vector<std::string> Config::GetIP(){
    return m_AdapterInfo.GetIP();
}

int Config::GetAdapterIdx(){
    return m_nAdapterIdx;
}

void Config::SetAdapterIdx(const int& nSelect){
    m_nAdapterIdx = nSelect;
}
