#include "RestClient.h"
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

static const QString kWebApiUrl = "http://localhost:8080/";

RestClient::RestClient( QObject* parent ) : QObject( parent )
{
	networkManager = new QNetworkAccessManager( this );

	connect( networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onNetworkReply(QNetworkReply*)) );
}

RestClient::~RestClient()
{
	delete networkManager;
}

void RestClient::pushRequest( const NetEntity& entity )
{
	QUrl url( kWebApiUrl + entity.getMethodUrl() );
	QNetworkRequest request( url );	
	QNetworkReply* reply = networkManager->post( request, QByteArray( entity.getRequestBody().toUtf8() ) );

	listMutex.lock();
	requestList.insert( reply, entity );
	listMutex.unlock();
}

void RestClient::onNetworkReply( QNetworkReply* reply )
{
	if( reply->error() == QNetworkReply::NoError )
	{
		NetEntity entity;
		bool found = false;

		listMutex.lock();
		
		MapType::Iterator it = requestList.find( reply );
		
		if( it != requestList.end() )
		{
			entity = *it;
			requestList.erase( it );
			found = true;
		}
		
		listMutex.unlock();

		if( found )
		{
			entity.setResult( reply->readAll() );
			emit replyFinished( entity );
		}
	}
	else
	{
		emit replyError( reply->errorString() );
	}

	reply->deleteLater();
}
