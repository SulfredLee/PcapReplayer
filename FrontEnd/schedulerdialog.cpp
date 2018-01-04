#include "schedulerdialog.h"
#include "ui_schedulerdialog.h"

SchedulerDialog::SchedulerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SchedulerDialog)
{
    ui->setupUi(this);
}

SchedulerDialog::~SchedulerDialog()
{
    delete ui;
}
