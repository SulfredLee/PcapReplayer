#include "Config.h"

Config::Config(){
    
}

Config::~Config(){
    
}

void Config::AddPcapFiles(const std::vector<std::string>& PcapFiles){
    m_PcapFiles.insert(m_PcapFiles.end(), PcapFiles.begin(), PcapFiles.end());
}

void Config::RemovePcapFile(const int& nIndex){
    m_PcapFiles.erase(m_PcapFiles.begin() + nIndex);
}

void Config::RemoveAllPcapFile(){
    m_PcapFiles.clear();
}

void Config::SetLatestFilePath(const std::string& INStr){
    m_strLatestFilePath = INStr;
}

std::string Config::GetLatestFilePath(){
    return m_strLatestFilePath;
}
