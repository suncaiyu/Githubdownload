#include "HttpManager.h"
#include <QNetworkReply>
#include <QDebug>
#include <QUrl>
#include <QFile>
#include <QCoreApplication>
HttpManager::HttpManager()
{

}

HttpManager::~HttpManager()
{

}

void HttpManager::GetInfo(const QUrl &url, TreeItem *item)
{
    QNetworkAccessManager* mManager = new QNetworkAccessManager();
    QNetworkReply *replay = mManager->get(QNetworkRequest(url));
    connect(replay, &QNetworkReply::finished, this, [this, item]() {
        QNetworkReply* replay = qobject_cast<QNetworkReply*>(this->sender());
        QByteArray info = replay->readAll();
        ProcessJson(info, item);
    });
    connect(mManager, &QNetworkAccessManager::finished, [mManager]()mutable{
        mManager->deleteLater();
    });
}

void HttpManager::DownLoadFile(const QUrl &url, const QString &name, int size)
{
    QNetworkAccessManager *mDownloadManager = new QNetworkAccessManager();
    QNetworkReply *replay = mDownloadManager->get(QNetworkRequest(url));
    if (replay->error() != QNetworkReply::NoError) {
        qDebug() << ("has error Code :" + QString::number(replay->error()));
        return;
    }
    QFile *mDownLoadFile = new QFile(QCoreApplication::applicationDirPath() + "/download/unknown");
    mDownLoadFile->open(QIODevice::WriteOnly);
    connect(replay, &QNetworkReply::readyRead, this, [this, mDownLoadFile](){
        QNetworkReply *r = qobject_cast<QNetworkReply *>(sender());
        if (r->error() != QNetworkReply::NoError) {
            qDebug() << ("has error Code:" + QString::number(r->error()));
            return;
        }
        qDebug() << "write";
        mDownLoadFile->write(r->readAll());
    });
    connect(replay, &QNetworkReply::downloadProgress, this, [this, mDownLoadFile, size](qint64 cur, qint64 all){
        QNetworkReply *r = qobject_cast<QNetworkReply *>(sender());
        if (r->error() != QNetworkReply::NoError) {
            qDebug() << ("has error Code:" + QString::number(r->error()));
            return;
        }
        DownLoadProcessSignal(all, cur);
        if (all == cur) {
            mDownLoadFile->close();
            if (mDownLoadFile->size() == 0) {
                qDebug() << ("文件大小是0");
                mDownLoadFile->remove();
                return;
            }
            if (mDownLoadFile->size() != size) {
                qDebug() << ("文件大小不对");
                mDownLoadFile->remove();
                return;
            }
        }
    });
//    connect(replay, &QNetworkReply::finished, this, [this, mDownLoadFile, name, size](){
//        QNetworkReply *r = qobject_cast<QNetworkReply *>(sender());
//        if (r->error() != QNetworkReply::NoError) {
//            qDebug() << ("has error Code:" + QString::number(r->error()));
//            mDownLoadFile->remove();
//            return;
//        }
//        mDownLoadFile->close();
//        if (mDownLoadFile->size() == 0) {
//            qDebug() << ("文件大小是0");
//            mDownLoadFile->remove();
//            return;
//        }
//        if (mDownLoadFile->size() != size) {
//            qDebug() << ("文件大小不对");
//            mDownLoadFile->remove();
//            return;
//        }
//        qDebug() << "download finished";
//        mDownLoadFile->rename(qApp->applicationDirPath() + "/download/" + name);
//        delete mDownLoadFile;
//    });
    connect(mDownloadManager, &QNetworkAccessManager::finished, [this, mDownloadManager, mDownLoadFile, name](){
        mDownLoadFile->rename(qApp->applicationDirPath() + "/download/" + name);
        delete mDownLoadFile;
        mDownloadManager->deleteLater();
        DownLoadFinishedSignal();
    });
}

void  HttpManager::GetInfoSlot()
{
    QNetworkReply *replay = qobject_cast<QNetworkReply *>(this->sender());
    QByteArray info = replay->readAll();
    ProcessJson(info);
}

void HttpManager::ProcessJson(const QByteArray &ba, TreeItem* item)
{
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(ba, &jsonError);
    if (!document.isNull() && (jsonError.error == QJsonParseError::NoError)) {
        qDebug() << "done";
    } else {
        if (document.isNull()) {
            qDebug() << "null";
        }
        if (jsonError.error != QJsonParseError::NoError) {
            qDebug() << "error:" << jsonError.error;
        }
    }
    if (document.isArray()) {
        ProcessJsonDocument(document, item);
    } else {
        QMap<QString, QVariant>json = document.toVariant().toMap();
        qDebug() << json["message"];
        qDebug() << "not array";
    }
}

void HttpManager::ProcessJsonDocument(const QJsonDocument &document, TreeItem* item)
{
    QStringList dirList;
    QJsonArray array = document.array();
    for (int i = 0; i < array.size(); ++i) {
        QJsonObject object = array.at(i).toObject();
        if (object.value("type").toString() != "file") {
            if (object.value("size").toInt() == 0) {
                TreeItem *tmpItem = SetData(item, object);
                GetInfo(QUrl(object.value("url").toString()), tmpItem);
            }
        } else {
            if (object.value("size").toInt() != 0) {
                SetData(item, object);
            }
        }
    }
    DoneSignal();
}

TreeItem *HttpManager::SetData(TreeItem* item, const QJsonObject& object)
{
    TreeItem* tmpItem = new TreeItem();
    tmpItem->setItemData("name", object.value("name").toString());
    tmpItem->setItemData("type", object.value("type").toString());
    tmpItem->setItemData("url", object.value("url").toString());
    tmpItem->setItemData("download_url", object.value("download_url").toString());
    tmpItem->setItemData("size", QString::number(object.value("size").toInt()));
    item->appendChild(tmpItem);
    return tmpItem;
}
