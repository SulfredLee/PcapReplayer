#include "SpeedCtrl.h"
#include "Config.h"
#include "Logger.h"

SpeedCtrl::SpeedCtrl()
    : m_SpeedFactorTimer(100, boost::bind(&SpeedCtrl::SpeedFactorTimerCallback, this))
    , m_OneSecTimer(1000, boost::bind(&SpeedCtrl::OneSecTimerCallback, this)){
    LOGMSG_CLASS_NAME("SpeedCtrl");
    m_dNextSendTime = 0;
    m_dPktArrivalTime = 0;
    m_unSentByte = 0;
    m_dSendTimeDiff = 0;
}

SpeedCtrl::~SpeedCtrl(){
}

void SpeedCtrl::InitComponent(boost::function<void (pcap_pkthdr*, const unsigned char*)> fn_1,
                              boost::function<void (unsigned int, double, double)> fn_2,
                              Config* pConfig){
    m_pConfig = pConfig;
    m_fn_1 = fn_1;
    m_fn_2 = fn_2;
    Reset();
}

void SpeedCtrl::SendPacket(pcap_pkthdr* pHeader, const unsigned char* pData){
    long lPacketArrivalSec = pHeader->ts.tv_sec;
    long lPacketArrivalmicroseconds = pHeader->ts.tv_usec;
    double dArrivalTime = lPacketArrivalSec + lPacketArrivalmicroseconds*0.000001;

    if (m_dSpeedLimit < 0) {// Fix Speed disable
        if (m_dNextSendTime == 0){
            m_dNextSendTime = dArrivalTime;
        }
        while (dArrivalTime > m_dNextSendTime){//Speed factor block
            m_dSendTimeDiff = dArrivalTime - m_dNextSendTime;
            boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
        }
    }else{// Speed factor disable
        while (m_dSpeedLimit > 0 && m_unSentByte * 8 > m_dSpeedLimit){//Fix Speed block
            boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
        }
    }

    m_dPktArrivalTime = dArrivalTime;
    m_unSentByte = m_unSentByte + pHeader->len;
    m_fn_1(pHeader, pData);
}

void SpeedCtrl::Reset(){
    m_dNextSendTime = 0;
    m_dPktArrivalTime = 0;
    m_dSendTimeDiff = 0;
    m_dSpeedFactor = m_pConfig->GetSpeedFactor();
    m_dSpeedLimit = m_pConfig->GetSpeedLimit();
}

void SpeedCtrl::SpeedFactorTimerCallback(){
    if (m_dNextSendTime != 0){
        m_dNextSendTime = m_dNextSendTime + (0.1 * m_dSpeedFactor);
    }
}

void SpeedCtrl::OneSecTimerCallback(){
    m_fn_2(m_unSentByte, m_dPktArrivalTime, m_dSendTimeDiff);
    m_unSentByte = 0;
}
