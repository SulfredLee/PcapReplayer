#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Config.h"
#include "LogMgr.h"

#include <sstream>
#include <stdio.h>

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
    // handle comboBox
    connect(ui->comboBox_InterfaceList, SIGNAL(activated(int)), this, SLOT(onSelectInterface(int)));
    // handle callback from PlayerCtrl
    connect(this, SIGNAL(onPlay_FromPlayerCtrl()), this, SLOT(onPlay()));
    connect(this, SIGNAL(onPause_FromPlayerCtrl()), this, SLOT(onPause()));
    connect(this, SIGNAL(onStop_FromPlayerCtrl()), this, SLOT(onStop()));
    connect(this, SIGNAL(onProgressBar_FromPlayerCtrl(int)), this, SLOT(onProgressBar(int)));
    connect(this, SIGNAL(onStatusBar_SentByte_FromPlayerCtrl(int)), this, SLOT(onStatusBar_SentByte(int)));
    connect(this, SIGNAL(onStatusBar_CurPktTime_FromPlayerCtrl(double)), this, SLOT(onStatusBar_CurPktTime(double)));
    connect(this, SIGNAL(onStatusBar_RemainLoopCount_FromPlayerCtrl(int)), this, SLOT(onStatusBar_RemainLoopCount(int)));
    connect(this, SIGNAL(onStatusBar_Invalidate_FromPlayerCtrl()), this, SLOT(onStatusBar_Invalidate()));

    m_qstrCurAppPath = QDir::currentPath();

    // handle validator
    ui->lineEdit_SpeedFactor->setValidator(new QDoubleValidator(0, 40000, 4, this));
    ui->lineEdit_SpeedLimit->setValidator(new QDoubleValidator(-1, 40000, 4, this));
    ui->lineEdit_LoopCount->setValidator(new QIntValidator(-1, 100, this));

    // handle UI status
    SwitchUIStatus_Init();
    ui->lineEdit_SpeedFactor->insert("1.0");
    ui->lineEdit_SpeedLimit->insert("-1.0");
    ui->lineEdit_LoopCount->insert("-1");

    // handle tableWidget
    ui->tableWidget_NetMapDst->clear();
    ui->tableWidget_NetMapDst->setColumnCount(2);
    ui->tableWidget_NetMapDst->setHorizontalHeaderItem(0, new QTableWidgetItem(QString("Original Dst IP")));
    ui->tableWidget_NetMapDst->setHorizontalHeaderItem(1, new QTableWidgetItem(QString("Converted Dst IP")));
    ui->tableWidget_NetMapDst->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableWidget_NetMapSrc->clear();
    ui->tableWidget_NetMapSrc->setColumnCount(2);
    ui->tableWidget_NetMapSrc->setHorizontalHeaderItem(0, new QTableWidgetItem(QString("Original Src IP")));
    ui->tableWidget_NetMapSrc->setHorizontalHeaderItem(1, new QTableWidgetItem(QString("Converted Src IP")));
    ui->tableWidget_NetMapSrc->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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

    // handle config
    // handle UI
    ui->comboBox_InterfaceList->clear();
    auto vecTemp = m_Compo.pConfig->GetInterfaceInfo();
    for (size_t i = 0; i < vecTemp.size(); i++){
        ui->comboBox_InterfaceList->addItem(ConvertString2QString(vecTemp[i]));
        ui->comboBox_InterfaceList->setCurrentIndex(0);
    }
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
    if (ui->listWidget_FileList->count() == 0){
        ui->BtnRemove->setEnabled(false);
        ui->BtnRemoveAll->setEnabled(false);
    }
    else{
        ui->BtnRemove->setEnabled(true);
        ui->BtnRemoveAll->setEnabled(true);
    }
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

