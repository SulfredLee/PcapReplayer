#ifndef PLAYERCTRL_H
#define PLAYERCTRL_H

#include <boost/timer.hpp>

#include <chrono>
#include <atomic>
#include <functional>
#include <thread>
#include <memory>

#include "Common.h"
#include "MsgQ.h"
#include "PcapReader.h"
#include "PcapSender.h"
#include "SpeedCtrl.h"

#include <string>
#include <sstream>
#include <fstream>

class Config;
class MainWindow;

struct PlayerCtrlComponent{
    Config* pConfig;
    MainWindow* pMainWindow;
};

class PlayerCtrl{
public:
    PlayerCtrl();
    ~PlayerCtrl();

    void InitComponent(const PlayerCtrlComponent& InCompo);
    MsgQ<PlayerMsg>* GetMsgQ();
private:
    void ProcessStop();
    void ProcessPause();
    void ProcessPlay();
    void Process_PcapReader(pcap_pkthdr* pHeader, const unsigned char* pData, int nProgress);
    void Process_SpeedCtrl_1(pcap_pkthdr* pHeader, const unsigned char* pData);
    void Process_SpeedCtrl_2(unsigned int unSentByte, double dPktTime, double dSendTimeDiff);
    void Process_PcapSender(pcap_pkthdr* pHeader, const unsigned char* pData);
    void Serialization(const bool& bSave, const std::string& strFile);
    void MsgQMain();
    void ReplayMain();
private:
    MsgQ<PlayerMsg> m_MsgQ;
    PlayerCtrlComponent m_Compo;
    PcapReader m_PcapReader;
    SpeedCtrl m_SpeedCtrl;
    PcapSender m_PcapSender;
    std::thread m_ReplayThread;
    std::thread m_MsgQThread;
    std::atomic<bool> m_bPause;
    std::atomic<bool> m_bStop;
    int m_nPreProgress; // previous progressBar status
    int m_nLoopCount;
};

#endif
