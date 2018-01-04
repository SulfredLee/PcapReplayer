#include "Config.h"
#include "LogMgr.h"

Config::Config(){
    LOGMSG_INFO("IN");
    m_PlayerStatus = PlayerStatus::Stop;
    m_nAdapterIdx = 0;
    m_dSpeedFactor = 1;
    LOGMSG_INFO("OUT");
}

Config::~Config(){
    
}

void Config::AddPcapFiles(const std::vector<std::string>& PcapFiles){
    boost::lock_guard<boost::mutex> lock(m_MuData);
    LOGMSG_INFO("IN");
    m_PcapFiles.insert(m_PcapFiles.end(), PcapFiles.begin(), PcapFiles.end());
    LOGMSG_INFO("OUT");
}

void Config::RemovePcapFile(const int& nIndex){
    boost::lock_guard<boost::mutex> lock(m_MuData);
    LOGMSG_INFO("IN");
    m_PcapFiles.erase(m_PcapFiles.begin() + nIndex);
    LOGMSG_INFO("OUT");
}

void Config::RemoveAllPcapFile(){
    boost::lock_guard<boost::mutex> lock(m_MuData);
    LOGMSG_INFO("IN");
    m_PcapFiles.clear();
    LOGMSG_INFO("OUT");
}

std::vector<std::string> Config::GetPcapFiles(){
    boost::lock_guard<boost::mutex> lock(m_MuData);
    return m_PcapFiles;
}

void Config::SetLatestFilePath(const std::string& INStr){
    boost::lock_guard<boost::mutex> lock(m_MuData);
    LOGMSG_INFO("IN");
    m_strLatestFilePath = INStr;
    LOGMSG_INFO("OUT");
}

void Config::SetPlayerStatus(const PlayerStatus& INStatus){
    boost::lock_guard<boost::mutex> lock(m_MuData);
    LOGMSG_INFO("IN");
    m_PlayerStatus = INStatus;

    std::stringstream ssLog;
    ssLog << (int)m_PlayerStatus;
    LOGMSG_INFO(ssLog.str());

    LOGMSG_INFO("OUT");
}

std::string Config::GetLatestFilePath(){
    boost::lock_guard<boost::mutex> lock(m_MuData);
    LOGMSG_INFO("IN");
    LOGMSG_INFO("OUT");
    return m_strLatestFilePath;
}

PlayerStatus Config::GetPlayerStatus(){
    boost::lock_guard<boost::mutex> lock(m_MuData);
    return m_PlayerStatus;
}

std::vector<std::string> Config::GetAdapterName(){
    boost::lock_guard<boost::mutex> lock(m_MuData);
    return m_AdapterInfo.GetAdapterName();
}

std::vector<std::string> Config::GetInterfaceInfo(){
    boost::lock_guard<boost::mutex> lock(m_MuData);
    return m_AdapterInfo.GetInterfaceInfo();
}

std::vector<std::string> Config::GetIP(){
    boost::lock_guard<boost::mutex> lock(m_MuData);
    return m_AdapterInfo.GetIP();
}

int Config::GetAdapterIdx(){
    boost::lock_guard<boost::mutex> lock(m_MuData);
    return m_nAdapterIdx;
}

void Config::SetAdapterIdx(const int& nSelect){
    boost::lock_guard<boost::mutex> lock(m_MuData);
    m_nAdapterIdx = nSelect;
}

void Config::SetSpeedFactor(const double& dSpeedFactor){
    boost::lock_guard<boost::mutex> lock(m_MuData);
    m_dSpeedFactor = dSpeedFactor;
    std::stringstream ssTempLine;
    ssTempLine << m_dSpeedFactor;
    LOGMSG_INFO(ssTempLine.str());
}

double Config::GetSpeedFactor(){
    boost::lock_guard<boost::mutex> lock(m_MuData);
    return m_dSpeedFactor;
}

void Config::SetSpeedLimit(const double& dSpeedLimit){
    boost::lock_guard<boost::mutex> lock(m_MuData);
    m_dSpeedLimit = dSpeedLimit;
}

double Config::GetSpeedLimit(){
    boost::lock_guard<boost::mutex> lock(m_MuData);
    return m_dSpeedLimit;
}

void Config::SetLoopCount(const int& nLoopCount){
    boost::lock_guard<boost::mutex> lock(m_MuData);
    m_nLoopCount = nLoopCount;
}

int Config::GetLoopCount(){
    boost::lock_guard<boost::mutex> lock(m_MuData);
    return m_nLoopCount;
}

void Config::SetMapDstIP(const std::map<std::string, std::string>& inMap){
    boost::lock_guard<boost::mutex> lock(m_MuData);
    m_mapDstIP = inMap;
}

void Config::SetMapSrcIP(const std::map<std::string, std::string>& inMap){
    boost::lock_guard<boost::mutex> lock(m_MuData);
    m_mapSrcIP = inMap;
}

std::map<std::string, std::string> Config::GetMapDstIP(){
    boost::lock_guard<boost::mutex> lock(m_MuData);
    return m_mapDstIP;
}

std::map<std::string, std::string> Config::GetMapSrcIP(){
    boost::lock_guard<boost::mutex> lock(m_MuData);
    //    boost::mutex::scoped_lock lock(m_MuData);
    return m_mapSrcIP;
}
