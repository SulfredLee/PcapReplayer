#ifndef SPEEDCTRL_H
#define SPEEDCTRL_H

#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/chrono.hpp>
#include <boost/atomic.hpp>

#include "pcap.h"

#include "MSecTimer.h"

#include <string>

class Config;

class SpeedCtrl{
public:
    SpeedCtrl();
    ~SpeedCtrl();

    void InitComponent(boost::function<void (pcap_pkthdr*,
                                             const unsigned char*,
                                             unsigned int)> f,
                       Config* pConfig);
    void SendPacket(pcap_pkthdr* pHeader, const unsigned char* pData);
private:
    void SpeedFactorTimerCallback();
    void OneSecTimerCallback();
private:
    boost::function<void (pcap_pkthdr* pHeader,
                          const unsigned char* pData,
                          unsigned int unSentByte)> m_fOutputCallback;
    Config* m_pConfig;
    MSecTimer m_SpeedFactorTimer;
    MSecTimer m_OneSecTimer;
    boost::atomic<double> m_dNextSendTime; // unit in second
    boost::atomic<unsigned int> m_unCurSentByte; // sent bit within one second
    boost::atomic<unsigned int> m_unPreSentByte; // sent bit within one second
};

#endif
