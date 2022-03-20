#include "DownloadForm.h"
#include "ui_DownloadForm.h"

DownloadForm::DownloadForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DownloadForm)
{
    ui->setupUi(this);
    ui->pushButton->setIcon(QIcon(":/img/res/start.png"));
    ui->pushButton_2->setIcon(QIcon(":/img/res/stop.png"));
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_2->setVisible(false);
}

DownloadForm::~DownloadForm()
{
    delete ui;
}

void DownloadForm::SetValue(int cur, int max)
{
    if (ui->progressBar->maximum() != max) {
        ui->progressBar->setMaximum(max);
    }
    ui->progressBar->setValue(cur);
}

void DownloadForm::on_pushButton_clicked()
{
    if (mIsStop) {
        StartSignal();
        ui->pushButton_2->setEnabled(true);
    } else {
        StopSignal();
    }
    mIsStop = !mIsStop;
}

void DownloadForm::on_pushButton_2_clicked()
{
    StopSignal();
}

void DownloadForm::on_progressBar_valueChanged(int value)
{
    if (value == ui->progressBar->maximum()) {
        ui->pushButton_2->setEnabled(false);
    }
}
