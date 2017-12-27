#ifndef CONFIG_H
#define DONFIG_H

#include <vector>
#include <string>

class Config{
public:
    Config();
    ~Config();

    void AddPcapFiles(const std::vector<std::string>& PcapFiles);
    void RemovePcapFile(const int& nIndex);
    void RemoveAllPcapFile();

    void SetLatestFilePath(const std::string& INStr);
    std::string GetLatestFilePath();
private:
    std::vector<std::string> m_PcapFiles;
    std::string m_strLatestFilePath;
};
#endif
