#include "schedulerdialog.h"
#include "ui_schedulerdialog.h"

#include "Config.h"
#include "LogMgr.h"

SchedulerDialog::SchedulerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SchedulerDialog)
{
    ui->setupUi(this);
    // handle checkbox
    connect(ui->checkBox_Enable, SIGNAL(stateChanged(int)), this, SLOT(onEnable(int)));
    // handle push button
    connect(ui->pushButton_Confirm, SIGNAL(pressed()), this, SLOT(onConfirm()));
    connect(ui->pushButton_Cancel, SIGNAL(pressed()), this, SLOT(onCancel()));
    // handle radio button
    connect(ui->radioButton_OneTimeOnly, SIGNAL(pressed()), this, SLOT(onOneTimeOnly()));
    connect(ui->radioButton_Weekly, SIGNAL(pressed()), this, SLOT(onWeekly()));
}

SchedulerDialog::~SchedulerDialog()
{
    delete ui;
}

void SchedulerDialog::InitComponent(Config* pConfig){
    m_pConfig = pConfig;
    SwitchUIStatus_Init();
}

void SchedulerDialog::SwitchUIStatus_Init(){
    // handle radio button
    ui->radioButton_Weekly->setDown(false);
    ui->radioButton_OneTimeOnly->setDown(false);

    ui->radioButton_Weekly->setEnabled(false);
    ui->radioButton_OneTimeOnly->setEnabled(false);
    // handle check box
    ui->checkBox_Enable->setCheckState(Qt::Unchecked);
    ui->checkBox_Monday->setCheckState(Qt::Unchecked);
    ui->checkBox_Tuesday->setCheckState(Qt::Unchecked);
    ui->checkBox_Wednesday->setCheckState(Qt::Unchecked);
    ui->checkBox_Thursday->setCheckState(Qt::Unchecked);
    ui->checkBox_Friday->setCheckState(Qt::Unchecked);
    ui->checkBox_Saturday->setCheckState(Qt::Unchecked);
    ui->checkBox_Sunday->setCheckState(Qt::Unchecked);

    ui->checkBox_Enable->setEnabled(true);
    ui->checkBox_Monday->setEnabled(false);
    ui->checkBox_Tuesday->setEnabled(false);
    ui->checkBox_Wednesday->setEnabled(false);
    ui->checkBox_Thursday->setEnabled(false);
    ui->checkBox_Friday->setEnabled(false);
    ui->checkBox_Saturday->setEnabled(false);
    ui->checkBox_Sunday->setEnabled(false);

    // handle calendarWidget
    ui->calendarWidget->setEnabled(false);

    // handle timeEdit
    ui->timeEdit->setEnabled(false);
}

void SchedulerDialog::SwitchUIStatus_OneTimeOnly(){
    // handle check box
    ui->checkBox_Monday->setEnabled(false);
    ui->checkBox_Tuesday->setEnabled(false);
    ui->checkBox_Wednesday->setEnabled(false);
    ui->checkBox_Thursday->setEnabled(false);
    ui->checkBox_Friday->setEnabled(false);
    ui->checkBox_Saturday->setEnabled(false);
    ui->checkBox_Sunday->setEnabled(false);
    // handle calendarWidget
    ui->calendarWidget->setEnabled(true);
    // handle timeEdit
    ui->timeEdit->setEnabled(true);
}

void SchedulerDialog::SwitchUIStatus_Weekly(){
    // handle check box
    ui->checkBox_Monday->setEnabled(true);
    ui->checkBox_Tuesday->setEnabled(true);
    ui->checkBox_Wednesday->setEnabled(true);
    ui->checkBox_Thursday->setEnabled(true);
    ui->checkBox_Friday->setEnabled(true);
    ui->checkBox_Saturday->setEnabled(true);
    ui->checkBox_Sunday->setEnabled(true);
    // handle calendarWidget
    ui->calendarWidget->setEnabled(false);
    // handle timeEdit
    ui->timeEdit->setEnabled(true);
}

