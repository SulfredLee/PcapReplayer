#ifndef SCHEDULERDIALOG_H
#define SCHEDULERDIALOG_H

#include <QDialog>
#include <QtCore/QDateTime>

namespace Ui {
class SchedulerDialog;
}

class Config;

class SchedulerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SchedulerDialog(QWidget *parent = 0);
    ~SchedulerDialog();

    void InitComponent(Config* pConfig);
private:
    // handle UI
    void SwitchUIStatus_Init();
    void SwitchUIStatus_OneTimeOnly();
    void SwitchUIStatus_Weekly();
    void SwitchUIStatus_Enable(const bool& bIN);
private:
    Ui::SchedulerDialog *ui;
    Config* m_pConfig;
private slots:
    void onEnable(int);
    void onConfirm();
    void onCancel();
    // handle radio button
    void onOneTimeOnly();
    void onWeekly();
};

#endif // SCHEDULERDIALOG_H
