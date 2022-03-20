#ifndef DOWNLOADFORM_H
#define DOWNLOADFORM_H

#include <QWidget>

namespace Ui {
class DownloadForm;
}

class DownloadForm : public QWidget
{
    Q_OBJECT

public:
    explicit DownloadForm(QWidget *parent = nullptr);
    ~DownloadForm();

public slots:
    void SetValue(int cur, int max);

signals:
    void StartSignal();
    void PauseSignal();
    void StopSignal();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_progressBar_valueChanged(int value);

private:
    Ui::DownloadForm *ui;
    bool mIsStop = true;
};

#endif // DOWNLOADFORM_H
