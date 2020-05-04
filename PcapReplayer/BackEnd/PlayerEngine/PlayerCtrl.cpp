#include "PlayerCtrl.h"
#include "LogMgr.h"
#include "Config.h"
#include "mainwindow.h"

PlayerCtrl::PlayerCtrl()
    : m_MsgQThread(&PlayerCtrl::MsgQMain, this){
    LOGMSG_INFO("IN");

    m_bPause = false;
    m_nPreProgress = -1;
    m_nLoopCount = -1;

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

    emit m_Compo.pMainWindow->onSerialization_FromPlayerCtrl(true);

    LOGMSG_INFO("OUT");
}

void PlayerCtrl::InitComponent(const PlayerCtrlComponent& InCompo){
    LOGMSG_INFO("IN");
    m_Compo = InCompo;
    // handle Replay pipe line
    m_PcapReader.InitComponent(boost::bind(&PlayerCtrl::Process_PcapReader, this, _1, _2, _3)
                               , m_Compo.pConfig);
    m_SpeedCtrl.InitComponent(boost::bind(&PlayerCtrl::Process_SpeedCtrl_1, this, _1, _2),
                              boost::bind(&PlayerCtrl::Process_SpeedCtrl_2, this, _1, _2, _3),
                              m_Compo.pConfig);
    m_PcapSender.InitComponent(boost::bind(&PlayerCtrl::Process_PcapSender, this, _1, _2),
                               m_Compo.pConfig);

    auto p = boost::make_shared<PlayerMsg>();
    *p = PlayerMsg::Stop;
    m_MsgQ.push(p);

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

    m_SpeedCtrl.Reset();

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

        m_PcapReader.Reset();
        m_SpeedCtrl.Reset();
        m_nLoopCount = m_Compo.pConfig->GetLoopCount();
        emit m_Compo.pMainWindow->onStatusBar_RemainLoopCount_FromPlayerCtrl(m_nLoopCount - 1);

        LOGMSG_INFO("OUT");
        return;
    }else{
        m_Compo.pConfig->SetPlayerStatus(PlayerStatus::Play);
        m_bPause = false;

        m_PcapReader.Reset();
        m_SpeedCtrl.Reset();
        m_PcapSender.SetAdapter();
        m_nLoopCount = m_Compo.pConfig->GetLoopCount();

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

void PlayerCtrl::Process_SpeedCtrl_1(pcap_pkthdr* pHeader, const unsigned char* pData){
    m_PcapSender.SendPacket(pHeader, pData);
}

void PlayerCtrl::Process_SpeedCtrl_2(unsigned int unSentByte, double dPktTime, double dSendTimeDiff){
    // handle UI update
    emit m_Compo.pMainWindow->onStatusBar_SentByte_FromPlayerCtrl(unSentByte);
    emit m_Compo.pMainWindow->onStatusBar_CurPktTime_FromPlayerCtrl(dPktTime);
    emit m_Compo.pMainWindow->onStatusBar_SendTimeDiff_FromPlayerCtrl(dSendTimeDiff);
    emit m_Compo.pMainWindow->onStatusBar_Invalidate_FromPlayerCtrl();
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
    do{
        std::stringstream ssLine;
        ssLine << "Remain Loop Count: " << m_nLoopCount - 1;
        LOGMSG_INFO(ssLine.str());

        emit m_Compo.pMainWindow->onStatusBar_RemainLoopCount_FromPlayerCtrl(m_nLoopCount - 1);

        m_SpeedCtrl.Reset();
        for (size_t i = 0; i < vecPcapFiles.size(); i++){
            if (m_Compo.pConfig->GetPlayerStatus() == PlayerStatus::Stop){
                break;
            }
            // todo: replay file one by one
            ssLine.str(std::string());
            ssLine << "Start to replay: " << vecPcapFiles[i];
            LOGMSG_INFO(ssLine.str());

            m_PcapReader.ReadFile(vecPcapFiles[i]);

            ssLine.str(std::string());
            ssLine << "Finish replay: " << vecPcapFiles[i];
            LOGMSG_INFO(ssLine.str());

            emit m_Compo.pMainWindow->onListWidgetNextFile_FromPlayerCtrl();
        }
    }while (--m_nLoopCount > 0);
    // always return to stop UI status when all files are finished
    emit m_Compo.pMainWindow->onStop_FromPlayerCtrl();
    LOGMSG_INFO("OUT");
}
