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

	void		getMenu(const QDateTime& startDate, const QDateTime& endDate);
	void		getUserMenu(const QDateTime& startDate, const QDateTime& endDate);
	void		getDish();
	void		getDishCategory();
	
signals:
	void		menuFinished(const Week& week);
    void        userMenuFinished();

private slots:
    void		onRequestFinished(const NetEntity& entity);

private:
	void		extractDays(const QJsonArray& json, std::vector<Day>& days);

private:
	RestClient*	restClient;
};

