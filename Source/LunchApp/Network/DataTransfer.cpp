#include "DataTransfer.h"
#include "RestClient.h"
#include <QJsonDocument>

static const QString kWebApiUrl = "http://194.102.98.239/api/";

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
    entity.setMethodType(eGetMethod);
    entity.setMethodUrl("menu");
    entity.addRequestParam("StartDate", "2016-03-01");
	entity.addRequestParam("EndDate", "2016-03-05");

	restClient->pushRequest(entity);
}

void DataTransfer::getUserMenu()
{
	NetEntity entity;
	entity.setMethodType(ePostMethod);
	entity.setMethodUrl("usermenu");
	entity.addRequestParam("StartDate", "2016-03-01");
	entity.addRequestParam("EndDate", "2016-03-05");

	restClient->pushRequest(entity);
}

void DataTransfer::onMenuFinished(const NetEntity& entity)
{
	bool ok;
	QJsonDocument json = QJsonDocument::fromJson(entity.getResult().toUtf8());

	emit menuFinished();
}
