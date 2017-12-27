#include <iostream>

#include <QApplication>

#include "mainwindow.h"
#include "Config.h"
#include "LogMgr.h"

int main(int argc, char* argv[]){
    QApplication app(argc, argv);

    Logging::CLogTargetDebugger traceLogger(Logging::LOG_LEVEL_TRACE);
    Logging::CLoggerFactory::getDefaultInstance()->AddTarget(&traceLogger);

    LOGMSG_INFO("IN");

    MainWindowComponent compo;
    Config config;
    MainWindow MW;
    compo.pConfig = &config;
    MW.InitComponent(compo);
    MW.show();

    bool bRTN = app.exec();

    LOGMSG_INFO("OUT");
    return bRTN;
}
