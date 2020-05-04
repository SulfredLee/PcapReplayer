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

    // Init all component
    Config config;
    MainWindowComponent MainWindowCompo;
    MainWindow MW;
    PlayerCtrlComponent PlayerCtrlCompo;
    PlayerCtrl playerCtrl;

    // handle BackEnd
    PlayerCtrlCompo.pConfig = &config;
    PlayerCtrlCompo.pMainWindow = &MW;
    playerCtrl.InitComponent(PlayerCtrlCompo);

    // handle FrontEnd
    MainWindowCompo.pConfig = &config;
    MainWindowCompo.pMsgQ = playerCtrl.GetMsgQ();
    MW.InitComponent(MainWindowCompo);
    MW.show();

    bool bRTN = app.exec();

    LOGMSG_INFO("OUT");
    return bRTN;
}
