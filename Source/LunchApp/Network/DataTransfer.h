#pragma once

#include <QObject>
#include "NetEntity.h"
#include "Model/Week.h"

class RestClient;

class DataTransfer : public QObject
{
	Q_OBJECT
public:
                DataTransfer(QObject* parent = 0);
				~DataTransfer();

	void		getMenu(const QDate& startDate, const QDate& endDate);
	void		getUserMenu(const QDate& startDate, const QDate& endDate);
	void		getDish();
	void		getDishCategory();
	
signals:
	void		menuFinished(Week& week);
    void        userMenuFinished();

private slots:
    void		onRequestFinished(const NetEntity& entity);

private:
	Dish::EDishRating	translateRating(const QString& rating) const;
	void				extractDays(const QJsonArray& json, std::vector<Day>& days);

private:
	RestClient*	restClient;
};

