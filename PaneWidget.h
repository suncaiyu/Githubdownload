#ifndef PANEWIDGET_H
#define PANEWIDGET_H

#include <QWidget>
#include <HttpManager/HttpManager.h>
#include <MyTree/TreeModel.h>
#include <MyTree/TreeDelegate.h>

QT_BEGIN_NAMESPACE
namespace Ui { class PaneWidget; }
QT_END_NAMESPACE

class PaneWidget : public QWidget
{
    Q_OBJECT

public:
    PaneWidget(QWidget *parent = nullptr);
    ~PaneWidget();

private slots:
    void on_pushButton_clicked();

private:
    Ui::PaneWidget *ui;
    std::shared_ptr<HttpManager> mManager;
    //TreeModel *mModel;
    std::shared_ptr<TreeModel> mModel;
    std::shared_ptr<TreeDelegate> mDelegate;
};
#endif // PANEWIDGET_H
