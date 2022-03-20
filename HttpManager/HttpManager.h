#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <MyTree/TreeItem.h>

class HttpManager : public QObject
{
    Q_OBJECT
public:
    HttpManager();
    ~HttpManager();
    void GetInfo(const QUrl &url, TreeItem *item = nullptr);
    void DownLoadFile(const QUrl &url, const QString &name, int size);
    
signals:
    void DoneSignal();
    void DownLoadFinishedSignal();
    void DownLoadProcessSignal(int all, int cur);

private slots:
    void GetInfoSlot();

private:
    void ProcessJson(const QByteArray &ba, TreeItem* item = nullptr);
    void ProcessJsonDocument(const QJsonDocument &jc, TreeItem* item = nullptr);
    TreeItem *SetData(TreeItem *item, const QJsonObject &object);
    TreeItem *mItem;
    /*QNetworkAccessManager* mManager;*/
};

#endif // HTTPMANAGER_H
