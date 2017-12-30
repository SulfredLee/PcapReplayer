#include "PlayerCtrl.h"

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include "LogMgr.h"
#include "Config.h"

PlayerCtrl::PlayerCtrl(){
    LOGMSG_INFO("IN");
    auto p = boost::make_shared<PlayerMsg>();
    *p = PlayerMsg::Stop;
    m_MsgQ.push(p);

    boost::thread MainThread(&PlayerCtrl::Main, this);
    MainThread.detach();
    LOGMSG_INFO("OUT");
}

PlayerCtrl::~PlayerCtrl(){
    LOGMSG_INFO("IN");
    LOGMSG_INFO("OUT");
}

void PlayerCtrl::InitComponent(const PlayerCtrlComponent& InCompo){
    LOGMSG_INFO("IN");
    m_Compo = InCompo;
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

    LOGMSG_INFO("OUT");
}

void PlayerCtrl::ProcessPause(){
    LOGMSG_INFO("IN");

    if (m_Compo.pConfig->GetPlayerStatus() == PlayerStatus::Pause){
        LOGMSG_INFO("OUT");
        return;
    }

    m_Compo.pConfig->SetPlayerStatus(PlayerStatus::Pause);

    LOGMSG_INFO("OUT");
}

void PlayerCtrl::ProcessPlay(){
    LOGMSG_INFO("IN");

    if (m_Compo.pConfig->GetPlayerStatus() == PlayerStatus::Play){
        LOGMSG_INFO("OUT");
        return;
    }

    m_Compo.pConfig->SetPlayerStatus(PlayerStatus::Play);

    LOGMSG_INFO("OUT");
}

void PlayerCtrl::Main(){
    LOGMSG_INFO("IN");
    while (true){
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
