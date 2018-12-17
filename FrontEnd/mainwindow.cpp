#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Config.h"
#include "LogMgr.h"

#include <sstream>
#include <stdio.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_Schedulerdialog(this){
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
    connect(this, SIGNAL(onPlay_FromMainWindow()), this, SLOT(onPlay()));
    connect(this, SIGNAL(onPause_FromPlayerCtrl()), this, SLOT(onPause()));
    connect(this, SIGNAL(onStop_FromPlayerCtrl()), this, SLOT(onStop()));
    connect(this, SIGNAL(onProgressBar_FromPlayerCtrl(int)), this, SLOT(onProgressBar(int)));
    connect(this, SIGNAL(onStatusBar_SentByte_FromPlayerCtrl(int)), this, SLOT(onStatusBar_SentByte(int)));
    connect(this, SIGNAL(onStatusBar_CurPktTime_FromPlayerCtrl(double)), this, SLOT(onStatusBar_CurPktTime(double)));
    connect(this, SIGNAL(onStatusBar_RemainLoopCount_FromPlayerCtrl(int)), this, SLOT(onStatusBar_RemainLoopCount(int)));
    connect(this, SIGNAL(onStatusBar_SendTimeDiff_FromPlayerCtrl(double)), this, SLOT(onStatusBar_SendTimeDiff(double)));
    connect(this, SIGNAL(onStatusBar_Invalidate_FromPlayerCtrl()), this, SLOT(onStatusBar_Invalidate()));
    connect(this, SIGNAL(onSerialization_FromPlayerCtrl(bool)), this, SLOT(onSerialization(bool)));
    connect(this, SIGNAL(onListWidgetNextFile_FromPlayerCtrl()), this, SLOT(onListWidgetNextFile()));

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
    m_qstrRemainLoopCount = "Remain Loop: 0";

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

    m_pDailyTimer = nullptr;

    setWindowTitle("PcapReplayer 2.0.0.15");
    setWindowIcon(QIcon(":/app_icon.png"));

    // init file drop
    setAcceptDrops(true);

    LOGMSG_INFO("OUT");
}

MainWindow::~MainWindow(){
    LOGMSG_INFO("IN");
    if (m_pDailyTimer != nullptr){
        delete m_pDailyTimer;
        m_pDailyTimer = nullptr;
    }
    LOGMSG_INFO("OUT");
    delete ui;
}

