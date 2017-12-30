#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDirIterator>
#include <QMessageBox>


#include <string>
#include <vector>

#include "MsgQ.h"
#include "Common.h"

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
    void SwitchUIStatus_Play();
    void SwitchUIStatus_Pause();
    void SwitchUIStatus_Stop();
private:
    void AddPcapFilesToUI(const QStringList& INFiles);
private: // local utils
    std::string ConvertQString2String(const QString& qstr);
    QString ConvertString2QString(const std::string& str);
    std::vector<std::string> ConvertQStringList(const QStringList& qstrList);
    QStringList ConvertVectorString(const std::vector<std::string>& vecStr);
    bool IsFileExists(const QString& qstrPath);
private:
    Ui::MainWindow *ui;

    QString m_strCurAppPath;
    MainWindowComponent m_Compo;

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
};

#endif // MAINWINDOW_H
