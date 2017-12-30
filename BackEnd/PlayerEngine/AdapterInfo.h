#ifndef ADAPTERINFO_H
#define ADAPTERINFO_H

#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iomanip>

#include "pcap.h"

class AdapterInfo{
public:
    AdapterInfo();
    ~AdapterInfo();

private:
    // AdapterName for an example \Device\NPF_{7485820D-FCE1-47FE-8D7B-E747D7853137}
    std::vector<std::string> m_vecAdapterName;
    std::vector<std::string> m_vecInterfaceInfo; // with IP, AdapterName
    std::vector<std::string> m_vecIP;
    std::vector<std::string> m_vecMacAddress; // Mac address for those NIC, network interface control
    bool m_bHasAdapter;
};

#endif
