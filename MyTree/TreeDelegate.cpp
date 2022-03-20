#include "TreeDelegate.h"
#include <QHeaderView>
#include <QDebug>
#include <QPushButton>
#include "MyTree/TreeItem.h"
#include "DownloadForm.h"

namespace {
    constexpr int IDENT = 20;
    const QColor COLOR_HOVER(250, 138, 56, 255);
    const QColor COLOR_SELECTED(248, 192, 80, 255);
    constexpr int LINE_HEIGHT = 30;
    constexpr qreal TRIANGLE_WIDTH = IDENT / 4.0;
    constexpr qreal PADDING = IDENT / 8.0;
    const QColor HIGHTLIGHT_TEXT_COLOR(255, 0, 0);
}

TreeDelegate::TreeDelegate(QTreeView *view, QObject *parent) : QStyledItemDelegate(parent), mView(view)
{
    SetQss();
}

void TreeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == 5 && isButton) {
        mView->openPersistentEditor(index);
    }
    QStyledItemDelegate::paint(painter, option, index);
}

QSize TreeDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize s = QStyledItemDelegate::sizeHint(option, index);
    return QSize(s.width(), 50);
}

bool TreeDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

QWidget* TreeDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (index.column() == 5) {
        TreeItem* item = index.data(Qt::UserRole).value<TreeItem*>();
        if (item->data(3) == "dir") {
            return nullptr;
        }
        DownloadForm *d = new DownloadForm(parent);
        connect(d, &DownloadForm::StartSignal, this, [d, this, item](){
            qDebug() << item->data(0);
            HttpManager *mHttp = new HttpManager();
            qDebug() << mHttp;
            connect(mHttp, &HttpManager::DownLoadProcessSignal, this, [d](int all, int cur){
                d->SetValue(cur, all);
            });
            connect(mHttp, &HttpManager::DownLoadFinishedSignal, this, [mHttp](){
                qDebug() << mHttp;
                if (mHttp != nullptr) {
                    delete mHttp;
                }
            });
            mHttp->DownLoadFile(item->data(2).toUrl(), item->data(0).toString(), item->data(4).toInt());
        });
        return d;
    }
    return QStyledItemDelegate::createEditor(parent, option, index);
}

void TreeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QStyledItemDelegate::setModelData(editor, model, index);
}

void TreeDelegate::SetQss()
{
    mView->header()->setDefaultAlignment(Qt::AlignCenter);
    QString qss = QString("QHeaderView::section { \
                    height:20px; \
                    border:0.5px solid rgb(210, 210, 210); \
                    background-color:qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:0 white, stop:1 rgba(239, 239, 239, 255)) \
                    } \
                    QTreeView{ undefined\
                    show-decoration-selected:1; \
                    outline:0.5px; \
                    } \
                    QTreeView::item:hover{ \
                    background:rgba(%1,%2,%3,%4); \
                    } \
                    QTreeView::item:selected{ \
                    background:rgba(%5,%6,%7,%8); \
                    }").arg(QString::number(COLOR_HOVER.red()), QString::number(COLOR_HOVER.green()), QString::number(COLOR_HOVER.blue()), QString::number(COLOR_HOVER.alpha()),
    QString::number(COLOR_SELECTED.red()), QString::number(COLOR_SELECTED.green()), QString::number(COLOR_SELECTED.blue()),QString::number(COLOR_SELECTED.alpha()));
    mView->setStyleSheet(qss);
}
