#include "SpeedCtrl.h"
#include "Config.h"
#include "LogMgr.h"

SpeedCtrl::SpeedCtrl()
    : m_SpeedFactorTimer(100, boost::bind(&SpeedCtrl::SpeedFactorTimerCallback, this))
    , m_OneSecTimer(1000, boost::bind(&SpeedCtrl::OneSecTimerCallback, this)){
    m_dNextSendTime = 0;
    m_unCurSentByte = 0;
    m_unPreSentByte = 0;
}

SpeedCtrl::~SpeedCtrl(){
}

void SpeedCtrl::InitComponent(boost::function<void (pcap_pkthdr* pHeader,
                                                    const unsigned char* pData,
                                                    unsigned int unSentByte)> f,
                              Config* pConfig){
    m_pConfig = pConfig;
    m_fOutputCallback = f;
    m_dNextSendTime = 0;
}

void SpeedCtrl::SendPacket(pcap_pkthdr* pHeader, const unsigned char* pData){
    long lPacketArrivalSec = pHeader->ts.tv_sec;
    long lPacketArrivalmicroseconds = pHeader->ts.tv_usec;
    double dArrivalTime = lPacketArrivalSec + lPacketArrivalmicroseconds*0.000001;

    if (m_dNextSendTime == 0){
        m_dNextSendTime = dArrivalTime;
    }
    while (dArrivalTime > m_dNextSendTime){
        boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
    }
    m_unCurSentByte = m_unCurSentByte + pHeader->len;
    m_fOutputCallback(pHeader, pData, m_unPreSentByte.load());
}

void SpeedCtrl::SpeedFactorTimerCallback(){
    if (m_dNextSendTime != 0){
        m_dNextSendTime = m_dNextSendTime + (0.1 * m_pConfig->GetSpeedFactor());
    }
}

void SpeedCtrl::OneSecTimerCallback(){
    m_unPreSentByte = m_unCurSentByte.load();
    m_unCurSentByte = 0;
}
