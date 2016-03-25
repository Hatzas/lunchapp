#include "DataTransfer.h"
#include "RestClient.h"
#include "Model/Day.h"
#include "Model/Dish.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPixmap>

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
	entity.setDates(startDate, endDate);
	
	restClient->pushRequest(entity);
}

void DataTransfer::getUserMenu(const QDate& startDate, const QDate& endDate)
{
	NetEntity entity;
	entity.setMethodType(eGetMethod);
	entity.setMethodUrl("usermenu");
	entity.setDates(startDate, endDate);
	
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
	QJsonDocument jsonDoc = QJsonDocument::fromJson(entity.getResult().toUtf8());
	QJsonArray jsonArray;
	std::vector<Day> vdays;

	if(!jsonDoc.isNull())
	{
		if(jsonDoc.isArray())
		{
			jsonArray = jsonDoc.array();
			
			extractDays(jsonArray, vdays);
		}
	}

	QDate startDate;
	QDate endDate;
	Week week(startDate, endDate, vdays);
	emit menuFinished(week);
}

void DataTransfer::extractDays(const QJsonArray& json, std::vector<Day>& days)
{
	days.clear();
	
	foreach(const QJsonValue& val, json)
	{
		QJsonObject dayObject = val.toObject();
		
		//QString date = dayObject["Date"].toString();
		QString day = dayObject["Day"].toString();

		QJsonArray dishArray = dayObject["Dishes"].toArray();
		std::vector<Dish> dishes;

		foreach(const QJsonValue& dish, dishArray)
		{
			QJsonObject dishObject = dish.toObject();

			QString name = dishObject["Name"].toString();
			QString description = dishObject["Description"].toString();

			QJsonObject pictureObj = dishObject["DishPicture"].toObject();
			QString pictureBytes = pictureObj["Thumbnail"].toString();
			QPixmap pixmap;
			pixmap.loadFromData(pictureBytes.toUtf8(), "PNG");

			dishes.push_back(Dish(name, description, pixmap, 0));
		}

		days.push_back(Day(day, dishes));
	}
}
