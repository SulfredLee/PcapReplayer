#include <iostream>

#include <QApplication>

#include "mainwindow.h"
#include "Config.h"
#include "Logger.h"
#include "PlayerCtrl.h"

int main(int argc, char* argv[]){
    QApplication app(argc, argv);

    Logger::LoggerConfig loggerConfig;
    loggerConfig.logLevel = Logger::LogLevel::WARN;
    loggerConfig.logPath = "./LogFiles";
    loggerConfig.fileSize = 0;
    loggerConfig.fileSizeLimit = 4 * 1024 * 1024; // 4 MByte
    loggerConfig.isToConsole = false;
    loggerConfig.isToFile = true;
    LOGMSG_INIT(loggerConfig);

    LOGMSG_MSG_S_C() << "IN" << std::endl;

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

    LOGMSG_MSG_S_C() << "OUT" << std::endl;
    return bRTN;
}