void SchedulerDialog::SwitchUIStatus_Enable(const bool& bIN){
    // handle radio button
    ui->radioButton_Weekly->setEnabled(bIN);
    ui->radioButton_OneTimeOnly->setEnabled(bIN);

    if (bIN && ui->radioButton_Weekly->isChecked()) {
        SwitchUIStatus_Weekly();
    }else if (bIN && ui->radioButton_OneTimeOnly->isChecked()) {
        SwitchUIStatus_OneTimeOnly();
    }else{
        // handle check box
        ui->checkBox_Monday->setEnabled(false);
        ui->checkBox_Tuesday->setEnabled(false);
        ui->checkBox_Wednesday->setEnabled(false);
        ui->checkBox_Thursday->setEnabled(false);
        ui->checkBox_Friday->setEnabled(false);
        ui->checkBox_Saturday->setEnabled(false);
        ui->checkBox_Sunday->setEnabled(false);
        // handle calendarWidget
        ui->calendarWidget->setEnabled(false);
        // handle timeEdit
        ui->timeEdit->setEnabled(false);
    }
}

void SchedulerDialog::onEnable(int nState){
    nState == Qt::Checked ? m_pConfig->SetSchedulerEnable(true) : m_pConfig->SetSchedulerEnable(false);
    nState == Qt::Checked ? SwitchUIStatus_Enable(true) : SwitchUIStatus_Enable(false);
}

void SchedulerDialog::onConfirm(){
    LOGMSG_INFO("IN");
    // handle calender widget
    QDate SchedulerDate = ui->calendarWidget->selectedDate();
    // handle timeEdit
    QTime SchedulerTime = ui->timeEdit->time();
    if (ui->radioButton_OneTimeOnly->isChecked()) {
        using namespace boost::posix_time;
        int nYear, nMonth, nDay;
        SchedulerDate.getDate(&nYear, &nMonth, &nDay);
        int nHour = SchedulerTime.hour();
        int nMinute = SchedulerTime.minute();
        int nSecond = SchedulerTime.second();
        ptime pt(boost::gregorian::date(nYear, nMonth, nDay)
                         , time_duration(nHour, nMinute, nSecond));
        m_pConfig->SetDateTime(pt);

        std::stringstream ssTempLine;
        ssTempLine << nYear << "/"
                   << nMonth << "/"
                   << nDay << " "
                   << nHour << ":"
                   << nMinute << ":"
                   << nSecond;
        LOGMSG_INFO(ssTempLine.str());
    }else{
        using namespace boost::posix_time;
        ptime now = second_clock::local_time();
        int nYear, nMonth, nDay;
        nYear = now.date().year();
        nMonth = now.date().month();
        nDay = now.date().day();
        int nHour = SchedulerTime.hour();
        int nMinute = SchedulerTime.minute();
        int nSecond = SchedulerTime.second();
        ptime pt(boost::gregorian::date(nYear, nMonth, nDay)
                         , time_duration(nHour, nMinute, nSecond));
        m_pConfig->SetDateTime(pt);

        std::stringstream ssTempLine;
        ssTempLine << nYear << "/"
                   << nMonth << "/"
                   << nDay << " "
                   << nHour << ":"
                   << nMinute << ":"
                   << nSecond;
        LOGMSG_INFO(ssTempLine.str());
    }

    // handle radio box
    m_pConfig->SetOneTimeOnly(ui->radioButton_OneTimeOnly->isChecked());
    // handle check box
    m_pConfig->SetSchedulerEnable(ui->checkBox_Enable->isChecked());
    m_pConfig->SetSchedulerDay(ui->checkBox_Sunday->isChecked()
                               , ui->checkBox_Monday->isChecked()
                               , ui->checkBox_Tuesday->isChecked()
                               , ui->checkBox_Wednesday->isChecked()
                               , ui->checkBox_Thursday->isChecked()
                               , ui->checkBox_Friday->isChecked()
                               , ui->checkBox_Saturday->isChecked());
    LOGMSG_INFO("OUT");
    close();
}

void SchedulerDialog::onCancel(){
    close();
}

void SchedulerDialog::onOneTimeOnly(){
    SwitchUIStatus_OneTimeOnly();
}

void SchedulerDialog::onWeekly(){
    SwitchUIStatus_Weekly();
}
