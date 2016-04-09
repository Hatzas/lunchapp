#include "DataTransfer.h"
#include "RestClient.h"
#include "Model/Day.h"
#include "Model/Dish.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPixmap>

static const QString kWebApiUrl = "http://194.102.98.239/api/";

static const QString kRatingHappy	= "happy";
static const QString kRatingWow		= "wow";
static const QString kRatingMeah	= "meah";


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
	entity.getDates(startDate, endDate);
	Week week(startDate, endDate, vdays);

	emit menuFinished(week);
}

Dish::EDishRating DataTransfer::translateRating(const QString& rating) const
{
	if(rating == kRatingHappy)
		return Dish::eHappy;
	else if(rating == kRatingWow)
		return Dish::eWow;
	else if(rating == kRatingMeah)
		return Dish::eMeah;

	return Dish::eNotRated;
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

		foreach(const QJsonValue& dishValue, dishArray)
		{
			QJsonObject dishObject = dishValue.toObject();

			QString name = dishObject["Name"].toString();
			QString description = dishObject["Description"].toString();

			// picture
			QJsonObject pictureObj = dishObject["DishPicture"].toObject();
			QString pictureBytes = pictureObj["Thumbnail"].toString();
			QPixmap pixmap;
			pixmap.loadFromData(QByteArray::fromBase64(pictureBytes.toUtf8()), "PNG");

			// statistics
			Dish dish(name, description, pixmap, 0);

			dish.setCourseNum(dishObject["Category"].toString().toInt());
			dish.setIdentifier(dishObject["Serial"].toString());
			QJsonArray statsArray = dishObject["DishStatistics"].toArray();
			foreach(const QJsonValue& statsValue, statsArray)
			{
				QJsonObject statsObj = statsValue.toObject();
				Dish::EDishRating dishRating = translateRating(statsObj["Rating"].toString());
				dish.setUserRating(dishRating);
				dish.setNumHappies(statsObj["RatingCount"].toInt());
			}
			
			dishes.push_back(dish);
		}

		days.push_back(Day(day, dishes));
	}
}
