#include "DataTransfer.h"
#include "RestClient.h"
#include <QJsonDocument>

static const QString kWebApiUrl = "http://www.google.ro/";

DataTransfer::DataTransfer(QObject* parent) : QObject(parent)
{
    restClient = new RestClient(kWebApiUrl, this);

    connect(restClient, SIGNAL(replyFinished(NetEntity)), this, SLOT(onMenuFinished(NetEntity)));
}

DataTransfer::~DataTransfer()
{
}

void DataTransfer::getMenu()
{
	NetEntity entity;
    entity.setMethodType(ePostMethod);
    entity.setMethodUrl("search?q=qt");
    entity.setRequestBody("");

	restClient->pushRequest( entity );
}

void DataTransfer::onMenuFinished(const NetEntity& entity)
{
	bool ok;
	QJsonDocument json = QJsonDocument::fromJson(entity.getResult().toUtf8());

	emit userMenu();
}
