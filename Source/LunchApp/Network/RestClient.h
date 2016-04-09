#pragma once

#include <QtNetwork/QNetworkAccessManager>
#include <QMutex>
#include "NetEntity.h"


class QNetworkReply;
//class QAuthenticator;

typedef QMap<QNetworkReply*, NetEntity>		MapType;


class RestClient : public QObject
{
	Q_OBJECT

public:
                RestClient(const QString& apiUrl, QObject* parent = 0);
				~RestClient();

    void		pushRequest(const NetEntity& entity);
    void        setWebApiUrl(const QString& apiUrl);

signals:
    void		replyFinished(const NetEntity& entity);

private slots:
    void		onNetworkReply(QNetworkReply* reply);
//	void		provideAuthenication(QNetworkReply* reply, QAuthenticator* auth);
private:
	QNetworkAccessManager*		networkManager;
    QString                     webApiUrl;
	QMutex						listMutex;
	MapType						requestList;
};

