#include "PaneWidget.h"
#include "ui_PaneWidget.h"

// 测试智能指针的delete
void deleter(TreeModel* x) {
    qDebug() << "DELETER FUNCTION CALLED";
    delete x;
}

PaneWidget::PaneWidget(QWidget* parent)
    : QWidget(parent), mManager(std::make_shared<HttpManager>())
    , ui(new Ui::PaneWidget)
{
    ui->setupUi(this);
    mModel = std::shared_ptr<TreeModel>(new TreeModel(QStringList() <<
        "name" << "url" << "download_url" << "type" << "size" << "download"), deleter);
    mDelegate = (std::make_shared<TreeDelegate>(ui->treeView));
//    ui->treeView->setAnimated(true);
    //ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->setItemDelegate(mDelegate.get());
    ui->pushButton->setIcon(QIcon(":/img/res/search.png"));
}

PaneWidget::~PaneWidget()
{
    delete ui;
}


void PaneWidget::on_pushButton_clicked()
{
    // 这里需要delete model
    //if (mModel != nullptr) {
    //    delete mModel;
    //    mModel = nullptr;
    //}
    std::shared_ptr<TreeModel>tmmp = std::shared_ptr<TreeModel>(new TreeModel(QStringList() <<
        "name" << "url" << "download_url" << "type" << "size" << "download"), deleter);
    tmmp.swap(mModel);
    //mModel = std::make_shared<TreeModel>(QStringList() <<
    //    "name" << "url" << "download_url" << "type" << "size" << "download");
    mDelegate->isButton = false;
    mManager->GetInfo(QUrl(ui->lineEdit->text()), mModel->root());
    ui->treeView->setModel(mModel.get());
    connect(mManager.get(), &HttpManager::DoneSignal, this, [this]() {
        mDelegate->isButton = true;
        ui->treeView->doItemsLayout();
    });
}
