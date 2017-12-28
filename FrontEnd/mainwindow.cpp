#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Config.h"
#include "LogMgr.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);

    LOGMSG_INFO("IN");

    // handle Menu action
    connect(ui->actionOpen_File_Menu, SIGNAL(triggered()), this, SLOT(onOpen_File()));
    connect(ui->actionOpen_Folder_Menu, SIGNAL(triggered()), this, SLOT(onOpen_Folder()));
    connect(ui->actionSave_Config_Menu, SIGNAL(triggered()), this, SLOT(onSave_Config()));
    connect(ui->actionLoad_Config_Menu, SIGNAL(triggered()), this, SLOT(onLoad_Config()));
    connect(ui->actionScheduler_Menu, SIGNAL(triggered()), this, SLOT(onScheduler()));
    // handle ToolBar action
    connect(ui->actionOpen_File_ToolBar, SIGNAL(triggered()), this, SLOT(onOpen_File()));
    connect(ui->actionOpen_Folder_ToolBar, SIGNAL(triggered()), this, SLOT(onOpen_Folder()));
    connect(ui->actionSave_Config_ToolBar, SIGNAL(triggered()), this, SLOT(onSave_Config()));
    connect(ui->actionLoad_Config_ToolBar, SIGNAL(triggered()), this, SLOT(onLoad_Config()));
    connect(ui->actionScheduler_ToolBar, SIGNAL(triggered()), this, SLOT(onScheduler()));
    // handle Push Button
    connect(ui->BtnPlay, SIGNAL(pressed()), this, SLOT(onPlay()));
    connect(ui->BtnPause, SIGNAL(pressed()), this, SLOT(onPause()));
    connect(ui->BtnStop, SIGNAL(pressed()), this, SLOT(onStop()));
    connect(ui->BtnRemove, SIGNAL(pressed()), this, SLOT(onRemove()));
    connect(ui->BtnRemoveAll, SIGNAL(pressed()), this, SLOT(onRemoveAll()));
    connect(ui->BtnRegularPlay, SIGNAL(pressed()), this, SLOT(onRegularPaly()));
    connect(ui->BtnAddSrcMap, SIGNAL(pressed()), this, SLOT(onAddSrcMap()));
    connect(ui->BtnAddDstMap, SIGNAL(pressed()), this, SLOT(onAddDstMap()));
    connect(ui->BtnRemoveScrIPMap, SIGNAL(pressed()), this, SLOT(onRemoveScrMapIP()));
    connect(ui->BtnRemoveDstIPMap, SIGNAL(pressed()), this, SLOT(onRemoveDstMapIP()));

    m_strCurAppPath = QDir::currentPath();

    // handle UI status
    SwitchUIStatus_Init();

    LOGMSG_INFO("OUT");
}

MainWindow::~MainWindow(){
    LOGMSG_INFO("IN");
    LOGMSG_INFO("OUT");
    delete ui;
}

void MainWindow::InitComponent(const MainWindowComponent& InCompo){
    LOGMSG_INFO("IN");
    m_Compo = InCompo;
    LOGMSG_INFO("OUT");
}

