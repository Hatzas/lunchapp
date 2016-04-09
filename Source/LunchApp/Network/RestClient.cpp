#include "RestClient.h"
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QUrlQuery>
//#include <QAuthenticator>


RestClient::RestClient(const QString &apiUrl, QObject* parent) : QObject(parent),
    webApiUrl(apiUrl)
{
    networkManager = new QNetworkAccessManager(this);

//	connect(networkManager, SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator*)), this, SLOT(provideAuthenication(QNetworkReply*, QAuthenticator*)));
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onNetworkReply(QNetworkReply*)));
}

RestClient::~RestClient()
{
}

void RestClient::pushRequest(const NetEntity& entity)
{
	QUrl url(webApiUrl + entity.getMethodUrl());
    QNetworkReply* reply = 0;

	switch(entity.getMethodType())
	{
	case eGetMethod:
		{
			QDate startDate, endDate;
			entity.getDates(startDate, endDate);
			QUrlQuery queryUrl;
			queryUrl.addQueryItem("startDate", startDate.toString(Qt::ISODate));
			queryUrl.addQueryItem("endDate", endDate.toString(Qt::ISODate));
			//queryUrl.addQueryItem("type", "2");

			foreach(const QueryPair& queryPair, entity.getRequestParams())
			{
				queryUrl.addQueryItem(queryPair.key, queryPair.value);
			}

			url.setQuery(queryUrl);
			//url.setUserName("123");
			
			QNetworkRequest request(url);
			reply = networkManager->get(request);
			break;
		}
	case ePostMethod:
		{
			QNetworkRequest request(url);
			reply = networkManager->post(request, entity.getJsonRequest().toUtf8());
			break;
		}
	}

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

	if(found)
	{
		if(reply->error() == QNetworkReply::NoError)
		{
			entity.setResult(reply->readAll());
		}
		else
		{
			entity.setError(reply->errorString());
		}
		
		emit replyFinished(entity);
	}

	reply->deleteLater();
}
/*
void RestClient::provideAuthenication(QNetworkReply* reply, QAuthenticator* auth)
{
	// NTLM Authentication
	QString re = auth->realm();
	QString a = reply->readAll(); // this is just to see what we received
	auth->setUser("");
	//auth->setPassword("");
}
*/