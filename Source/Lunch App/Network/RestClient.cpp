#include "RestClient.h"
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>


RestClient::RestClient(const QString &apiUrl, QObject* parent) : QObject(parent),
    webApiUrl(apiUrl)
{
    networkManager = new QNetworkAccessManager(this);

    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onNetworkReply(QNetworkReply*)));
}

RestClient::~RestClient()
{
	delete networkManager;
}

void RestClient::pushRequest(const NetEntity& entity)
{
    QUrl url(webApiUrl + entity.getMethodUrl());
	QNetworkRequest request( url );	
    QNetworkReply* reply = networkManager->post(request, QByteArray(entity.getRequestBody().toUtf8()));

	listMutex.lock();
    requestList.insert(reply, entity);
	listMutex.unlock();
}

void RestClient::setWebApiUrl(const QString& apiUrl)
{
    webApiUrl = apiUrl;
}

void RestClient::onNetworkReply(QNetworkReply* reply)
{
    NetEntity entity;
    bool found = false;
    listMutex.lock();
    MapType::Iterator it = requestList.find(reply);

    if(it != requestList.end())
    {
        entity = *it;
        requestList.erase(it);
        found = true;
    }
    listMutex.unlock();

    if(reply->error() == QNetworkReply::NoError)
	{
        if(found)
		{
            entity.setResult(reply->readAll());
		}
	}

    if(found)
    {
        emit replyFinished(entity);
    }
	reply->deleteLater();
}