void MainWindow::SwitchUIStatus_Init(){
    LOGMSG_INFO("IN");
    // handle Menu Action
    ui->actionOpen_File_Menu->setEnabled(true);
    ui->actionOpen_Folder_Menu->setEnabled(true);
    ui->actionSave_Config_Menu->setEnabled(true);
    ui->actionLoad_Config_Menu->setEnabled(true);
    ui->actionScheduler_Menu->setEnabled(true);
    // handle ToolBar Action
    ui->actionOpen_File_ToolBar->setEnabled(true);
    ui->actionOpen_Folder_ToolBar->setEnabled(true);
    ui->actionSave_Config_ToolBar->setEnabled(true);
    ui->actionLoad_Config_ToolBar->setEnabled(true);
    ui->actionScheduler_ToolBar->setEnabled(true);
    // hanlde Push Button
    ui->BtnPlay->setEnabled(true);
    ui->BtnPause->setEnabled(false);
    ui->BtnStop->setEnabled(false);
    ui->BtnRemove->setEnabled(false);
    ui->BtnRemoveAll->setEnabled(false);
    ui->BtnRegularPlay->setEnabled(true);
    ui->BtnAddSrcMap->setEnabled(true);
    ui->BtnAddDstMap->setEnabled(true);
    ui->BtnRemoveScrIPMap->setEnabled(true);
    ui->BtnRemoveDstIPMap->setEnabled(true);
    // handle Line Edit
    ui->lineEdit_SpeedFactor->setEnabled(true);
    ui->lineEdit_LoopCount->setEnabled(true);
    ui->lineEdit_SpeedLimit->setEnabled(true);
    // handle Combo Box
    ui->comboBox_InterfaceList->setEnabled(true);
    // handle List Widget
    ui->listWidget_FileList->setEnabled(true);
    // handle Table Widget
    ui->tableWidget_NetMapDst->setEnabled(true);
    ui->tableWidget_NetMapSrc->setEnabled(true);
    // handle Progress Bar
    ui->progressBar->setValue(0);
    // handle Label
    ui->LabelSchedulerStatus->setText("");
    LOGMSG_INFO("OUT");
}

void MainWindow::SwitchUIStatus_Play(){
    LOGMSG_INFO("IN");
    // handle Menu Action
    ui->actionOpen_File_Menu->setEnabled(false);
    ui->actionOpen_Folder_Menu->setEnabled(false);
    ui->actionSave_Config_Menu->setEnabled(false);
    ui->actionLoad_Config_Menu->setEnabled(false);
    ui->actionScheduler_Menu->setEnabled(false);
    // handle ToolBar Action
    ui->actionOpen_File_ToolBar->setEnabled(false);
    ui->actionOpen_Folder_ToolBar->setEnabled(false);
    ui->actionSave_Config_ToolBar->setEnabled(false);
    ui->actionLoad_Config_ToolBar->setEnabled(false);
    ui->actionScheduler_ToolBar->setEnabled(false);
    // hanlde Push Button
    ui->BtnPlay->setEnabled(false);
    ui->BtnPause->setEnabled(true);
    ui->BtnStop->setEnabled(true);
    ui->BtnRemove->setEnabled(false);
    ui->BtnRemoveAll->setEnabled(false);
    ui->BtnRegularPlay->setEnabled(false);
    ui->BtnAddSrcMap->setEnabled(false);
    ui->BtnAddDstMap->setEnabled(false);
    ui->BtnRemoveScrIPMap->setEnabled(false);
    ui->BtnRemoveDstIPMap->setEnabled(false);
    // handle Line Edit
    ui->lineEdit_SpeedFactor->setEnabled(false);
    ui->lineEdit_LoopCount->setEnabled(false);
    ui->lineEdit_SpeedLimit->setEnabled(false);
    // handle Combo Box
    ui->comboBox_InterfaceList->setEnabled(false);
    // handle List Widget
    ui->listWidget_FileList->setEnabled(false);
    // handle Table Widget
    ui->tableWidget_NetMapDst->setEnabled(false);
    ui->tableWidget_NetMapSrc->setEnabled(false);
    // handle Progress Bar
    ui->progressBar->setValue(0);
    LOGMSG_INFO("OUT");
}

void MainWindow::SwitchUIStatus_Pause(){
    LOGMSG_INFO("IN");
    SwitchUIStatus_Play();
    // handle Difference
    ui->BtnPlay->setEnabled(true);
    ui->BtnPause->setEnabled(false);
    LOGMSG_INFO("OUT");
}

void MainWindow::SwitchUIStatus_Stop(){
    LOGMSG_INFO("IN");
    SwitchUIStatus_Init();
    LOGMSG_INFO("OUT");
}

