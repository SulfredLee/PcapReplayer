#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDirIterator>
#include <QMessageBox>
#include <QtWidgets/QTableWidget>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDragLeaveEvent>
#include <QtGui/QDragMoveEvent>
#include <QtGui/QDropEvent>
#include <QtCore/QMimeData>
// #include <QtCore/QFile>
// #include <QtWidgets/QFileDialog>


#include <string>
#include <sstream>
#include <vector>
#include <map>

#include <boost/date_time.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/map.hpp>

#include "MsgQ.h"
#include "Common.h"
#include "schedulerdialog.h"
#include "DailyTimer.h"
#include "pcap.h"

class Config;

namespace Ui {
class MainWindow;
}

struct MainWindowComponent{
    Config* pConfig;
    MsgQ<PlayerMsg>* pMsgQ;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void InitComponent(const MainWindowComponent& InCompo);
    void SwitchUIStatus_Init();
    void SwitchUIStatus_Play_Pause();
    void SwitchUIStatus_Stop();
private:
    void AddPcapFilesToUI(const QStringList& INFiles);
    void AddIPMapToUI(const QMap<QString, QString>& SrcMap
                      , const QMap<QString, QString>& DstMap);
    void AddSchedulToUI();
    void GetBitPerSec(double bit, QString& line, int step);
    QMap<QString, QString> GetMapFromNetworkTable(QTableWidget const * inTable);
private: // local utils
    std::string ConvertQString2String(const QString& qstr);
    QString ConvertString2QString(const std::string& str);
    std::vector<std::string> ConvertQStringList(const QStringList& qstrList);
    QStringList ConvertVectorString(const std::vector<std::string>& vecStr);
    bool IsFileExists(const QString& qstrPath);
    QString ConvertTime2QString(double dTime);
    std::map<std::string, std::string> ConvertQMap2StdMap(const QMap<QString, QString>& inMap);
    QMap<QString, QString> ConvertStdMap2QMap(const std::map<std::string, std::string>& inMap);
    void DailyTimerCallback();
    // override
    void dragEnterEvent(QDragEnterEvent* event);
    // override
    void dragMoveEvent(QDragMoveEvent* event);
    // override
    void dragLeaveEvent(QDragLeaveEvent* event);
    // override
    void dropEvent(QDropEvent* event);
private:
    Ui::MainWindow *ui;

    QString m_qstrCurAppPath;
    QString m_qstrSentByte;
    QString m_qstrCurPktTime;
    QString m_qstrRemainLoopCount;
    QString m_qstrSendTimeDiff;
    MainWindowComponent m_Compo;
    SchedulerDialog m_Schedulerdialog;
    DailyTimer* m_pDailyTimer;
signals:
    void onPlay_FromPlayerCtrl();
    void onPlay_FromMainWindow();
    void onPause_FromPlayerCtrl();
    void onStop_FromPlayerCtrl();
    void onProgressBar_FromPlayerCtrl(int);
    void onStatusBar_SentByte_FromPlayerCtrl(int);
    void onStatusBar_CurPktTime_FromPlayerCtrl(double);
    void onStatusBar_RemainLoopCount_FromPlayerCtrl(int);
    void onStatusBar_SendTimeDiff_FromPlayerCtrl(double);
    void onStatusBar_Invalidate_FromPlayerCtrl();
    void onSerialization_FromPlayerCtrl(bool);
private slots:
    // handle menu and tool bar
    void onOpen_File();
    void onOpen_Folder();
    void onSave_Config();
    void onLoad_Config();
    void onScheduler();
    // handle push button
    void onPlay();
    void onPause();
    void onStop();
    void onRemove();
    void onRemoveAll();
    void onRegularPaly();
    void onAddSrcMap();
    void onAddDstMap();
    void onRemoveScrMapIP();
    void onRemoveDstMapIP();
    void onSelectInterface(int);
    // handle progressBar
    void onProgressBar(int);
    // handle statusBar
    void onStatusBar_SentByte(int);
    void onStatusBar_CurPktTime(double);
    void onStatusBar_RemainLoopCount(int);
    void onStatusBar_SendTimeDiff(double);
    void onStatusBar_Invalidate();
    void onSerialization(bool);
};

#endif // MAINWINDOW_H
