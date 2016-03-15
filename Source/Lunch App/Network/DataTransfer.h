#pragma once

#include <QObject>
#include "NetEntity.h"


class RestClient;

class DataTransfer : public QObject
{
	Q_OBJECT
public:
				DataTransfer( QObject* parent  = 0 );
				~DataTransfer();

	void		getMenu();

private slots:
	void		onMenuFinished( const NetEntity& entity );

private:
	RestClient*	restClient;
};

