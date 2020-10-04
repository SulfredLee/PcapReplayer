#ifndef SPEEDCTRL_H
#define SPEEDCTRL_H

#include <atomic>
#include <chrono>
#include <thread>
#include <functional>

#include "pcap.h"

#include "MSecTimer.h"

#include <string>

class Config;

class SpeedCtrl{
public:
    SpeedCtrl();
    ~SpeedCtrl();

    void InitComponent(std::function<void (pcap_pkthdr*, const unsigned char*)> fn_1,
                       std::function<void (unsigned int, double, double)> fn_2,
                       Config* pConfig);
    void SendPacket(pcap_pkthdr* pHeader, const unsigned char* pData);
    void Reset();
private:
    void SpeedFactorTimerCallback();
    void OneSecTimerCallback();
private:
    std::function<void (pcap_pkthdr* pHeader, const unsigned char* pData)> m_fn_1;
    std::function<void (unsigned int unSentByte, double dPktTime, double dSendTimeDiff)> m_fn_2;
    Config* m_pConfig;
    MSecTimer m_SpeedFactorTimer;
    MSecTimer m_OneSecTimer;
    std::atomic<double> m_dNextSendTime; // unit in second
    std::atomic<double> m_dPktArrivalTime; // unit in second
    std::atomic<unsigned int> m_unSentByte; // sent bit within one second
    double m_dSpeedLimit;
    double m_dSpeedFactor;
    double m_dSendTimeDiff; // time left to send next package
};

#endif
