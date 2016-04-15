#include "NetEntity.h"


QueryPair::QueryPair(const QString& k, const QString& v) :
	key(k),
	value(v)
{
}


NetEntity::NetEntity(const MethodType method) :
	methodType( method )
{
}

NetEntity::~NetEntity()
{
}

QString NetEntity::getJsonRequest() const
{
	return jsonBody;
}

void NetEntity::setPostBody(const QString& json)
{
	jsonBody = json;
}

void NetEntity::addRequestParam(const QString& key, const QString& value)
{
	requestParams.push_back(QueryPair(key, value));
}

void NetEntity::addRequestParam(const QueryPair& queryPair)
{
	requestParams.push_back(queryPair);
}

void NetEntity::getDates(QDate& startDate, QDate& endDate) const
{
	startDate = requestStartDate;
	endDate = requestEndDate;
}

void NetEntity::setDates(const QDate& startDate, const QDate& endDate)
{
	requestStartDate = startDate;
	requestEndDate = endDate;
}
