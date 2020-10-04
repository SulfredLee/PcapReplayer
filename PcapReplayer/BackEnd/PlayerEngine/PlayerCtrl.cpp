#include "PlayerCtrl.h"
#include "Logger.h"
#include "Config.h"
#include "mainwindow.h"

PlayerCtrl::PlayerCtrl()
    : m_MsgQThread(&PlayerCtrl::MsgQMain, this){
    LOGMSG_CLASS_NAME("PlayerCtrl");
    LOGMSG_MSG_S() << "IN" << std::endl;

    m_bPause = false;
    m_nPreProgress = -1;
    m_nLoopCount = -1;

    LOGMSG_MSG_S() << "OUT" << std::endl;
}

PlayerCtrl::~PlayerCtrl(){
    LOGMSG_MSG_S() << "IN" << std::endl;
    m_ReplayThread.interrupt();
    m_ReplayThread.join();

    m_MsgQThread.interrupt();
    auto p = boost::make_shared<PlayerMsg>();
    *p = PlayerMsg::Stop;
    m_MsgQ.push(p);
    m_MsgQThread.join();

    emit m_Compo.pMainWindow->onSerialization_FromPlayerCtrl(true);

    LOGMSG_MSG_S() << "OUT" << std::endl;
}

void PlayerCtrl::InitComponent(const PlayerCtrlComponent& InCompo){
    LOGMSG_MSG_S() << "IN" << std::endl;
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

    LOGMSG_MSG_S() << "OUT" << std::endl;
}

MsgQ<PlayerMsg>* PlayerCtrl::GetMsgQ(){
    LOGMSG_MSG_S() << "IN" << std::endl;
    LOGMSG_MSG_S() << "OUT" << std::endl;
    return &m_MsgQ;
}

void PlayerCtrl::ProcessStop(){
    LOGMSG_MSG_S() << "IN" << std::endl;

    if (m_Compo.pConfig->GetPlayerStatus() == PlayerStatus::Stop){
        LOGMSG_MSG_S() << "OUT" << std::endl;
        return;
    }

    m_Compo.pConfig->SetPlayerStatus(PlayerStatus::Stop);

    // we stop the current replay
    m_ReplayThread.interrupt();

    m_SpeedCtrl.Reset();

    LOGMSG_MSG_S() << "OUT" << std::endl;
}

void PlayerCtrl::ProcessPause(){
    LOGMSG_MSG_S() << "IN" << std::endl;

    if (m_Compo.pConfig->GetPlayerStatus() == PlayerStatus::Pause){
        LOGMSG_MSG_S() << "OUT" << std::endl;
        return;
    }

    m_Compo.pConfig->SetPlayerStatus(PlayerStatus::Pause);

    // we pause the current replay
    m_bPause = true;

    LOGMSG_MSG_S() << "OUT" << std::endl;
}

void PlayerCtrl::ProcessPlay(){
    LOGMSG_MSG_S() << "IN" << std::endl;

    if (m_Compo.pConfig->GetPlayerStatus() == PlayerStatus::Play){
        LOGMSG_MSG_S() << "OUT" << std::endl;
        return;
    }else if (m_Compo.pConfig->GetPlayerStatus() == PlayerStatus::Pause){
        m_Compo.pConfig->SetPlayerStatus(PlayerStatus::Play);
        m_bPause = false;

        m_PcapReader.Reset();
        m_SpeedCtrl.Reset();
        m_nLoopCount = m_Compo.pConfig->GetLoopCount();
        emit m_Compo.pMainWindow->onStatusBar_RemainLoopCount_FromPlayerCtrl(m_nLoopCount - 1);

        LOGMSG_MSG_S() << "OUT" << std::endl;
        return;
    }else{
        m_Compo.pConfig->SetPlayerStatus(PlayerStatus::Play);
        m_bPause = false;

        m_PcapReader.Reset();
        m_SpeedCtrl.Reset();
        m_PcapSender.SetAdapter();
        m_nLoopCount = m_Compo.pConfig->GetLoopCount();

        m_ReplayThread = boost::thread(&PlayerCtrl::ReplayMain, this);
        LOGMSG_MSG_S() << "OUT" << std::endl;
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
    LOGMSG_MSG_S() << "IN" << std::endl;
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
    LOGMSG_MSG_S() << "OUT" << std::endl;
}

void PlayerCtrl::ReplayMain(){
    LOGMSG_MSG_S() << "IN" << std::endl;
    auto vecPcapFiles = m_Compo.pConfig->GetPcapFiles();
    do{
        LOGMSG_MSG_S() << "Remain Loop Count: " << m_nLoopCount - 1 << std::endl;

        emit m_Compo.pMainWindow->onStatusBar_RemainLoopCount_FromPlayerCtrl(m_nLoopCount - 1);

        m_SpeedCtrl.Reset();
        for (size_t i = 0; i < vecPcapFiles.size(); i++){
            if (m_Compo.pConfig->GetPlayerStatus() == PlayerStatus::Stop){
                break;
            }
            // todo: replay file one by one
            LOGMSG_MSG_S() << "Start to replay: " << vecPcapFiles[i];

            m_PcapReader.ReadFile(vecPcapFiles[i]);

            LOGMSG_MSG_S() << "Finish replay: " << vecPcapFiles[i] << std::endl;

            emit m_Compo.pMainWindow->onListWidgetNextFile_FromPlayerCtrl();
        }
    }while (--m_nLoopCount > 0);
    // always return to stop UI status when all files are finished
    emit m_Compo.pMainWindow->onStop_FromPlayerCtrl();
    LOGMSG_MSG_S() << "OUT" << std::endl;
}
