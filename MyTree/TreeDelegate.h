#ifndef TREEDELEGATE_H
#define TREEDELEGATE_H
#include <QStyledItemDelegate>
#include <QTreeView>
#include "HttpManager/HttpManager.h"

class TreeDelegate : public QStyledItemDelegate
{
public:
    TreeDelegate(QTreeView *view, QObject *parent = nullptr);
    virtual ~TreeDelegate() {};

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model,const QStyleOptionViewItem &option, const QModelIndex &index) override;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    bool isButton = false;
private:
    QTreeView *mView;
    void SetQss();
};

#endif // TREEDELEGATE_H