void MainWindow::SwitchUIStatus_Play_Pause(){
    LOGMSG_INFO("IN");
    // handle any status to Play or Pause
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
    ui->BtnStop->setEnabled(true);
    ui->BtnRemove->setEnabled(false);
    ui->BtnRemoveAll->setEnabled(false);
    ui->BtnRegularPlay->setEnabled(false);
    ui->BtnAddSrcMap->setEnabled(false);
    ui->BtnAddDstMap->setEnabled(false);
    ui->BtnRemoveScrIPMap->setEnabled(false);
    ui->BtnRemoveDstIPMap->setEnabled(false);
    // handle Combo Box
    ui->comboBox_InterfaceList->setEnabled(false);
    // handle List Widget
    ui->listWidget_FileList->setEnabled(false);
    // handle Table Widget
    ui->tableWidget_NetMapDst->setEnabled(false);
    ui->tableWidget_NetMapSrc->setEnabled(false);

    // handle Difference between Pause and Play
    if (m_Compo.pConfig->GetPlayerStatus() == PlayerStatus::Stop // handle from Stop to Play
        || m_Compo.pConfig->GetPlayerStatus() == PlayerStatus::Pause){ // handle from Pause to Play
        // handle Line Edit
        ui->lineEdit_SpeedFactor->setEnabled(false);
        ui->lineEdit_LoopCount->setEnabled(false);
        ui->lineEdit_SpeedLimit->setEnabled(false);
        // hanlde Push Button
        ui->BtnPlay->setEnabled(false);
        ui->BtnPause->setEnabled(true);
        if (m_Compo.pConfig->GetPlayerStatus() == PlayerStatus::Stop){ // handle from Stop to Play
            // handle Progress Bar
            ui->progressBar->setValue(0);
        }
    } else if (m_Compo.pConfig->GetPlayerStatus() == PlayerStatus::Play){ // handle from Play to Pause
        // handle Line Edit
        ui->lineEdit_SpeedFactor->setEnabled(true);
        ui->lineEdit_LoopCount->setEnabled(true);
        ui->lineEdit_SpeedLimit->setEnabled(true);
        // hanlde Push Button
        ui->BtnPlay->setEnabled(true);
        ui->BtnPause->setEnabled(false);
    }
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

void MainWindow::GetBitPerSec(double bit, QString& line, int step){
    if (step >= 4)
        return;

    line = QString::number(bit);
    switch (step)
        {
        case 0:
            line += " Bit/sec";
            break;
        case 1:
            line += " KBit/sec";
            break;
        case 2:
            line += " MBit/sec";
            break;
        case 3:
            line += " GBit/sec";
            break;
        default:
            break;
        }

    int newBit = bit / 1000;
    if (newBit > 0)
        GetBitPerSec(bit / 1000, line, step + 1);
}

QMap<QString, QString> MainWindow::GetMapFromNetworkTable(QTableWidget const * inTable){
    QMap<QString, QString> mapTemp;
    for (int nRow = 0; nRow < inTable->rowCount(); nRow++)
        {
            QTableWidgetItem * itemOriginal = inTable->item(nRow, 0);
            QTableWidgetItem * itemConverted = inTable->item(nRow, 1);
            QString strOriginalIP = itemOriginal->text();
            QString strConvertedIP = itemConverted->text();
            mapTemp.insert(strOriginalIP, strConvertedIP);
        }
    return mapTemp;
}

std::string MainWindow::ConvertQString2String(const QString& qstr){
#ifdef WINDOWS
    return qstr.toLocal8Bit().constData(); // we get the current locale for the current system
#else
    return qstr.toUtf8().constData();
#endif
}

QString MainWindow::ConvertString2QString(const std::string& str){
    return QString::fromStdString(str);
}

std::vector<std::string> MainWindow::ConvertQStringList(const QStringList& qstrList){
    LOGMSG_DEBUG("IN");
    std::vector<std::string> result;
    for (qint32 i = 0; i < qstrList.length(); i++){
        result.push_back(ConvertQString2String(qstrList[i]));
    }
    LOGMSG_DEBUG("OUT");
    return result;
}

QStringList MainWindow::ConvertVectorString(const std::vector<std::string>& vecStr){
    LOGMSG_DEBUG("IN");
    QStringList result;
    for (size_t i = 0; i < vecStr.size(); i++){
        result += ConvertString2QString(vecStr[i]);
    }
    LOGMSG_DEBUG("OUT");
    return result;
}

bool MainWindow::IsFileExists(const QString& qstrPath){
    LOGMSG_INFO("IN");
    QFileInfo check_file(qstrPath);
    LOGMSG_INFO("OUT");
    return (check_file.exists() && check_file.isFile());
}

QString MainWindow::ConvertTime2QString(double dTime){
    struct timeval tv;
    time_t nowtime;
    struct tm *nowtm;
    char tmbuf[64], buf[64];

    nowtime = (int)dTime;
    nowtm = localtime(&nowtime);
    strftime(tmbuf, sizeof tmbuf, "%Y-%m-%d,%H:%M:%S", nowtm);
#ifdef WINDOWS
    sprintf_s(buf, sizeof buf, "%s.%06d", tmbuf, (long)((dTime - (unsigned int)dTime) * 1000000));
#else
    sprintf(buf, "%s.%06d", tmbuf, (long)((dTime - (unsigned int)dTime) * 1000000));
#endif

    return QString(buf);
}

std::map<std::string, std::string> MainWindow::ConvertQMap2StdMap(const QMap<QString, QString>& inMap){
    std::map<std::string, std::string> outMap;
    for (QMap<QString, QString>::const_iterator it = inMap.begin();
         it != inMap.end(); it++)        {
        std::string strKey = it.key().toUtf8().constData();
        std::string strValue = it.value().toUtf8().constData();
        outMap.insert(std::make_pair(strKey, strValue));
    }
    return outMap;
}

void MainWindow::onOpen_File(){
    LOGMSG_INFO("IN");
    QStringList fileNames; // fileNames contains full path with file name
    if (m_Compo.pConfig->GetLatestFilePath() == "")
        fileNames = QFileDialog::getOpenFileNames(this,
                                                  tr("Open Pcap"),
                                                  m_qstrCurAppPath,
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
                                                        m_qstrCurAppPath,
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

    if (m_Compo.pConfig->GetPlayerStatus() == PlayerStatus::Play){
        LOGMSG_INFO("OUT");
        return;
    }

    // handle UI
    SwitchUIStatus_Play_Pause();

    // handle Config
    m_Compo.pConfig->SetSpeedFactor(ui->lineEdit_SpeedFactor->text().toDouble());
    m_Compo.pConfig->SetSpeedLimit(ui->lineEdit_SpeedLimit->text().toDouble() * 1000 * 1000);
    m_Compo.pConfig->SetLoopCount(ui->lineEdit_LoopCount->text().toInt());
    m_Compo.pConfig->SetMapDstIP(ConvertQMap2StdMap(GetMapFromNetworkTable(ui->tableWidget_NetMapDst)));
    m_Compo.pConfig->SetMapSrcIP(ConvertQMap2StdMap(GetMapFromNetworkTable(ui->tableWidget_NetMapSrc)));

    // handle Ctrl
    auto p = boost::make_shared<PlayerMsg>();
    *p = PlayerMsg::Play;
    m_Compo.pMsgQ->push(p);

    LOGMSG_INFO("OUT");
}

void MainWindow::onPause(){
    LOGMSG_INFO("IN");

    if (m_Compo.pConfig->GetPlayerStatus() == PlayerStatus::Pause){
        LOGMSG_INFO("OUT");
        return;
    }

    SwitchUIStatus_Play_Pause();

    auto p = boost::make_shared<PlayerMsg>();
    *p = PlayerMsg::Pause;
    m_Compo.pMsgQ->push(p);

    LOGMSG_INFO("OUT");
}

void MainWindow::onStop(){
    LOGMSG_INFO("IN");

    if (m_Compo.pConfig->GetPlayerStatus() == PlayerStatus::Stop){
        LOGMSG_INFO("OUT");
        return;
    }

    SwitchUIStatus_Stop();

    auto p = boost::make_shared<PlayerMsg>();
    *p = PlayerMsg::Stop;
    m_Compo.pMsgQ->push(p);

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
    SwitchUIStatus_Play_Pause();
    LOGMSG_INFO("OUT");
}

void MainWindow::onAddSrcMap(){
    LOGMSG_INFO("IN");
    ui->tableWidget_NetMapSrc->insertRow(ui->tableWidget_NetMapSrc->rowCount());
    LOGMSG_INFO("OUT");
}

void MainWindow::onAddDstMap(){
    LOGMSG_INFO("IN");
    ui->tableWidget_NetMapDst->insertRow(ui->tableWidget_NetMapDst->rowCount());
    LOGMSG_INFO("OUT");
}

void MainWindow::onRemoveScrMapIP(){
    LOGMSG_INFO("IN");
    int nCurRow = ui->tableWidget_NetMapSrc->currentRow();
    if (nCurRow == -1) {
        return;
    }
    ui->tableWidget_NetMapSrc->removeRow(nCurRow);

//  QMessageBox TestingBox;
//  TestingBox.setWindowTitle(QString("Wait for input!"));
//  TestingBox.show();
//  TestingBox.exec();
    LOGMSG_INFO("OUT");
}

void MainWindow::onRemoveDstMapIP(){
    LOGMSG_INFO("IN");
    int nCurRow = ui->tableWidget_NetMapDst->currentRow();
    if (nCurRow == -1) {
        return;
    }
    ui->tableWidget_NetMapDst->removeRow(nCurRow);

    LOGMSG_INFO("OUT");
}

void MainWindow::onSelectInterface(int nSelect){
    m_Compo.pConfig->SetAdapterIdx(nSelect);
}

void MainWindow::onProgressBar(int nValue){
    ui->progressBar->setValue(nValue);
}

void MainWindow::onStatusBar_SentByte(int nSentByte){
    GetBitPerSec(nSentByte * 8, m_qstrSentByte, 0);
    // LOGMSG_INFO(ConvertQString2String(m_qstrSentByte));
}

void MainWindow::onStatusBar_CurPktTime(double dCurPktTime){
    m_qstrCurPktTime = ConvertTime2QString(dCurPktTime);
}

void MainWindow::onStatusBar_RemainLoopCount(int nRemainLoopCount){
    m_qstrRemainLoopCount = "Remain Loop: " + QString::number(nRemainLoopCount);
}

void MainWindow::onStatusBar_Invalidate(){
    QString qstrTempLine;
    qstrTempLine = m_qstrRemainLoopCount + "    " + m_qstrSentByte + "    " + m_qstrCurPktTime;
    ui->statusbar->showMessage(qstrTempLine);
}
