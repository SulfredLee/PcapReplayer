#include "Config.h"
#include "Logger.h"

Config::Config()
    : m_DateTime(boost::gregorian::date(1997, 7, 1)
                 , boost::posix_time::time_duration(0, 0, 0)){
    LOGMSG_CLASS_NAME("Config");
    LOGMSG_MSG_S() << "IN" << std::endl;
    m_PlayerStatus = PlayerStatus::Stop;
    m_nAdapterIdx = 0;
    m_dSpeedFactor = 1;
    m_vecSchedulerDay.resize(7, false);
    m_strLatestFilePath = "";
    m_strDefaultConfigPath = "default_config.cfg";
    m_strLatestConfigPath = "";
    m_bSchedulerEnable = false;
    LOGMSG_MSG_S() << "OUT" << std::endl;
}

Config::~Config(){

}

void Config::AddPcapFiles(const std::vector<std::string>& PcapFiles){
    std::lock_guard<std::mutex> lock(m_MuData);
    // boost::lock_guard<boost::mutex> lock(m_MuData);
    LOGMSG_MSG_S() << "IN" << std::endl;
    m_PcapFiles.insert(m_PcapFiles.end(), PcapFiles.begin(), PcapFiles.end());
    LOGMSG_MSG_S() << "OUT" << std::endl;
}

void Config::RemovePcapFile(const int& nIndex){
    std::lock_guard<std::mutex> lock(m_MuData);
    LOGMSG_MSG_S() << "IN" << std::endl;
    m_PcapFiles.erase(m_PcapFiles.begin() + nIndex);
    LOGMSG_MSG_S() << "OUT" << std::endl;
}

void Config::RemoveAllPcapFile(){
    std::lock_guard<std::mutex> lock(m_MuData);
    LOGMSG_MSG_S() << "IN" << std::endl;
    m_PcapFiles.clear();
    LOGMSG_MSG_S() << "OUT" << std::endl;
}

std::vector<std::string> Config::GetPcapFiles(){
    std::lock_guard<std::mutex> lock(m_MuData);
    return m_PcapFiles;
}

void Config::SetLatestFilePath(const std::string& INStr){
    std::lock_guard<std::mutex> lock(m_MuData);
    LOGMSG_MSG_S() << "IN" << std::endl;
    m_strLatestFilePath = INStr;
    LOGMSG_MSG_S() << "OUT" << std::endl;
}

void Config::SetPlayerStatus(const PlayerStatus& INStatus){
    std::lock_guard<std::mutex> lock(m_MuData);
    LOGMSG_MSG_S() << "IN" << std::endl;
    m_PlayerStatus = INStatus;

    LOGMSG_MSG_S() << (int)m_PlayerStatus << std::endl;

    LOGMSG_MSG_S() << "OUT" << std::endl;
}

std::string Config::GetLatestFilePath(){
    std::lock_guard<std::mutex> lock(m_MuData);
    LOGMSG_MSG_S() << "IN" << std::endl;
    LOGMSG_MSG_S() << "OUT" << std::endl;
    return m_strLatestFilePath;
}

PlayerStatus Config::GetPlayerStatus(){
    std::lock_guard<std::mutex> lock(m_MuData);
    return m_PlayerStatus;
}

std::vector<std::string> Config::GetAdapterName(){
    std::lock_guard<std::mutex> lock(m_MuData);
    return m_AdapterInfo.GetAdapterName();
}

std::vector<std::string> Config::GetInterfaceInfo(){
    std::lock_guard<std::mutex> lock(m_MuData);
    return m_AdapterInfo.GetInterfaceInfo();
}

std::vector<std::string> Config::GetIP(){
    std::lock_guard<std::mutex> lock(m_MuData);
    return m_AdapterInfo.GetIP();
}

int Config::GetAdapterIdx(){
    std::lock_guard<std::mutex> lock(m_MuData);
    return m_nAdapterIdx;
}

void Config::SetAdapterIdx(const int& nSelect){
    std::lock_guard<std::mutex> lock(m_MuData);
    m_nAdapterIdx = nSelect;
}

