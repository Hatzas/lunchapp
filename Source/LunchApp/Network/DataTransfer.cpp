#include "DataTransfer.h"
#include "RestClient.h"
#include <QJsonDocument>

static const QString kWebApiUrl = "http://194.102.98.239/api/";

DataTransfer::DataTransfer(QObject* parent) : QObject(parent)
{
    restClient = new RestClient(kWebApiUrl, this);

    connect(restClient, SIGNAL(replyFinished(NetEntity)), this, SLOT(onRequestFinished(NetEntity)));
}

DataTransfer::~DataTransfer()
{
}

void DataTransfer::getMenu(const QDate& startDate, const QDate& endDate)
{
	NetEntity entity;
    entity.setMethodType(eGetMethod);
    entity.setMethodUrl("menu");
	entity.addRequestParam("StartDate","2016-03-21");
	entity.addRequestParam("EndDate", "2016-03-25");

	restClient->pushRequest(entity);
}

void DataTransfer::getUserMenu(const QDate& startDate, const QDate& endDate)
{
	NetEntity entity;
	entity.setMethodType(eGetMethod);
	entity.setMethodUrl("usermenu");
	entity.addRequestParam("StartDate", "2016-03-21");
	entity.addRequestParam("EndDate", "2016-03-25");

	restClient->pushRequest(entity);
}

void DataTransfer::getDish()
{
	NetEntity entity;
	entity.setMethodType(eGetMethod);
	entity.setMethodUrl("dish");

	restClient->pushRequest(entity);
}

void DataTransfer::getDishCategory()
{
	NetEntity entity;
	entity.setMethodType(eGetMethod);
	entity.setMethodUrl("dishcategory");

	restClient->pushRequest(entity);
}

void DataTransfer::onRequestFinished(const NetEntity& entity)
{
	bool ok;
	QJsonDocument json = QJsonDocument::fromJson(entity.getResult().toUtf8());

	emit menuFinished();
}