void MainWindow::AddPcapFilesToUI(const QStringList& INFiles){
    LOGMSG_INFO("IN");
    // handle UI
    // ui->listWidget_FileList->clear();
    if (INFiles.isEmpty()){
        LOGMSG_INFO("OUT");
        return;
    }

    // handle config
    QFileInfo fileInfo(INFiles.front());
    m_Compo.pConfig->SetLatestFilePath(ConvertQString2String(fileInfo.filePath()));

    // handle UI
    for (qint32 i = 0; i < INFiles.length(); i++){
        if (IsFileExists(INFiles[i])){
            ui->listWidget_FileList->addItem(INFiles[i]);
            ui->BtnRemove->setEnabled(true);
            ui->BtnRemoveAll->setEnabled(true);
        }
    }
    ui->listWidget_FileList->setCurrentRow(0);
    LOGMSG_INFO("OUT");
}

std::string MainWindow::ConvertQString2String(const QString& qstr){
    LOGMSG_INFO("IN");
    LOGMSG_INFO("OUT");
    // for unix
    return qstr.toUtf8().constData();
    // for windows
    // return qstr.toLocal8Bit().constData(); // we get the current locale for the current system
}

QString MainWindow::ConvertString2QString(const std::string& str){
    LOGMSG_INFO("IN");
    LOGMSG_INFO("OUT");
    return QString::fromStdString(str);
}

std::vector<std::string> MainWindow::ConvertQStringList(const QStringList& qstrList){
    LOGMSG_INFO("IN");
    std::vector<std::string> result;
    for (qint32 i = 0; i < qstrList.length(); i++){
        result.push_back(ConvertQString2String(qstrList[i]));
    }
    LOGMSG_INFO("OUT");
    return result;
}

QStringList MainWindow::ConvertVectorString(const std::vector<std::string>& vecStr){
    LOGMSG_INFO("IN");
    QStringList result;
    for (size_t i = 0; i < vecStr.size(); i++){
        result += ConvertString2QString(vecStr[i]);
    }
    LOGMSG_INFO("OUT");
    return result;
}

bool MainWindow::IsFileExists(const QString& qstrPath){
    LOGMSG_INFO("IN");
    QFileInfo check_file(qstrPath);
    LOGMSG_INFO("OUT");
    return (check_file.exists() && check_file.isFile());
}

void MainWindow::onOpen_File(){
    LOGMSG_INFO("IN");
    QStringList fileNames; // fileNames contains full path with file name
    if (m_Compo.pConfig->GetLatestFilePath() == "")
        fileNames = QFileDialog::getOpenFileNames(this,
                                                  tr("Open Pcap"),
                                                  m_strCurAppPath,
                                                  tr("Pcap Files (*.pcap)"));
    else
        fileNames = QFileDialog::getOpenFileNames(this,
                                                  tr("Open Pcap"),
                                                  ConvertString2QString(m_Compo.pConfig->GetLatestFilePath()),
                                                  tr("Pcap Files (*.pcap)"));

    if (fileNames.length() == 0){
        LOGMSG_INFO("OUT");
        return;
    }
    AddPcapFilesToUI(fileNames);

    m_Compo.pConfig->AddPcapFiles(ConvertQStringList(fileNames));
    LOGMSG_INFO("OUT");
}

void MainWindow::onOpen_Folder(){
    LOGMSG_INFO("IN");
    QString strFilePath;
    if (m_Compo.pConfig->GetLatestFilePath() == "")
        strFilePath = QFileDialog::getExistingDirectory(this,
                                                        "",
                                                        m_strCurAppPath,
                                                        QFileDialog::ShowDirsOnly);
    else
        strFilePath = QFileDialog::getExistingDirectory(this,
                                                        "",
                                                        ConvertString2QString(m_Compo.pConfig->GetLatestFilePath()),
                                                        QFileDialog::ShowDirsOnly);

    if (strFilePath == ""){
        LOGMSG_INFO("OUT");
        return;
    }

    //QDirIterator it(m_strLatestFilePath, QStringList() << "*.pcap", QDir::Files, QDirIterator::Subdirectories);
    QDirIterator it(strFilePath, QStringList() << "*.pcap", QDir::Files, QDirIterator::NoIteratorFlags);

    QStringList fileNames;
    while (it.hasNext()){
        it.next();
        QString line = it.filePath();
        fileNames.push_back(line);
    }

    AddPcapFilesToUI(fileNames);
    m_Compo.pConfig->AddPcapFiles(ConvertQStringList(fileNames));
    LOGMSG_INFO("OUT");
}

