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
	return QString(); // TODO make json from request params
}

void NetEntity::addRequestParam(const QString& key, const QString& value)
{
	requestParams.push_back(QueryPair(key, value));
}

void NetEntity::addRequestParam(const QueryPair& queryPair)
{
	requestParams.push_back(queryPair);
}
