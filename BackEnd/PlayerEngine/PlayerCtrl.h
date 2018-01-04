#ifndef PLAYERCTRL_H
#define PLAYERCTRL_H

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/chrono.hpp>
#include <boost/atomic.hpp>
#include <boost/timer.hpp>

#include "Common.h"
#include "MsgQ.h"
#include "PcapReader.h"
#include "PcapSender.h"
#include "SpeedCtrl.h"

#include <string>
#include <sstream>

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
    void MsgQMain();
    void ReplayMain();
private:
    MsgQ<PlayerMsg> m_MsgQ;
    PlayerCtrlComponent m_Compo;
    PcapReader m_PcapReader;
    SpeedCtrl m_SpeedCtrl;
    PcapSender m_PcapSender;
    boost::thread m_ReplayThread;
    boost::thread m_MsgQThread;
    boost::atomic<bool> m_bPause;
    int m_nPreProgress; // previous progressBar status
    int m_nLoopCount;
};

#endif