void MainWindow::onSave_Config(){
    LOGMSG_INFO("IN");
    QMessageBox TestingBox;
    TestingBox.setWindowTitle(QString("Wait for input!"));
    TestingBox.show();
    TestingBox.exec();
    LOGMSG_INFO("OUT");
}

void MainWindow::onLoad_Config(){
    LOGMSG_INFO("IN");
    QMessageBox TestingBox;
    TestingBox.setWindowTitle(QString("Wait for input!"));
    TestingBox.show();
    TestingBox.exec();
    LOGMSG_INFO("OUT");
}

void MainWindow::onScheduler(){
    LOGMSG_INFO("IN");
    QMessageBox TestingBox;
    TestingBox.setWindowTitle(QString("Wait for input!"));
    TestingBox.show();
    TestingBox.exec();
    LOGMSG_INFO("OUT");
}

void MainWindow::onPlay(){
    LOGMSG_INFO("IN");
    SwitchUIStatus_Play();
    LOGMSG_INFO("OUT");
}

void MainWindow::onPause(){
    LOGMSG_INFO("IN");
    SwitchUIStatus_Pause();
    LOGMSG_INFO("OUT");
}

void MainWindow::onStop(){
    LOGMSG_INFO("IN");
    SwitchUIStatus_Stop();
    LOGMSG_INFO("OUT");
}

void MainWindow::onRemove(){
    LOGMSG_INFO("IN");
    int nCurIdx = ui->listWidget_FileList->currentRow();
    QListWidgetItem* pItem = ui->listWidget_FileList->takeItem(nCurIdx);
    delete pItem;
    if (ui->listWidget_FileList->count() == 0){
        // handle UI
        ui->BtnRemove->setEnabled(false);
        ui->BtnRemoveAll->setEnabled(false);
    }
    m_Compo.pConfig->RemovePcapFile(nCurIdx);
    LOGMSG_INFO("OUT");
}

void MainWindow::onRemoveAll(){
    LOGMSG_INFO("IN");
    ui->listWidget_FileList->clear();
    // handle UI
    ui->BtnRemove->setEnabled(false);
    ui->BtnRemoveAll->setEnabled(false);

    m_Compo.pConfig->RemoveAllPcapFile();
    LOGMSG_INFO("OUT");
}

void MainWindow::onRegularPaly(){
    LOGMSG_INFO("IN");
    SwitchUIStatus_Play();
    LOGMSG_INFO("OUT");
}

void MainWindow::onAddSrcMap(){
    LOGMSG_INFO("IN");
    QMessageBox TestingBox;
    TestingBox.setWindowTitle(QString("Wait for input!"));
    TestingBox.show();
    TestingBox.exec();
    LOGMSG_INFO("OUT");
}

void MainWindow::onAddDstMap(){
    LOGMSG_INFO("IN");
    QMessageBox TestingBox;
    TestingBox.setWindowTitle(QString("Wait for input!"));
    TestingBox.show();
    TestingBox.exec();
    LOGMSG_INFO("OUT");
}

void MainWindow::onRemoveScrMapIP(){
    LOGMSG_INFO("IN");
    QMessageBox TestingBox;
    TestingBox.setWindowTitle(QString("Wait for input!"));
    TestingBox.show();
    TestingBox.exec();
    LOGMSG_INFO("OUT");
}

void MainWindow::onRemoveDstMapIP(){
    LOGMSG_INFO("IN");
    QMessageBox TestingBox;
    TestingBox.setWindowTitle(QString("Wait for input!"));
    TestingBox.show();
    TestingBox.exec();
    LOGMSG_INFO("OUT");
}