void MainWindow::InitComponent(const MainWindowComponent& InCompo){
    LOGMSG_INFO("IN");
    m_Compo = InCompo;

    // handle config
    onSerialization(false);
    m_Compo.pConfig->SetLatestFilePath(ConvertQString2String(m_qstrCurAppPath));
    // handle UI
    ui->comboBox_InterfaceList->clear();
    auto vecTemp = m_Compo.pConfig->GetInterfaceInfo();
    for (size_t i = 0; i < vecTemp.size(); i++){
        ui->comboBox_InterfaceList->addItem(ConvertString2QString(vecTemp[i]));
        ui->comboBox_InterfaceList->setCurrentIndex(0);
    }

    std::vector<std::string> vecTempPcapFiles = m_Compo.pConfig->GetPcapFiles();
    m_Compo.pConfig->RemoveAllPcapFile();
    AddPcapFilesToUI(ConvertVectorString(vecTempPcapFiles));
    AddIPMapToUI(ConvertStdMap2QMap(m_Compo.pConfig->GetMapSrcIP())
                 , ConvertStdMap2QMap(m_Compo.pConfig->GetMapDstIP()));
    AddSchedulToUI();
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
    m_Compo.pConfig->AddPcapFiles(ConvertQStringList(INFiles));

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

void MainWindow::AddIPMapToUI(const QMap<QString, QString>& SrcMap
                              , const QMap<QString, QString>& DstMap){
    int nRow = 0;
    ui->tableWidget_NetMapSrc->setRowCount(SrcMap.size());
    for (auto it = SrcMap.begin(); it != SrcMap.end(); it++){
        ui->tableWidget_NetMapSrc->setItem(nRow, 0, new QTableWidgetItem(it.key()));
        ui->tableWidget_NetMapSrc->setItem(nRow, 1, new QTableWidgetItem(it.value()));
        nRow++;
    }
    nRow = 0;
    ui->tableWidget_NetMapDst->setRowCount(DstMap.size());
    for (auto it = DstMap.begin(); it != DstMap.end(); it++){
        ui->tableWidget_NetMapDst->setItem(nRow, 0, new QTableWidgetItem(it.key()));
        ui->tableWidget_NetMapDst->setItem(nRow, 1, new QTableWidgetItem(it.value()));
        nRow++;
    }
}

void MainWindow::AddSchedulToUI(){
    LOGMSG_INFO("IN");
    if (m_Compo.pConfig->GetSchedulerEnable()) {
        QString qstrSchedulerStatus = "";
        if (m_Compo.pConfig->GetOneTimeOnly()) {
            m_pDailyTimer = new DailyTimer(m_Compo.pConfig->GetDateTime()
                                           , boost::bind(&MainWindow::DailyTimerCallback, this));
            // handle UI Label information
            boost::posix_time::ptime tempTime = m_Compo.pConfig->GetDateTime();
            qstrSchedulerStatus = "Scheduler On : ";
            std::stringstream ssTempLine;
            ssTempLine << tempTime.date().year() << "/"
                       << tempTime.date().month() << "/"
                       << tempTime.date().day() << " ";
            qstrSchedulerStatus += ConvertString2QString(ssTempLine.str());
        }else{
            boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
            boost::posix_time::ptime newTime(now.date()
                                             , m_Compo.pConfig->GetDateTime().time_of_day());
            if (newTime < now) {
                boost::gregorian::date_duration dd(1);
                boost::posix_time::ptime advanceTime(newTime.date() + dd
                                                     , newTime.time_of_day());
                newTime = advanceTime;
            }
            m_pDailyTimer = new DailyTimer(newTime
                                           , boost::bind(&MainWindow::DailyTimerCallback, this));
            // handle UI Label information
            qstrSchedulerStatus = "Weekly Scheduler On : ";
            std::vector<bool> vecTemp = m_Compo.pConfig->GetSchedulerDay();
            for (size_t i = 0; i < vecTemp.size(); ++i) {
                if (vecTemp[i]) {
                    switch (i) {
                    case 0: {
                        qstrSchedulerStatus += "Sunday ";
                        break;
                    }
                    case 1: {
                        qstrSchedulerStatus += "Monday ";
                        break;
                    }
                    case 2: {
                        qstrSchedulerStatus += "Tuesdyay ";
                        break;
                    }
                    case 3: {
                        qstrSchedulerStatus += "Wednesday ";
                        break;
                    }
                    case 4: {
                        qstrSchedulerStatus += "Thrusday ";
                        break;
                    }
                    case 5: {
                        qstrSchedulerStatus += "Friday ";
                        break;
                    }
                    case 6: {
                        qstrSchedulerStatus += "Saturday ";
                        break;
                    }
                    default:
                        break;
                    }
                }
            }
        }
        // handle UI Label information
        boost::posix_time::ptime tempTime = m_Compo.pConfig->GetDateTime();
        std::stringstream ssTempLine;
        ssTempLine << tempTime.time_of_day().hours() << ":"
                   << tempTime.time_of_day().minutes() << ":"
                   << tempTime.time_of_day().seconds();
        qstrSchedulerStatus += ConvertString2QString(ssTempLine.str());
        ui->LabelSchedulerStatus->setText(qstrSchedulerStatus);
        ui->LabelSchedulerStatus->setStyleSheet("QLabel {background-color : green; color : yellow; }");
    }else{
        // handle UI Label information
        ui->LabelSchedulerStatus->setText("Scheduler Disabled");
        ui->LabelSchedulerStatus->setStyleSheet("QLabel {background-color : red; color : blue;}");
    }
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

void MainWindow::DailyTimerCallback(){
    LOGMSG_INFO("IN");
    if (m_Compo.pConfig->GetOneTimeOnly()) {
        emit onPlay_FromMainWindow();
    }else{
        std::vector<bool> vecWeek = m_Compo.pConfig->GetSchedulerDay();
        namespace namePT = boost::posix_time;
        namePT::ptime now = namePT::second_clock::local_time();
        if (vecWeek[now.date().day_of_week().as_number()]) { // if today need replay
            emit onPlay_FromMainWindow();
        }

        std::stringstream ssTempLine;
        ssTempLine << now.date().day_of_week()
                   << " " << now.date().day_of_week().as_number(); // Sunday: 0, Monday: 1, ... , Saturday: 6
        LOGMSG_INFO(ssTempLine.str());
    }
    LOGMSG_INFO("OUT");
}

// override
void MainWindow::dragEnterEvent(QDragEnterEvent* event){
    LOGMSG_INFO("IN");
    // if some actions should not be usable, like move, this code must be adopted
    event->acceptProposedAction();
    LOGMSG_INFO("OUT");
}

// override
void MainWindow::dragMoveEvent(QDragMoveEvent* event){
    LOGMSG_INFO("IN");
    // if some actions should not be usable, like move, this code must be adopted
    event->acceptProposedAction();
    LOGMSG_INFO("OUT");
}

// override
void MainWindow::dragLeaveEvent(QDragLeaveEvent* event){
    LOGMSG_INFO("IN");
    // if some actions should not be usable, like move, this code must be adopted
    event->accept();
    LOGMSG_INFO("OUT");
}

// override
void MainWindow::dropEvent(QDropEvent* event){
    LOGMSG_INFO("IN");
    const QMimeData* mimeData = event->mimeData();

    if (mimeData->hasUrls()){
        QStringList pathList;
        QList<QUrl> urlList = mimeData->urls();

        for (int i = 0; i < urlList.size(); ++i){
            QString fileName = urlList.at(i).toLocalFile();
            if (fileName.endsWith("pcap", Qt::CaseInsensitive))
                pathList.append(urlList.at(i).toLocalFile());
        }
        AddPcapFilesToUI(pathList);
    }
    LOGMSG_INFO("OUT");
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

QMap<QString, QString> MainWindow::ConvertStdMap2QMap(const std::map<std::string, std::string>& inMap){
    QMap<QString, QString> outMap;
    for (auto it = inMap.begin(); it != inMap.end(); it++){
        QString qstrKey = ConvertString2QString(it->first);
        QString qstrValue = ConvertString2QString(it->second);
        outMap.insert(qstrKey, qstrValue);
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
    LOGMSG_INFO("OUT");
}

void MainWindow::onSave_Config(){
    LOGMSG_INFO("IN");
    QString qstrConfigFileName;
    if (m_Compo.pConfig->GetConfigPath() == ""){
        qstrConfigFileName = QFileDialog::getSaveFileName(this
                                                          , tr("Save Config")
                                                          , m_qstrCurAppPath
                                                          , tr("Config Files (*.cfg)"));
    }else{
        qstrConfigFileName = QFileDialog::getSaveFileName(this
                                                          , tr("Save Config")
                                                          , ConvertString2QString(m_Compo.pConfig->GetConfigPath())
                                                          , tr("Config Files (*.cfg)"));
    }
    if (qstrConfigFileName.length() == 0) {
        return;
    }

    QFileInfo fileInfo(qstrConfigFileName);
    m_Compo.pConfig->SetConfigPath(ConvertQString2String(fileInfo.filePath()));

    onSerialization(true);
    //    QMessageBox TestingBox;
    //    TestingBox.setWindowTitle(QString("Wait for input!"));
    //    TestingBox.show();
    //    TestingBox.exec();
    LOGMSG_INFO("OUT");
}

void MainWindow::onLoad_Config(){
    LOGMSG_INFO("IN");
    QString qstrConfigFileName;
    if (m_Compo.pConfig->GetConfigPath() == "") {
        qstrConfigFileName = QFileDialog::getOpenFileName(this
                                                          , tr("Load Config")
                                                          , m_qstrCurAppPath
                                                          , tr("Config File (*.cfg)"));
    }else {
        qstrConfigFileName = QFileDialog::getOpenFileName(this
                                                          , tr("Load Config")
                                                          , ConvertString2QString(m_Compo.pConfig->GetConfigPath())
                                                          , tr("Config File (*.cfg)"));
    }
    if (qstrConfigFileName.length() == 0){
        return;
    }

    QFileInfo fileInfo(qstrConfigFileName);
    m_Compo.pConfig->SetConfigPath(ConvertQString2String(fileInfo.filePath()));

    onSerialization(true);
    //    QMessageBox TestingBox;
    //    TestingBox.setWindowTitle(QString("Wait for input!"));
    //    TestingBox.show();
    //    TestingBox.exec();
    LOGMSG_INFO("OUT");
}

void MainWindow::onScheduler(){
    LOGMSG_INFO("IN");
    m_Schedulerdialog.setModal(true);
    m_Schedulerdialog.InitComponent(m_Compo.pConfig);
    m_Schedulerdialog.show();
    m_Schedulerdialog.exec();

    if (m_pDailyTimer != nullptr){
        delete m_pDailyTimer;
        m_pDailyTimer = nullptr;
    }

    AddSchedulToUI();

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
    AddSchedulToUI();

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
    if (nRemainLoopCount < 0) {
        m_qstrRemainLoopCount = "Remain Loop: 0";
    }else{
        m_qstrRemainLoopCount = "Remain Loop: " + QString::number(nRemainLoopCount);
    }
}

void MainWindow::onStatusBar_SendTimeDiff(double dSendTimeDiff){
    if (dSendTimeDiff < 1) {
        m_qstrSendTimeDiff = "Next package: 0 sec";
    }else{
        m_qstrSendTimeDiff = "Next package: " + QString::number(dSendTimeDiff) + " sec";
    }
}

void MainWindow::onStatusBar_Invalidate(){
    QString qstrTempLine;
    qstrTempLine = m_qstrSendTimeDiff + "    " + m_qstrRemainLoopCount + "    " + m_qstrSentByte + "    " + m_qstrCurPktTime;
    ui->statusbar->showMessage(qstrTempLine);
}

void MainWindow::onSerialization(bool bSave){
    LOGMSG_INFO("IN");
    if (bSave) {
        LOGMSG_INFO(m_Compo.pConfig->GetConfigPath());
        std::ofstream ofs(m_Compo.pConfig->GetConfigPath());
        boost::archive::text_oarchive oa(ofs);
        oa << m_Compo.pConfig->GetLatestFilePath();
        if (m_Compo.pConfig->GetPcapFiles().size() == 0)
        {
            std::vector<std::string> tempVec;
            tempVec.push_back("Empty.pcap");
            oa << tempVec;
        }
        else
            oa << m_Compo.pConfig->GetPcapFiles();
        if (m_Compo.pConfig->GetMapDstIP().size() == 0)
        {
            std::map<std::string, std::string> tempMap;
            tempMap.insert(std::make_pair("0.0.0.0", "0.0.0.0"));
            oa << tempMap;
        }
        else
            oa << m_Compo.pConfig->GetMapDstIP();
        if (m_Compo.pConfig->GetMapDstIP().size() == 0)
        {
            std::map<std::string, std::string> tempMap;
            tempMap.insert(std::make_pair("0.0.0.0", "0.0.0.0"));
            oa << tempMap;
        }
        else
            oa << m_Compo.pConfig->GetMapSrcIP();
        oa << m_Compo.pConfig->GetSchedulerEnable();
        oa << m_Compo.pConfig->GetOneTimeOnly();
        oa << m_Compo.pConfig->GetSchedulerDay();
        oa << m_Compo.pConfig->GetDateTime().date().year().operator unsigned short();
        oa << m_Compo.pConfig->GetDateTime().date().month().as_number(); // unsigned short
        oa << m_Compo.pConfig->GetDateTime().date().day().as_number(); // unsigned short
        oa << m_Compo.pConfig->GetDateTime().time_of_day().hours(); // long
        oa << m_Compo.pConfig->GetDateTime().time_of_day().minutes(); // long
        oa << m_Compo.pConfig->GetDateTime().time_of_day().seconds(); // long
    }else{
        LOGMSG_INFO(m_Compo.pConfig->GetConfigPath());
        std::ifstream ifs(m_Compo.pConfig->GetConfigPath());
        if (!ifs.is_open()) {
            return;
        }
        boost::archive::text_iarchive ia(ifs);
        std::string strTemp;
        bool bTemp;
        std::vector<std::string> vecStrTemp;
        std::vector<bool> vecBoolTemp;
        std::map<std::string, std::string> mapTemp;
        ia >> strTemp;
        m_Compo.pConfig->SetLatestFilePath(strTemp);
        ia >> vecStrTemp;
        m_Compo.pConfig->RemoveAllPcapFile();
        m_Compo.pConfig->AddPcapFiles(vecStrTemp);
        ia >> mapTemp;
        if (mapTemp.find("0.0.0.0") == mapTemp.end() && mapTemp.size() != 0)
            m_Compo.pConfig->SetMapDstIP(mapTemp);
        mapTemp.clear();
        ia >> mapTemp;
        if (mapTemp.find("0.0.0.0") == mapTemp.end() && mapTemp.size() != 0)
            m_Compo.pConfig->SetMapSrcIP(mapTemp);
        ia >> bTemp;
        m_Compo.pConfig->SetSchedulerEnable(bTemp);
        ia >> bTemp;
        m_Compo.pConfig->SetOneTimeOnly(bTemp);
        ia >> vecBoolTemp;
        m_Compo.pConfig->SetSchedulerDay(vecBoolTemp[0]
                                         , vecBoolTemp[1]
                                         , vecBoolTemp[2]
                                         , vecBoolTemp[3]
                                         , vecBoolTemp[4]
                                         , vecBoolTemp[5]
                                         , vecBoolTemp[6]);
        unsigned short usYear, usMonth, usDay;
        long lHour, lMinute, lSecond;
        ia >> usYear >> usMonth >> usDay >> lHour >> lMinute >> lSecond;
        boost::posix_time::ptime ptTemp(boost::gregorian::date(usYear, usMonth, usDay)
                                        , boost::posix_time::time_duration(lHour, lMinute, lSecond));
        m_Compo.pConfig->SetDateTime(ptTemp);
    }
    LOGMSG_INFO("OUT");
}

void MainWindow::onListWidgetNextFile(){
    LOGMSG_INFO("IN");
    int nCurRow = ui->listWidget_FileList->currentRow();
    int nTotalFiles = ui->listWidget_FileList->count();
    nCurRow = ++nCurRow % nTotalFiles;
    ui->listWidget_FileList->setCurrentRow(nCurRow);
    LOGMSG_INFO("OUT");
}
