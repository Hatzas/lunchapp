#include "DataTransfer.h"
#include "RestClient.h"
#include <QJson/Parser>

DataTransfer::DataTransfer(QObject* parent) : QObject(parent)
{
    restClient = new RestClient(this);

    connect(restClient, SIGNAL(replyFinished(NetEntity)), this, SLOT(onMenuFinished(NetEntity)));
}

DataTransfer::~DataTransfer()
{
}

void DataTransfer::getMenu()
{
	NetEntity entity;
    entity.setMethodType(ePostMethod);
    entity.setMethodUrl("menu/get");
    entity.setRequestBody("");

	restClient->pushRequest( entity );
}

void DataTransfer::onMenuFinished(const NetEntity& entity)
{
    QJson::Parser json;
    json.parse(QByteArray(entity.getResult().toUtf8()));

	emit userMenu();
}
