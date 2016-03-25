#pragma once

#include <QString>
#include <QList>
#include <QDate>

class QueryPair
{
public:
				QueryPair(const QString& k, const QString& v);
	QString		key;
	QString		value;
};

enum MethodType
{
	eGetMethod = 0,
	ePostMethod,
	ePatchMethod,
	eDeleteMethod,
};


class NetEntity
{
public:
							NetEntity(const MethodType method = ePostMethod);
							~NetEntity();

		MethodType			getMethodType() const { return methodType; }
        void				setMethodType(const MethodType method) { methodType = method; }

		QString				getMethodUrl() const { return methodUrl; }
        void				setMethodUrl(const QString& url) { methodUrl = url; }

		QString				getJsonRequest() const;
		QList<QueryPair>	getRequestParams() const { return requestParams; }
        void				addRequestParam(const QString& key, const QString& value);
		void				addRequestParam(const QueryPair& queryPair);
		
		QString				getResult() const { return result; }
        void				setResult(const QString& jsonResult) { result = jsonResult; }

		QString				getError() const { return error; }
		void				setError(const QString& errorString) { error = errorString; }

		void				getDates(QDate& startDate, QDate& endDate) const;
		void				setDates(const QDate& startDate, const QDate& endDate);
private:
	MethodType				methodType;			// method request type
	QString					methodUrl;			// e.g. "meal/get"
	QList<QueryPair>		requestParams;
	QString					result;
	QString					error;
	QDate					requestStartDate;
	QDate					requestEndDate;
};
