#include "PlayerCtrl.h"
#include "LogMgr.h"
#include "Config.h"
#include "mainwindow.h"

PlayerCtrl::PlayerCtrl()
    : m_MsgQThread(&PlayerCtrl::MsgQMain, this){
    LOGMSG_INFO("IN");
    auto p = boost::make_shared<PlayerMsg>();
    *p = PlayerMsg::Stop;
    m_MsgQ.push(p);

    m_bPause = false;
    m_nPreProgress = -1;
    m_unPreSentByte = 0;

    LOGMSG_INFO("OUT");
}

PlayerCtrl::~PlayerCtrl(){
    LOGMSG_INFO("IN");
    m_ReplayThread.interrupt();
    m_ReplayThread.join();

    m_MsgQThread.interrupt();
    auto p = boost::make_shared<PlayerMsg>();
    *p = PlayerMsg::Stop;
    m_MsgQ.push(p);
    m_MsgQThread.join();

    LOGMSG_INFO("OUT");
}

void PlayerCtrl::InitComponent(const PlayerCtrlComponent& InCompo){
    LOGMSG_INFO("IN");
    m_Compo = InCompo;

    // handle Replay pipe line
    m_PcapReader.InitComponent(boost::bind(&PlayerCtrl::Process_PcapReader, this, _1, _2, _3));
    m_SpeedCtrl.InitComponent(boost::bind(&PlayerCtrl::Process_SpeedCtrl, this, _1, _2, _3),
                              m_Compo.pConfig);
    LOGMSG_INFO("OUT");
}

MsgQ<PlayerMsg>* PlayerCtrl::GetMsgQ(){
    LOGMSG_INFO("IN");
    LOGMSG_INFO("OUT");
    return &m_MsgQ;
}

void PlayerCtrl::ProcessStop(){
    LOGMSG_INFO("IN");

    if (m_Compo.pConfig->GetPlayerStatus() == PlayerStatus::Stop){
        LOGMSG_INFO("OUT");
        return;
    }

    m_Compo.pConfig->SetPlayerStatus(PlayerStatus::Stop);

    // we stop the current replay
    m_ReplayThread.interrupt();

    LOGMSG_INFO("OUT");
}

void PlayerCtrl::ProcessPause(){
    LOGMSG_INFO("IN");

    if (m_Compo.pConfig->GetPlayerStatus() == PlayerStatus::Pause){
        LOGMSG_INFO("OUT");
        return;
    }

    m_Compo.pConfig->SetPlayerStatus(PlayerStatus::Pause);

    // we pause the current replay
    m_bPause = true;

    LOGMSG_INFO("OUT");
}

void PlayerCtrl::ProcessPlay(){
    LOGMSG_INFO("IN");

    if (m_Compo.pConfig->GetPlayerStatus() == PlayerStatus::Play){
        LOGMSG_INFO("OUT");
        return;
    }else if (m_Compo.pConfig->GetPlayerStatus() == PlayerStatus::Pause){
        m_Compo.pConfig->SetPlayerStatus(PlayerStatus::Play);
        m_bPause = false;
        LOGMSG_INFO("OUT");
        return;
    }else{
        m_Compo.pConfig->SetPlayerStatus(PlayerStatus::Play);
        m_bPause = false;
        m_PcapSender.InitComponent(boost::bind(&PlayerCtrl::Process_PcapSender, this, _1, _2),
                                   m_Compo.pConfig);
        m_ReplayThread = boost::thread(&PlayerCtrl::ReplayMain, this);
        LOGMSG_INFO("OUT");
        return;
    }
}

void PlayerCtrl::Process_PcapReader(pcap_pkthdr* pHeader, const unsigned char* pData, int nProgress){
    while (m_bPause){
        boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
        boost::this_thread::interruption_point(); // this thread is m_ReplayThread
    }
    boost::this_thread::interruption_point(); // this thread is m_ReplayThread
    if (m_nPreProgress != nProgress){
        m_nPreProgress = nProgress;
        emit m_Compo.pMainWindow->onProgressBar_FromPlayerCtrl(nProgress);
    }

    m_SpeedCtrl.SendPacket(pHeader, pData);
}

void PlayerCtrl::Process_SpeedCtrl(pcap_pkthdr* pHeader, const unsigned char* pData, unsigned int unSentByte){
    m_PcapSender.SendPacket(pHeader, pData);
    if (m_unPreSentByte != unSentByte){ // if we need to update UI
        // handle current sent byte
        m_unPreSentByte = unSentByte;
        // handle current packet time
        long lPacketArrivalSec = pHeader->ts.tv_sec;
        long lPacketArrivalmicroseconds = pHeader->ts.tv_usec;
        double dCurPktTime = lPacketArrivalSec + lPacketArrivalmicroseconds*0.000001;
        // handle UI update
        emit m_Compo.pMainWindow->onStatusBar_SentByte_FromPlayerCtrl(unSentByte);
        emit m_Compo.pMainWindow->onStatusBar_CurPktTime_FromPlayerCtrl(dCurPktTime);
        emit m_Compo.pMainWindow->onStatusBar_Invalidate_FromPlayerCtrl();
    }
}

void PlayerCtrl::Process_PcapSender(pcap_pkthdr* pHeader, const unsigned char* pData){
}

void PlayerCtrl::MsgQMain(){
    LOGMSG_INFO("IN");
    while (true){
        boost::this_thread::interruption_point(); // this thread is m_MsgQThread
        auto pMsg = boost::make_shared<PlayerMsg>();
        while (m_MsgQ.get(pMsg)){
            switch(*pMsg){
            case PlayerMsg::Stop:
                ProcessStop();
                break;
            case PlayerMsg::Pause:
                ProcessPause();
                break;
            case PlayerMsg::Play:
                ProcessPlay();
                break;
            default:
                break;
            }
        }
    }
    LOGMSG_INFO("OUT");
}

void PlayerCtrl::ReplayMain(){
    LOGMSG_INFO("IN");
    auto vecPcapFiles = m_Compo.pConfig->GetPcapFiles();
    for (size_t i = 0; i < vecPcapFiles.size(); i++){
        if (m_Compo.pConfig->GetPlayerStatus() == PlayerStatus::Stop){
            break;
        }
        // todo: replay file one by one
        std::stringstream ssLine;
        ssLine << "Start to replay: " << vecPcapFiles[i];
        LOGMSG_INFO(ssLine.str());

        m_PcapReader.ReadFile(vecPcapFiles[i]);

        ssLine.str(std::string());
        ssLine << "Finish replay: " << vecPcapFiles[i];
        LOGMSG_INFO(ssLine.str());
    }
    // always return to stop UI status when all files are finished
    emit m_Compo.pMainWindow->onStop_FromPlayerCtrl();
    LOGMSG_INFO("OUT");
}
