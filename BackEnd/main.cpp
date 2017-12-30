#include <iostream>

#include <QApplication>

#include "mainwindow.h"
#include "Config.h"
#include "LogMgr.h"
#include "PlayerCtrl.h"

int main(int argc, char* argv[]){
    QApplication app(argc, argv);

    Logging::CLogTargetDebugger traceLogger(Logging::LOG_LEVEL_TRACE);
    Logging::CLoggerFactory::getDefaultInstance()->AddTarget(&traceLogger);

    LOGMSG_INFO("IN");

    // handle Config
    Config config;
    // handle BackEnd
    PlayerCtrlComponent PlayerCtrlCompo;
    PlayerCtrl playerCtrl;
    PlayerCtrlCompo.pConfig = &config;
    playerCtrl.InitComponent(PlayerCtrlCompo);

    // handle FrontEnd
    MainWindowComponent MainWindowCompo;
    MainWindow MW;
    MainWindowCompo.pConfig = &config;
    MainWindowCompo.pMsgQ = playerCtrl.GetMsgQ();
    MW.InitComponent(MainWindowCompo);
    MW.show();

    bool bRTN = app.exec();

    LOGMSG_INFO("OUT");
    return bRTN;
}
