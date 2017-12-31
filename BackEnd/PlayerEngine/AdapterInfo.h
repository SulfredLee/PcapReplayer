#ifndef ADAPTERINFO_H
#define ADAPTERINFO_H

#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iomanip>

class AdapterInfo{
public:
    AdapterInfo();
    ~AdapterInfo();

    std::vector<std::string> GetAdapterName();
    std::vector<std::string> GetInterfaceInfo();
    std::vector<std::string> GetIP();
private:
#ifdef WINDOWS
    std::string GetMacAddress(const std::string& strAdapterName);
#endif
private:
    // AdapterName for an example
    // On Unix
    //     ens33
    // On Windows
    //     \Device\NPF_{7485820D-FCE1-47FE-8D7B-E747D7853137}
    std::vector<std::string> m_vecAdapterName;
    std::vector<std::string> m_vecInterfaceInfo; // with IP, AdapterName
    std::vector<std::string> m_vecIP;
#ifdef WINDOWS
    std::vector<std::string> m_vecMacAddress; // Mac address for those NIC, network interface control
#endif
    bool m_bHasAdapter;
};

#endif
