#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Config.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
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
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::InitComponent(const MainWindowComponent& InCompo){
    m_Compo = InCompo;
}

void MainWindow::SwitchUIStatus_Init(){
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
}

void MainWindow::SwitchUIStatus_Play(){
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
}

void MainWindow::SwitchUIStatus_Pause(){
    SwitchUIStatus_Play();
    // handle Difference
    ui->BtnPlay->setEnabled(true);
    ui->BtnPause->setEnabled(false);
}

void MainWindow::SwitchUIStatus_Stop(){
    SwitchUIStatus_Init();
}

void MainWindow::AddPcapFilesToUI(const QStringList& INFiles){
    // handle UI
    // ui->listWidget_FileList->clear();
    if (INFiles.isEmpty()){
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
}

std::string MainWindow::ConvertQString2String(const QString& qstr){
    // for unix
    return qstr.toUtf8().constData();
    // for windows
    // return qstr.toLocal8Bit().constData(); // we get the current locale for the current system
}

QString MainWindow::ConvertString2QString(const std::string& str){
    return QString::fromStdString(str);
}

std::vector<std::string> MainWindow::ConvertQStringList(const QStringList& qstrList){
    std::vector<std::string> result;
    for (qint32 i = 0; i < qstrList.length(); i++){
        result.push_back(ConvertQString2String(qstrList[i]));
    }
    return result;
}

QStringList MainWindow::ConvertVectorString(const std::vector<std::string>& vecStr){
    QStringList result;
    for (size_t i = 0; i < vecStr.size(); i++){
        result += ConvertString2QString(vecStr[i]);
    }
    return result;
}

bool MainWindow::IsFileExists(const QString& qstrPath){
    QFileInfo check_file(qstrPath);
    return (check_file.exists() && check_file.isFile());
}

void MainWindow::onOpen_File(){
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

    if (fileNames.length() == 0)
        return;
    AddPcapFilesToUI(fileNames);

    m_Compo.pConfig->AddPcapFiles(ConvertQStringList(fileNames));
}

void MainWindow::onOpen_Folder(){
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

    if (strFilePath == "")
        return;

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
}

void MainWindow::onSave_Config(){
    QMessageBox TestingBox;
    TestingBox.setWindowTitle(QString("Wait for input!"));
    TestingBox.show();
    TestingBox.exec();
}

void MainWindow::onLoad_Config(){
    QMessageBox TestingBox;
    TestingBox.setWindowTitle(QString("Wait for input!"));
    TestingBox.show();
    TestingBox.exec();
}

void MainWindow::onScheduler(){
    QMessageBox TestingBox;
    TestingBox.setWindowTitle(QString("Wait for input!"));
    TestingBox.show();
    TestingBox.exec();
}

void MainWindow::onPlay(){
    SwitchUIStatus_Play();
}

void MainWindow::onPause(){
    SwitchUIStatus_Pause();
}

void MainWindow::onStop(){
    SwitchUIStatus_Stop();
}

void MainWindow::onRemove(){
    int nCurIdx = ui->listWidget_FileList->currentRow();
    QListWidgetItem* pItem = ui->listWidget_FileList->takeItem(nCurIdx);
    delete pItem;
    if (ui->listWidget_FileList->count() == 0){
        // handle UI
        ui->BtnRemove->setEnabled(false);
        ui->BtnRemoveAll->setEnabled(false);
    }
    m_Compo.pConfig->RemovePcapFile(nCurIdx);
}

void MainWindow::onRemoveAll(){
    ui->listWidget_FileList->clear();
    // handle UI
    ui->BtnRemove->setEnabled(false);
    ui->BtnRemoveAll->setEnabled(false);

    m_Compo.pConfig->RemoveAllPcapFile();
}

void MainWindow::onRegularPaly(){
    SwitchUIStatus_Play();
}

void MainWindow::onAddSrcMap(){
    QMessageBox TestingBox;
    TestingBox.setWindowTitle(QString("Wait for input!"));
    TestingBox.show();
    TestingBox.exec();
}

void MainWindow::onAddDstMap(){
    QMessageBox TestingBox;
    TestingBox.setWindowTitle(QString("Wait for input!"));
    TestingBox.show();
    TestingBox.exec();
}

void MainWindow::onRemoveScrMapIP(){
    QMessageBox TestingBox;
    TestingBox.setWindowTitle(QString("Wait for input!"));
    TestingBox.show();
    TestingBox.exec();
}

void MainWindow::onRemoveDstMapIP(){
    QMessageBox TestingBox;
    TestingBox.setWindowTitle(QString("Wait for input!"));
    TestingBox.show();
    TestingBox.exec();
}
