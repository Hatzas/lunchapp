#pragma once

#include <QObject>
#include "NetEntity.h"


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
	void		menuFinished();
    void        userMenuFinished();

private slots:
    void		onRequestFinished(const NetEntity& entity);

private:
	RestClient*	restClient;
};

