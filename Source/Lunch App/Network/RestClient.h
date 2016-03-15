#pragma once

#include <QtNetwork/QNetworkAccessManager>
#include <QMutex>
#include "NetEntity.h"


class QNetworkReply;

typedef QMap<QNetworkReply*, NetEntity>		MapType;


class RestClient : public QObject
{
	Q_OBJECT

public:
				RestClient( QObject* parent = 0 );
				~RestClient();

	void		pushRequest( const NetEntity& entity );

signals:
	void		replyFinished( const NetEntity& entity );
	void		replyError( const QString& errorMessage );

private slots:
	void		onNetworkReply( QNetworkReply* reply );

private:
	QNetworkAccessManager*		networkManager;

	QMutex						listMutex;
	MapType						requestList;
};

