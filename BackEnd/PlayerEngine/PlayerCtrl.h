#ifndef PLAYERCTRL_H
#define PLAYERCTRL_H

#include "Common.h"
#include "MsgQ.h"

class Config;

struct PlayerCtrlComponent{
    Config* pConfig;
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
    void Main();
private:
    MsgQ<PlayerMsg> m_MsgQ;
    PlayerCtrlComponent m_Compo;
};

#endif