void Config::SetSpeedFactor(const double& dSpeedFactor){
    std::lock_guard<std::mutex> lock(m_MuData);
    m_dSpeedFactor = dSpeedFactor;
    LOGMSG_MSG_S() << m_dSpeedFactor;
}

double Config::GetSpeedFactor(){
    std::lock_guard<std::mutex> lock(m_MuData);
    return m_dSpeedFactor;
}

void Config::SetSpeedLimit(const double& dSpeedLimit){
    std::lock_guard<std::mutex> lock(m_MuData);
    m_dSpeedLimit = dSpeedLimit;
}

double Config::GetSpeedLimit(){
    std::lock_guard<std::mutex> lock(m_MuData);
    return m_dSpeedLimit;
}

void Config::SetLoopCount(const int& nLoopCount){
    std::lock_guard<std::mutex> lock(m_MuData);
    m_nLoopCount = nLoopCount;
}

int Config::GetLoopCount(){
    std::lock_guard<std::mutex> lock(m_MuData);
    return m_nLoopCount;
}

void Config::SetMapDstIP(const std::map<std::string, std::string>& inMap){
    std::lock_guard<std::mutex> lock(m_MuData);
    m_mapDstIP = inMap;
}

void Config::SetMapSrcIP(const std::map<std::string, std::string>& inMap){
    std::lock_guard<std::mutex> lock(m_MuData);
    m_mapSrcIP = inMap;
}

std::map<std::string, std::string> Config::GetMapDstIP(){
    std::lock_guard<std::mutex> lock(m_MuData);
    return m_mapDstIP;
}

std::map<std::string, std::string> Config::GetMapSrcIP(){
    std::lock_guard<std::mutex> lock(m_MuData);
    //    boost::mutex::scoped_lock lock(m_MuData);
    return m_mapSrcIP;
}

void Config::SetSchedulerEnable(const bool& bState){
    std::lock_guard<std::mutex> lock(m_MuData);
    m_bSchedulerEnable = bState;
}

bool Config::GetSchedulerEnable(){
    std::lock_guard<std::mutex> lock(m_MuData);
    return m_bSchedulerEnable;
}

void Config::SetOneTimeOnly(const bool& bIN){
    std::lock_guard<std::mutex> lock(m_MuData);
    m_bOneTimeOnly = bIN;
}

bool Config::GetOneTimeOnly(){
    std::lock_guard<std::mutex> lock(m_MuData);
    return m_bOneTimeOnly;
}

void Config::SetSchedulerDay(const bool& bSun
                             , const bool& bMon
                             , const bool& bTue
                             , const bool& bWed
                             , const bool& bThru
                             , const bool& bFri
                             , const bool& bSat){
    std::lock_guard<std::mutex> lock(m_MuData);
    m_vecSchedulerDay[0] = bSun;
    m_vecSchedulerDay[1] = bMon;
    m_vecSchedulerDay[2] = bTue;
    m_vecSchedulerDay[3] = bWed;
    m_vecSchedulerDay[4] = bThru;
    m_vecSchedulerDay[5] = bFri;
    m_vecSchedulerDay[6] = bSat;
}

std::vector<bool> Config::GetSchedulerDay(){
    std::lock_guard<std::mutex> lock(m_MuData);
    return m_vecSchedulerDay;
}

void Config::SetDateTime(const boost::posix_time::ptime& DateTime){
    std::lock_guard<std::mutex> lock(m_MuData);
    m_DateTime = DateTime;
}

boost::posix_time::ptime Config::GetDateTime(){
    std::lock_guard<std::mutex> lock(m_MuData);
    return m_DateTime;
}

void Config::SetConfigPath(const std::string& strPath){
    std::lock_guard<std::mutex> lock(m_MuData);
  m_strLatestConfigPath = strPath;
}

std::string Config::GetConfigPath(){
    std::lock_guard<std::mutex> lock(m_MuData);
  return m_strLatestConfigPath == "" ? m_strDefaultConfigPath : m_strLatestConfigPath;
}
