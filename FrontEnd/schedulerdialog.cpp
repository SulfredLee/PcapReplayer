#include "schedulerdialog.h"
#include "ui_schedulerdialog.h"

#include "Config.h"

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
}

void SchedulerDialog::SwitchUIStatus_OneTimeOnly(){
    ui->checkBox_Monday->setEnabled(false);
    ui->checkBox_Tuesday->setEnabled(false);
    ui->checkBox_Wednesday->setEnabled(false);
    ui->checkBox_Thursday->setEnabled(false);
    ui->checkBox_Friday->setEnabled(false);
    ui->checkBox_Saturday->setEnabled(false);
    ui->checkBox_Sunday->setEnabled(false);
}

void SchedulerDialog::SwitchUIStatus_Weekly(){
    ui->checkBox_Monday->setEnabled(true);
    ui->checkBox_Tuesday->setEnabled(true);
    ui->checkBox_Wednesday->setEnabled(true);
    ui->checkBox_Thursday->setEnabled(true);
    ui->checkBox_Friday->setEnabled(true);
    ui->checkBox_Saturday->setEnabled(true);
    ui->checkBox_Sunday->setEnabled(true);
}

void SchedulerDialog::SwitchUIStatus_Enable(const bool& bIN){
    // handle radio button
    ui->radioButton_Weekly->setEnabled(bIN);
    ui->radioButton_OneTimeOnly->setEnabled(bIN);

    // handle check box
    ui->checkBox_Monday->setEnabled(bIN);
    ui->checkBox_Tuesday->setEnabled(bIN);
    ui->checkBox_Wednesday->setEnabled(bIN);
    ui->checkBox_Thursday->setEnabled(bIN);
    ui->checkBox_Friday->setEnabled(bIN);
    ui->checkBox_Saturday->setEnabled(bIN);
    ui->checkBox_Sunday->setEnabled(bIN);

    ui->radioButton_Weekly->isChecked() && bIN ? SwitchUIStatus_Weekly() : SwitchUIStatus_OneTimeOnly();
}

void SchedulerDialog::onEnable(int nState){
    nState == Qt::Checked ? m_pConfig->SetSchedulerEnable(true) : m_pConfig->SetSchedulerEnable(false);
    nState == Qt::Checked ? SwitchUIStatus_Enable(true) : SwitchUIStatus_Enable(false);
}

void SchedulerDialog::onConfirm(){
    // handle calender widget
    QDate SchedulerDate = ui->calendarWidget->selectedDate();
    // handle timeEdit
    QTime SchedulerTime = ui->timeEdit->time();
    // handle radio box
    m_pConfig->SetOneTimeOnly(ui->radioButton_OneTimeOnly->isChecked());
    // handle check box
    m_pConfig->SetSchedulerEnable(ui->checkBox_Enable->isChecked());
    m_pConfig->SetSchedulerDay(ui->checkBox_Monday->isChecked()
                               , ui->checkBox_Tuesday->isChecked()
                               , ui->checkBox_Wednesday->isChecked()
                               , ui->checkBox_Thursday->isChecked()
                               , ui->checkBox_Friday->isChecked()
                               , ui->checkBox_Saturday->isChecked()
                               , ui->checkBox_Sunday->isChecked());
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
