#ifndef SCHEDULERDIALOG_H
#define SCHEDULERDIALOG_H

#include <QDialog>

namespace Ui {
class SchedulerDialog;
}

class SchedulerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SchedulerDialog(QWidget *parent = 0);
    ~SchedulerDialog();

private:
    Ui::SchedulerDialog *ui;
};

#endif // SCHEDULERDIALOG_H
